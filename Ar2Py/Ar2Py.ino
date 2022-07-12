#include "Arduino.h"

#define btn 2

void isr1() {
	if (digitalRead(btn)) {
		Serial.println("on");
	} else {
		Serial.println("off");
	}
}

void setup()
{
	Serial.begin(115200);
	pinMode(btn, INPUT_PULLUP);
	attachInterrupt(digitalPinToInterrupt(btn), isr1, CHANGE);
}

void loop()
{
	Serial.println("idle");
	delay(3000);
}
