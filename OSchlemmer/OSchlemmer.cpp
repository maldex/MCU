#include "Arduino.h"
#include "Servo.h"
#include "EEPROM.h"
/*
 */


class CustomServo: public Servo {
private:
	unsigned long lastmillis;
public:
	int pin;
	void maintain();
	void turnOn(uint8_t v);
	void turnOff();
};

void CustomServo::maintain() {
	if ( millis() - lastmillis > 1000 ){
		this->detach();
	}
}

void CustomServo::turnOn(uint8_t v) {
	Serial.println("on");
	this->attach(pin);
	this->write(v);
	lastmillis = millis();
}

void CustomServo::turnOff() {
	 Serial.println("on");
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
	for (int i=0; i++; i < sizeof(ServoPins)){
		ServoObj[i].pin = ServoPins[i];
	}
}

void loop() {
	analogWrite(led1, random(64,255));
	for (int i=0; i++; i < sizeof(ServoPins)){
		ServoObj[i].turnOn(analogRead(poti1)* 180.0 / 1024.0 );
		ServoObj[i].maintain();
	}
}
