#include "Arduino.h"
/* oke, not nice:
according to https://www.tindie.com/products/Conceptinetics/dmx-shield-for-arduino-rdm-capable/
are Arduino Pin 0&1 OR 3&4 for RX/TX, Pin 2 for Master/slave anyway. so you end-up with config:
0/1 for RX/TX(jumpers on xX-UART): you loose the usage of Serial.print-functions
3/4 for RX/TX(jumpers on xX-IO): you loose the usage of interrupt driven pin-handling
                                 (attachInterrup(), available only on PIN 2/3)

-> detour: using new-style port based interrupt handler on Port 2 (A0-A5)

Architecture:
setup():
 - registering interrupt handler for button
loop()
 - checks it interrupt was fired, if yes, negative/postivie flip delta variable
 - recalculate: value + delta:   if delta is negative, value will be decreased
 - finally delay by potentiometer setting
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
const int button =	A0;
const int poti =	A1;
const int led = 	11;

/* Luminescence values */
int intensity_delta = -1;			// how much to add/remove per interval
int intensity = 0;  				// the intensity the light shall shine
int last_intensity = intensity;
const int min_intensity = 0;		// minimum
const int max_intensity = 255;		// maximum

/* finally: the arduino style main functions */
void setup() {
	// setup pinmodes
	pinMode(button, INPUT);
	pinMode(poti, INPUT);
	pinMode(led, OUTPUT);

	pciSetup(button);		// interrupt: register pin to go to ISR()-Method when raising/falling

	Serial.begin(115200); Serial.println("hi - remove this Serial.xxx() lines for the dmx stuff on pin 0/1");
}

void loop() {
	if (last_interrupt_port) {				// was there a interrupt while i was sleeping?
		if (not digitalRead(button)) {  	// was the interrupt upon a release of button?
			last_interrupt_port = false;	// reset button-trigger
			intensity_delta = -intensity_delta;	// flip from positive to negative
			last_intensity = -1;			// trigger recalculation
		}
	}

	if (intensity != last_intensity) {		// did the intensity change?
		last_intensity = intensity;			// save last value and recalculate fading

		intensity += intensity_delta;   // add/remove from intensity
		if (intensity <= min_intensity)	{ intensity = min_intensity; }
		if (intensity >= max_intensity) { intensity = max_intensity; }

		// ---- set the value to whereever it goes here
		analogWrite(led, intensity);
		Serial.println(intensity);  // info: serial-print takes time ....
		// ----
	}

	// now delay according the the poti setting
	delay(analogRead(poti) / 50 );
}
