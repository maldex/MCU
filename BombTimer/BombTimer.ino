#include "Arduino.h"
#include "EEPROM.h"
#include "Adafruit_LEDBackpack.h"




#define btn_tl A1  // button top left (1)
#define btn_tr A0  // button top right (3)
#define btn_bl A3  // button bottom left (*)
#define btn_br A2  // button bottom right (#)
#define led 13     // onboard led at pin 13

#define value_addr 0xFA  // EEPROM address of value
#define brigt_addr 0xFB  // EEPROM address of value

enum status {
	idle = 10,
	precount = 20,
	counting = 30,
	prefinished = 40,
	finished = 50,
};

Adafruit_7segment Display = Adafruit_7segment();
status state = idle;

void setTimerValue(){
	Serial.println("set timer - press reset to finish");
	uint8_t value = EEPROM.read(value_addr);
	Display.blinkRate(1);
	while (true) {  // loop forever
		if (digitalRead(btn_tr)) value++;
		if (digitalRead(btn_tl)) value--;
		if (value > 100) value = 100;
		Display.print((int)value); Display.writeDisplay();
		delay(100);
		EEPROM.write(value_addr, value);
	}
}

void setBrightness(){
	Serial.println("set brightness - press reset to finish");
	uint8_t value = EEPROM.read(brigt_addr);
	Display.blinkRate(1);
	while (true) {  // loop forever
		if (digitalRead(btn_tr)) value++;
		if (digitalRead(btn_tl)) value--;
		if (value > 16) value = 16;
		Display.setBrightness(value);
		Display.print((int)value); Display.writeDisplay();
		delay(100);
		EEPROM.write(brigt_addr, value);
	}
}


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
		if (value<1000) Display.writeDigitNum(0, 0);
		if (value<100) Display.writeDigitNum(1, 0);
		if (value<10) Display.writeDigitNum(2, 0);
		break;
	}
	Display.drawColon(colon);
	Display.writeDisplay();
}


int gobal_counter = 0;
unsigned long last_statechange = 0;

void setup()
{
	Serial.begin(9600); Serial.println("7 Segment BombTimer");

	pinMode(btn_tl, INPUT);
	pinMode(btn_tr, INPUT);
	pinMode(btn_bl, INPUT);
	pinMode(btn_br, INPUT);
	pinMode(led, OUTPUT);

	Display.begin(0x73);
	Display.setBrightness(EEPROM.read(brigt_addr));

	if (digitalRead(btn_br)) {
		setTimerValue();
	}
	if (digitalRead(btn_bl)) {
		setBrightness();
	}

	last_statechange = millis();
}




void loop()
{
	// gather facts
	bool keypress_tl = digitalRead(btn_tl);
	bool keypress_tr = digitalRead(btn_tr);
	bool keypress_bl = digitalRead(btn_bl);
	bool keypress_br = digitalRead(btn_br);

	unsigned long now = millis();
	unsigned long state_age = (now - last_statechange);

	gobal_counter++;

	// little bit of reporting
	Serial.println();
	Serial.print(keypress_tl);Serial.print(keypress_tr);Serial.print(keypress_bl);Serial.print(keypress_br);
	Serial.print(" "); Serial.print(state);
	Serial.print(" "); Serial.print(state_age);

	// enter the state-machinery
	switch (state){
	case idle:
		digitalWrite(led, false);
		displayPrint(INT16_MAX); // just dashes
		if (keypress_tl) {
			last_statechange = now;
			state = precount;
		}
		break;

	case precount:
		displayPrint(EEPROM.read(value_addr)*100);
		if (state_age > 1000){
			last_statechange = now;
			state = counting;
		}
		break;

	case counting:
		signed long remaining = 3 * 1000 - state_age ;
		displayPrint(remaining/10);
		Serial.print(" "); Serial.print(remaining);
		if (remaining <= 0){
			last_statechange = now;
			state = finished;
		}
		break;

	case prefinished:
		Serial.println("PREFINISHED");
		break;



	case 40:
		delay(1000);

		Serial.print("PREFIN"); Serial.print(gobal_counter);
		if (gobal_counter%2){ displayPrint(0); }
		else { displayPrint(INT16_MIN, false); }
//
//		if (state_age > 3000) {
//			last_statechange = now;
//			state = finished;
//		}

		break;

	case finished:
		displayPrint(0);
		digitalWrite(led, true);

		if (digitalRead(btn_bl)) {state = idle;}
		break;



	default: break;  // should never happen
	}

	delay(10);
}
