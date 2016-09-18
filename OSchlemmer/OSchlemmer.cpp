#include "Arduino.h"
#include "Servo.h"
#include "EEPROM.h"
/* https://github.com/maldex/MCU/tree/master/OSchlemmer
 */


class CustomServo: public Servo {
private:
	uint8_t low;
	uint8_t high;
	uint8_t speed;
public:
	int pin;
	void getstarted();
	void maintain();
	void calibrate(uint8_t analog_pin);
	void enable(uint8_t v);
	void setState();
	void setState(bool state);
};

void CustomServo::getstarted() {
	low = EEPROM.read(pin * 10);
	high = EEPROM.read(pin * 10 + 1);
	speed = EEPROM.read(pin * 10 + 2);
}

void CustomServo::maintain() {
//
}

void CustomServo::enable(uint8_t v) {
	this->attach(pin);
	this->write(v);
//	lastmillis = millis();
}

void CustomServo::setState(bool state) {
	if (state) {
		this->enable(high);
	}else{
		this->enable(low);
	}
}

void CustomServo::calibrate(uint8_t analog_pin){
	Serial.print("'l'ow or 'h'igh value, adjust via poti on pin "); Serial.println(analog_pin);
	this->attach(pin);
	int lvalue;
	while (true){
		delay(10);
		int nvalue = analogRead(analog_pin) * 180.0 / 1023.0;;
		uint8_t value = nvalue; //
		if (lvalue != nvalue ){
			this->write(value);
			Serial.print("Servo on PIN "); Serial.print(pin); Serial.print(" - "); Serial.println(value);
			lvalue = nvalue;
		}

		if (Serial.available()) {
			char c = Serial.read();
			if (c == 'l'){
				EEPROM.write(pin * 10, value);
				Serial.print("l written to ");Serial.println(pin * 10);
			}
			else if (c == 'h'){
				EEPROM.write(pin * 10 + 1, value);
				Serial.print("h written to ");Serial.println(pin * 10 + 1);
			}
			else if ( c == 's'){
				Serial.println("enter speed in 0-255: ");
				uint8_t speed = Serial.parseInt();
				EEPROM.write(pin * 10 + 2, speed);
				Serial.print(speed);Serial.print("s written to ");Serial.println(pin * 10 + 2);
			}
		}
	}
}


/* ------------------------------------------------------------ here we start */




/* pins used in this program */
const int button1 =	A5;
const int poti1 =	A4;
const int poti2 =	A3;
const int led1 =	9;
const int led2 =	10;
const int led3 = 	11;


unsigned long lmillis;

void myirq(){
	if ( millis() - lmillis > 100 ){
		lmillis = millis();
		digitalWrite(led1,!digitalRead(led1));
	}
}

uint8_t ServoPins[4] = { 5,7,6,4 };
CustomServo ServoObj[4];
/* finally: the arduino style main functions */
void setup() {
	// setup pinmodes
	pinMode(button1, INPUT);
	pinMode(poti1, INPUT);
	pinMode(poti2, INPUT);
	pinMode(led3, OUTPUT);
	pinMode(led2, OUTPUT);
	pinMode(led1, OUTPUT);

	Serial.begin(115200);
	Serial.println("hi");
	for (int i=0; i < sizeof(ServoPins); i++){
		ServoObj[i].pin = ServoPins[i];
		ServoObj[i].getstarted();
	}

	if (digitalRead(button1)){
			Serial.println("Calibration: enter number of Servo: ");
			while (! Serial.available()) delay(10);
			uint8_t n = Serial.read();
			Serial.flush(); // empty remaining buffer
			n = n - 48;
			if (n > sizeof(ServoPins)) {
				Serial.print("unknown servo "); Serial.println(n);
			} else {
				ServoObj[n].calibrate(poti1);
			}
	}
	Serial.println("starting");

}

bool state = false;
void loop() {
	if (state) {
		analogWrite(led3, random(0, analogRead(poti1) ));
		delay(random(0,10));
	}

	if (digitalRead(button1)){
		state = (state == false);
		analogWrite(led3,0);
		while(digitalRead(button1)) delay(10); // cheap debounce
		Serial.print("button pressed ");Serial.println(state);
		ServoObj[0].setState(state);
		digitalWrite(led2, state);
//		for (int i=0; i < sizeof(ServoPins); i++){
//			ServoObj[i].setState(state);
//		}
	}
	myirq();

	uint8_t sv = analogRead(poti2) *  180.0 / 1023.0 ;

	ServoObj[1].enable( sv );

//	for (int i=0; i++; i < sizeof(ServoPins)){
//		ServoObj[i].turnOn(analogRead(poti1)* 180.0 / 1024.0 );
//		ServoObj[i].maintain();
//	}
}
