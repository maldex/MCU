// Do not remove the include below
#include "Arduino.h"
#include "Servo.h"
#include "EEPROM.h"
///////////////////////////////////////////////
// 3rd: Allow for Stream like output, Serial << "this" << "syntax" << is << cool;
///////////////////////////////////////////////
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

///////////////////////////////////////////////
// 3rd: free memory estimation
///////////////////////////////////////////////
#ifdef __arm__
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
///////////////////////////////////////////////
// 3rd: read/write LONG instead of only BYTE from eeprom, from https://playground.arduino.cc/Code/EEPROMReadWriteLong
///////////////////////////////////////////////
long EEPROMReadlong(long address){
	long four = EEPROM.read(address);
	long three = EEPROM.read(address + 1);
	long two = EEPROM.read(address + 2);
	long one = EEPROM.read(address + 3);
	return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
}
void EEPROMWritelong(int address, long value){
	if (EEPROMReadlong(address) == value) return;  // avoid same-value writes to EEPROM
	byte four = (value & 0xFF);
	byte three = ((value >> 8) & 0xFF);
	byte two = ((value >> 16) & 0xFF);
	byte one = ((value >> 24) & 0xFF);
	EEPROM.write(address, four);
	EEPROM.write(address + 1, three);
	EEPROM.write(address + 2, two);
	EEPROM.write(address + 3, one);
}



///////////////////////////////////////////////
// state machinery
///////////////////////////////////////////////
#define STATE_DAZZLED	0
#define STATE_EXPLORING	1
#define STATE_ON_LINE	2
#define STATE_OFF_LINE	3
#define STATE_REVERSING	4

uint16_t gc = 0;  // global counter
int8_t state = STATE_EXPLORING;
long last_state_change = 0, now = 0, state_age = 0, delta = 0;
float steer = 0;


///////////////////////////////////////////////
// Class for Servo with calibration in eeprom
///////////////////////////////////////////////
class Drive2Wd
{
public:
	Servo		motor_left, motor_right;

	uint16_t	null_left, null_right;
	uint16_t	should_left, should_right;

	Drive2Wd(){ /* empty constructor, we'll start with begin(), stack/heap/instance thing on MCU */};
	void begin(uint8_t pin_left, uint8_t pin_right){
		get_calibration();
		motor_left.attach(pin_left);					motor_right.attach(pin_right);
		motor_left.writeMicroseconds(null_left);		motor_right.writeMicroseconds(null_right);
	};

	/* exact 0-value of servo is stored in eeprom */
	void get_calibration(){
		null_left = EEPROMReadlong(986);				null_right = EEPROMReadlong(996);
	};

	void set_calibration(){
		null_left = motor_left.readMicroseconds(); 		null_right=motor_right.readMicroseconds();
		EEPROMWritelong(986, null_left);				EEPROMWritelong(996, null_right);
	};

	int calc_servo_timing(int value, int calib) {
		if (value == 0) return calib;
		if (value > 0) return map(value, 0, 512, calib, MAX_PULSE_WIDTH);
		if (value < 0) return map(value, -512, 0, MIN_PULSE_WIDTH, calib);
	};

	void drive(int velo, int steering, int max_velo){
		int left = float(map(steering, -512, 512, -max_velo, max_velo));
	};


	void write(int left, int right){
		should_left = calc_servo_timing(-left, null_left);		should_right = calc_servo_timing(right, null_right);
		if (left == 0) {
			motor_left.writeMicroseconds(null_left);
			should_left = null_left;
		}
		if (right == 0) {
			motor_right.writeMicroseconds(null_right);
			should_right = null_right;
		}
		maintain();
	};

	void maintain(){
		int accel = delta;
		int left = motor_left.readMicroseconds();
		int right = motor_right.readMicroseconds();
		if ( left < should_left) { left+=accel; }
		if ( left > should_left) { left-=accel; }
		if ( right < should_right) { right+=accel; }
		if ( right > should_right) { right-=accel; }
		motor_left.writeMicroseconds(left);
		motor_right.writeMicroseconds(right);
	}
};
// instanciate my class - once
Drive2Wd My2Wd;

