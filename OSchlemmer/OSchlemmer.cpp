#include "Arduino.h"
/*
 */


/* ------------------------------------------------ interrupt code: handler for ALL Arduino pins */
/* ### Interrupt handler: when setup through pciSetup(), ISR-function will be called ### */
char last_interrupt_port = false;		// 'lockobject': this will be altered when interrupt fired
void pciSetup(byte pin) {
	*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
	PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
	PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
ISR (PCINT0_vect){ last_interrupt_port = 'A'; } // handle pin change interrupt for D8 to D13 here
ISR (PCINT1_vect){ last_interrupt_port = 'B'; } // handle pin change interrupt for A0 to A5 here
ISR (PCINT2_vect){ last_interrupt_port = 'C'; }	// handle pin change interrupt for D0 to D7 here
/* ### end interrupt handler and callback routines ### */


/* ------------------------------------------------------------ here we start */
/* pins used in this program */
const int button1 =	A0;
const int poti1 =	A1;
const int led1 = 	11;

/* finally: the arduino style main functions */
void setup() {
	// setup pinmodes
	pinMode(button1, INPUT);
	pinMode(poti1, INPUT);
	pinMode(led1, OUTPUT);

	pciSetup(button1);		// interrupt: register pin to go to ISR()-Method when raising/falling

	Serial.begin(115200);
	Serial.println("hi");
}

void loop() {
	if (last_interrupt_port) {				// was there a interrupt while i was sleeping?
		if (not digitalRead(button1)) {  	// was the interrupt upon a release of button1?
			Serial.println("jopo");
		}
	}

	Serial.println("jooo");

	// now delay according the the poti1 setting
	delay(analogRead(poti1) / 50 );
}
