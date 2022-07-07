#include "Arduino.h"

// Sinmple example how to communicate between arduino and python

#define btn 2
#define led 13

void isr1() { Serial.print("2:"); Serial.println(digitalRead(btn)); }

void setup()
{
	Serial.begin(115200);
	attachInterrupt(digitalPinToInterrupt(btn), isr1, CHANGE);
	pinMode(led, OUTPUT);
}

// The loop function is called in an endless loop
void loop()
{
	Serial.println("...");
	delay(1000);

	if (Serial.available()){
		String line = Serial.readStringUntil('\n');
		line.trim();
		if(line == "LED True") { digitalWrite(led, HIGH); }
		if(line == "LED False") { digitalWrite(led, LOW); }
	}
}
