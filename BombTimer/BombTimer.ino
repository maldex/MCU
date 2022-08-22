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
	idle,
	precount,
	counting,
	prefinished,
	finished
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
		if (value<10000) Display.writeDigitNum(0, 0);
		if (value<1000) Display.writeDigitNum(1, 0);
		if (value<100) Display.writeDigitNum(2, 0);
		if (value<10) Display.writeDigitNum(3, 0);
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
	//	if (digitalRead(btn_tl)){Display.print(1111); Display.writeDisplay(); return; }
	//	if (digitalRead(btn_tr)){Display.print(3333); Display.writeDisplay(); return; }
	//	if (digitalRead(btn_bl)){Display.print(7777); Display.writeDisplay(); return; }
	//	if (digitalRead(btn_br)){Display.print(9999); Display.writeDisplay(); return; }


	gobal_counter++;
	unsigned long millisNow = millis();
	float state_age = (millisNow - last_statechange);
	float remaining = EEPROM.read(value_addr) - (millisNow - last_statechange) ;

	switch (state){
	case idle:
		digitalWrite(led, false);
		displayPrint(INT16_MAX); // just dashes
		if (digitalRead(btn_tl)) {
			last_statechange = millisNow;
			state = prefinished;
		}
		break;

	case precount:

		break;

	case counting:
		break;

	case prefinished:
		if (gobal_counter%2){ displayPrint(0); }
		else { displayPrint(INT16_MIN, false); }
		delay(100);

		if ((millisNow - last_statechange) > 3000) {
			last_statechange = millisNow;
			state = finished;
		}

		break;

	case finished:
		displayPrint(0);
		digitalWrite(led, true);

		if (digitalRead(btn_bl)) {state = idle;}
		break;

	default: break;  // should never happen
	}

}