///////////////////////////////////////////////
// pin assignments
///////////////////////////////////////////////
const int pin_col_left = 2, pin_col_right = 3;		// must map to interrupts
const int pin_servo_left = 4, pin_servo_right = 7;
const int pin_line_sens = A3;
const int pin_pot_left = A1, pin_pot_right = A2;
const int pin_led_left = 10, pin_led_right = 9, pin_led_3rd = 8;

///////////////////////////////////////////////
// sensor value reading
///////////////////////////////////////////////
int s_value, t_value, v_value;
void readSensors(){
	// double-read sensors, increase accuracy
	s_value = analogRead(pin_line_sens); 			s_value = analogRead(pin_line_sens);
	t_value = analogRead(pin_pot_left); 			t_value = 1023 - analogRead(pin_pot_left);
	v_value = analogRead(pin_pot_right); 			v_value = 1023 - analogRead(pin_pot_right);  // invert
}

///////////////////////////////////////////////
// interrupt handling
///////////////////////////////////////////////
bool col_left = false, col_right = false;
void interrupt_left(){ col_left = true; }
void interrupt_right(){ col_right = true; }


void reportStatus(){
	Serial.flush();
//	Serial.write(0xFE); Serial.write(0x51); //LCD: clear screen
	Serial.write(0xFE); Serial.write(0x45); Serial.write(0x00); //LCD: 1st line
	Serial << F("state/a/d: ") << state << '/';
	if (state_age < 1000) Serial.print('0');
	if (state_age < 100) Serial.print('0');
	if (state_age < 10) Serial.print('0');
	Serial << state_age << '/' <<  delta;
	Serial.write(0xFE); Serial.write(0x45); Serial.write(0x40); //LCD: 2nd line
	Serial << delta << F("mot: ") << My2Wd.motor_left.readMicroseconds() << '/' << My2Wd.motor_right.readMicroseconds() << '/'<< steer;
	Serial.write(0xFE); Serial.write(0x45); Serial.write(0x14); //LCD: 3rd line
	Serial << F("s/t/v: ") << s_value << '/' << t_value << '/' << v_value;
	Serial.write(0xFE); Serial.write(0x45); Serial.write(0x54); //LCD: 4th line

//	Serial << F("Status/age/FreeMem/delta | Servo left/right/steer | Sensor s/t/v: ") << state << '/' << state_age << '/' << freeMemory() << '/' << delta << F(" | ") << My2Wd.motor_left.readMicroseconds() << '/' << My2Wd.motor_right.readMicroseconds() << '/'<< steer << F(" | ") << s_value << '/' << t_value << '/' << v_value << '\n';



}

void set_new_state(uint8_t new_state) {
	reportStatus();
//	Serial << F("State change ") << state << F(" -> ") << new_state << F(" after ") << millis() - last_state_change << '\n';
	Serial.write(0xFE); Serial.write(0x51); //LCD: clear screen

	Serial.write(0xFE); Serial.write(0x45); Serial.write(0x54); //LCD: 4th line
	Serial << F("CHANGE ") << state << F(" -> ") << new_state << F(" ") << millis() - last_state_change;
	state=new_state, last_state_change=millis();
	digitalWrite(pin_led_left,	LOW);
	digitalWrite(pin_led_right, LOW);
}


