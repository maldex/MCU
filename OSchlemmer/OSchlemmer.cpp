#include "Arduino.h"
#include "Servo.h"
#include "EEPROM.h"
/*
 */


class CustomServo: public Servo {
private:
	uint8_t low;
	uint8_t high;
public:
	int pin;
	void readEeprom();
	void maintain();
	void calibrate(uint8_t analog_pin);
	void turnOn(uint8_t v);
	void turnOff();
};

void CustomServo::readEeprom() {
	low = EEPROM.read(pin * 10);
	high = EEPROM.read(pin * 10 + 1);
}

void CustomServo::maintain() {
//
}

void CustomServo::turnOn(uint8_t v) {
	Serial.println("on");
	this->attach(pin);
	this->write(v);
//	lastmillis = millis();
}

void CustomServo::turnOff() {
	 Serial.println("on");
}

void CustomServo::calibrate(uint8_t analog_pin){
	Serial.print("'l'ow or 'h'igh value, adjust via poti on pin "); Serial.println(analog_pin);
	this->attach(pin);
	while (true){
		uint8_t value = analogRead(analog_pin) * 180.0 / 1023.0;
		Serial.print("Servo on PIN "); Serial.print(pin); Serial.print(" - "); Serial.println(value);
		delay(100);

		this->write(value);
		if (Serial.available()) {
			char c = Serial.read();
			if (c == 'l')
				EEPROM.write(pin * 10, value);
			if (c == 'h')
				EEPROM.write(pin * 10 + 1, value);
		}
	}
}

/* ------------------------------------------------------------ here we start */


/* pins used in this program */
const int button1 =	A0;
const int poti1 =	A1;
const int led1 = 	11;

uint8_t ServoPins[4] = { 2,3,4,5 };
CustomServo ServoObj[4];
/* finally: the arduino style main functions */
void setup() {
	// setup pinmodes
	pinMode(button1, INPUT);
	pinMode(poti1, INPUT);
	pinMode(led1, OUTPUT);

	Serial.begin(115200);
	Serial.println("hi");
	for (int i=0; i < sizeof(ServoPins); i++){
		ServoObj[i].pin = ServoPins[i];
		ServoObj[i].readEeprom();
	}

	if (digitalRead(button1)){
			Serial.println("Calibration: enter number of Servo: ");
			while (! Serial.available()) delay(10);
			uint8_t n = Serial.read();
			while (Serial.available()) Serial.read(); // empty remaining buffer
			n = n - 48;
			if (n > sizeof(ServoPins)) {
				Serial.print("unknown servo "); Serial.println(n);
			} else {
				ServoObj[n].calibrate(poti1);
			}
	}
	Serial.println("starting");

}

void loop() {
	analogWrite(led1, random(64,255));
//	for (int i=0; i++; i < sizeof(ServoPins)){
//		ServoObj[i].turnOn(analogRead(poti1)* 180.0 / 1024.0 );
//		ServoObj[i].maintain();
//	}
}
