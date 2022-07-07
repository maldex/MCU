#include "Arduino.h"

void isr1() { Serial.print("2:"); Serial.println(digitalRead(2)); }
void isr2() { Serial.print("3:"); Serial.println(digitalRead(2)); }

void setup()
{
	Serial.begin(115200);
	attachInterrupt(digitalPinToInterrupt(2), isr1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(3), isr2, CHANGE);

}

// The loop function is called in an endless loop
void loop()
{
	Serial.println("...");
	delay(1000);
}