///////////////////////////////////////////////
// Arduino: setup
///////////////////////////////////////////////
void setup(){
	Serial.begin(9600);  // https://www.newhavendisplay.com/specs/NHD-0420D3Z-FL-GBW.pdf
//	Serial.write(0xFE); Serial.write(0x61); Serial.write(0x08) ;	//LCD: set 115200 baud
//	Serial.begin(115200);
//		Serial.write(0xFE); Serial.write(0x61); Serial.write(0x04) ;	//LCD: set 9600 baud

	Serial.write(0xFE); Serial.write(0x51); Serial.write(0x13);//LCD: clear screen
	Serial.print(F("left motors, right sensors"));
	// setup arduino pins
	pinMode(pin_col_left, INPUT); pinMode(pin_col_left, INPUT);
	pinMode(pin_led_left, OUTPUT); pinMode(pin_led_right, OUTPUT); pinMode(pin_led_3rd, OUTPUT);

	// just sensor readings
	if ( digitalRead(pin_col_right) ) {
		digitalWrite(pin_led_right, true);
		while ( digitalRead(pin_col_right) ) {}  // wait for release
		delay(200);
		while ( ! digitalRead(pin_col_right) ) {
			readSensors(); reportStatus();
			analogWrite(pin_led_left, s_value/4 );
			analogWrite(pin_led_right, t_value/4 );
			digitalWrite(pin_led_3rd, (s_value>=t_value));
			delay(25);
		}
	}

	// start wheels
	My2Wd.begin(pin_servo_left, pin_servo_right);
	My2Wd.write(0,0);

	// calibrate
	if ( digitalRead(pin_col_left) ) {
		digitalWrite(pin_led_left, true);
		while ( digitalRead(pin_col_left) ) {}  // wait for release
		delay(200);
		while ( ! digitalRead(pin_col_left) ) { // until sensor touched another time
			readSensors(); reportStatus();
			int v_left = v_value - 512; int v_right = t_value - 512;
			My2Wd.write(v_left, v_right);
			digitalWrite(pin_led_3rd, !digitalRead(pin_led_3rd));
			digitalWrite(pin_led_left, !digitalRead(pin_led_left));
			digitalWrite(pin_led_right, !digitalRead(pin_led_right));
			delay(25);
		}
		My2Wd.set_calibration();
	}


	// attaching interrupts
	attachInterrupt(pin_col_left - 2, interrupt_left, RISING);
	attachInterrupt(pin_col_right - 2, interrupt_right, RISING);

	reportStatus();
}
///////////////////////////////////////////////
// Arduino: loop
///////////////////////////////////////////////
bool above_threshold; uint16_t readings;
void loop(){
	// do some timing
	gc++;
	delta = 5 -(millis() - now);   // sync to 10millis
	if (delta > 0) delay( delta ); else delta = 0;
	now = millis();

	// was there an collision?
	if (col_left or col_right) {
		My2Wd.write(0, 0);
		set_new_state(STATE_REVERSING);
		col_left = false; col_right = false;
	}

	// go on with my daily business
	readSensors();
	v_value /= 8;   // only use positive 0-127

	readings += s_value;
	if (gc%4 == 0) {
		above_threshold = readings >= t_value * 4;
		readings = 0;
	}

	digitalWrite(pin_led_3rd, (above_threshold));



	// execute state-machine
	state_age = now - last_state_change;
	steer = (state_age / 333.0);
	if (steer > 1.0) steer = 1.0;

	if (gc%25==0){
		reportStatus();
	}
	switch (state) {
	case STATE_DAZZLED:
		My2Wd.write(0,0);
		digitalWrite(pin_led_3rd, random(2));
		digitalWrite(pin_led_left, random(2));
		digitalWrite(pin_led_right, random(2));
		if ( state_age > 500){
			set_new_state(STATE_EXPLORING);
		}
		break;
	case STATE_EXPLORING:
		digitalWrite(pin_led_left,!(gc%4));
		if (above_threshold) {
			My2Wd.write(0,0);
			set_new_state(STATE_ON_LINE);
		}
		if (state_age > 1000){ // reset to my own state
			set_new_state(STATE_EXPLORING);
			My2Wd.write(random(-v_value,v_value*2), random(-v_value,v_value*2));
		}
		break;
	case STATE_ON_LINE:
		digitalWrite(pin_led_right,!(gc%4));
		My2Wd.write(v_value , float(v_value*(1-steer)));
		if ( ! above_threshold and state_age > 10) { // line lost 10ms ago
			set_new_state(STATE_OFF_LINE);
		}
		break;
	case STATE_OFF_LINE:
		digitalWrite(pin_led_right,!(gc%4));
		My2Wd.write(float(v_value * (1-steer)), v_value);
		if ( above_threshold and state_age > 100) { // line found 10ms ago
			set_new_state(STATE_ON_LINE);
		}
		if ( state_age > 3500 ){
			My2Wd.write(0, 0);
			set_new_state(STATE_REVERSING);
		}
		break;
	case STATE_REVERSING:
		digitalWrite(pin_led_left, (now%200 >= 100));
		digitalWrite(pin_led_right, (now%200 <= 100));
		if (state_age > 250 ) {
			My2Wd.write(-v_value*2,-v_value*2);
			if (above_threshold or state_age > 1500) { set_new_state(STATE_DAZZLED);}
		}
		break;
	default:
		Serial.println("unknown state");
	}
	My2Wd.maintain();
	/* That's all Folks */
}
