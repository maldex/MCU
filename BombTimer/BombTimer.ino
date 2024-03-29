#include "Arduino.h"
#include "EEPROM.h"
#include "Adafruit_LEDBackpack.h"  /* Adafruit LED Backpack Library */

// input and output ports
#define b_tl A1  // button top left (1)
#define b_tr A0  // button top right (3)
#define b_bl A3  // button bottom left (*)
#define b_br A2  // button bottom right (#)
#define led 13   // onboard led at pin 13

// eeprom addresses of certain values
#define value_addr 0xFA  // EEPROM address of value
#define brigt_addr 0xFB  // EEPROM address of brigtness
#define facto_addr 0xFC  // EEPROM address of factor

// possible states for the finite state machine
enum status {
	idle = 10,
	postidle = 20,
	precount = 30,
	counting = 40,
	prefinished = 50,
	finished = 60,
};
status state = idle;

// global runtime variables
unsigned int global_counter = 0;
unsigned long last_statechange = 0;
unsigned long now, state_age;
signed long remaining;

// Instantiate the display itself
Adafruit_7segment Display = Adafruit_7segment();


// function to display values in a nice way
void displayPrint(int value, bool colon = true){
	switch (value) {
	case INT16_MAX:
		// just print dashes
		for (int digit = 0; digit < 5; digit++) { Display.writeDigitRaw(digit, 0x40); }
		break;
	case INT16_MIN:
		// clear the display
		for (int digit = 0; digit < 5; digit++) { Display.writeDigitRaw(digit, 0x00); }
		break;
	default:

		// print number leading zero
		Display.print(value);
		if (value<1000) { Display.writeDigitNum(0, 0); }
		if (value<100) { Display.writeDigitNum(1, 0); }
		if (value<10) { Display.writeDigitNum(3, 0); }
		break;
	}
	Display.drawColon(colon);
	Display.writeDisplay();
}

// configure function to set total time
void setTimerValue(){
	Serial.println("set timer - press reset to finish");
	uint8_t value = EEPROM.read(value_addr);
	Display.blinkRate(1);
	while (true) {  // loop forever
		if (digitalRead(b_tr)) value++;
		if (digitalRead(b_tl)) value--;
		if (value > 100) value = 100;
		Display.print((int)value); Display.writeDisplay();
		EEPROM.write(value_addr, value);
		delay(100);
	}
}

// configure function to set brigtness
void setBrightness(){
	Serial.println("set brightness - press reset to finish");
	uint8_t value = EEPROM.read(brigt_addr);
	Display.blinkRate(1);
	while (true) {  // loop forever
		if (digitalRead(b_tr)) value++;
		if (digitalRead(b_tl)) value--;
		if (value > 16) value = 16;
		Display.setBrightness(value);
		Display.print((int)value); Display.writeDisplay();
		EEPROM.write(brigt_addr, value);
		delay(100);
	}
}

// configure function to set accuracy
void setFactor(){
	Serial.println("set factor - press reset to finish");
	uint8_t value = EEPROM.read(facto_addr);
	Display.drawColon(true);
	Display.blinkRate(1);
	while (true) {  // loop forever
		if (digitalRead(b_tr)) value=1;
		if (digitalRead(b_tl)) value=100;
		displayPrint((int)value, true);
		EEPROM.write(facto_addr, value);
		delay(100);
	}
}

void setup()  /* ARDUINO NATIVE FUNCTION: RUN ONCE AFTER POWERON */
{
	Serial.begin(9600); Serial.println("7 Segment BombTimer");

	pinMode(b_tl, INPUT); pinMode(b_tr, INPUT); pinMode(b_bl, INPUT); pinMode(b_br, INPUT);
	pinMode(led, OUTPUT);

	Display.begin(0x73);
	Display.setBrightness(EEPROM.read(brigt_addr));

	// catch configure functions first if according pressed
	if (digitalRead(b_bl)) {
		setTimerValue();
	}
	if (digitalRead(b_br)) {
		setBrightness();
	}

	if (digitalRead(b_tr)) {
		setFactor();
	}

}


void loop()  /* ARDUINO NATIVE FUNCTION: RUN REPEATEDLY */
{
	// gather facts
	bool keypress_tl = digitalRead(b_tl);
	bool keypress_tr = digitalRead(b_tr);
	bool keypress_bl = digitalRead(b_bl);
	bool keypress_br = digitalRead(b_br);

	now = millis();
	state_age = (now - last_statechange);

	global_counter++;

	// little bit of reporting
	Serial.println();
	Serial.print(keypress_tl);Serial.print(keypress_tr);Serial.print(keypress_bl);Serial.print(keypress_br);
	Serial.print(" "); Serial.print(state);
	Serial.print(" "); Serial.print(state_age);

	// enter the state-machinery
	switch(state){
	case idle:
		Serial.print(" idle");
		digitalWrite(led, false);
		displayPrint(INT16_MAX); // just dashes
		if (keypress_tl) {
			last_statechange = now; state = postidle;
		}
		break;

	case postidle:
		Serial.print(" postidle");

		if (state_age > 1700) {
			last_statechange = now; state = precount;
		}
		break;

	case precount:
		Serial.print(" precount");
		displayPrint(EEPROM.read(value_addr)*100/EEPROM.read(facto_addr));
		if (keypress_tr or state_age > 1000) {
			last_statechange = now; state = counting;
		}
		break;

	case counting:
		Serial.print(" counting");
		remaining = long(EEPROM.read(value_addr)) * 1000 - state_age ;
		displayPrint((long)remaining/10/EEPROM.read(facto_addr), (remaining/500)%2);
		Serial.print(" "); Serial.print(remaining);
		if (remaining <= 0) {
			last_statechange = now; state = prefinished;
		}
		if (keypress_tl) last_statechange = now;
		if (keypress_br) state = idle;
		break;

	case prefinished:
		Serial.print(" prefinished");
		if (global_counter%2){ displayPrint(0); } // display 00:00
		else { displayPrint(INT16_MIN, false); } // switch off
		if (state_age > 1000) {
			last_statechange = now; state = finished;
		}
		delay(100);
		break;

	case finished:
		Serial.print(" finished");
		digitalWrite(led, true);
		displayPrint(0);
		if (keypress_br) {
			last_statechange = now; state = idle;
		}
		break;

	default:
		Serial.print(" DEFAULT SHOULD NOT HAPPEN");
		while (true) {delay(1000);}
		break;

	}

	// thats it, just wait a little bit for buffers to flush
	delay(10);
}
