#include "Arduino.h"
#include <Conceptinetics.h>
#include <Rdm_Defines.h>
#include <Rdm_Uid.h>

/* Conceptinetics */
#define DMX_MASTER_CHANNELS   5 
#define RXEN_PIN              2
#define LIGHTCHAN             5

DMX_Master        dmx_master ( DMX_MASTER_CHANNELS, RXEN_PIN );
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
volatile char last_interrupt_port = false;       // 'lockobject': this will be altered when interrupt fired
void pciSetup(byte pin) {
      *digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin));  // enable pin
      PCIFR  |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
      PCICR  |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
}
ISR (PCINT0_vect){ last_interrupt_port = 'A'; } // handle pin change interrupt for D8 to D13 here Not Used here
ISR (PCINT1_vect){ last_interrupt_port = 'B'; } // handle pin change interrupt for A0 to A5 here Not Used here
ISR (PCINT2_vect){ last_interrupt_port = true; } // handle pin change interrupt for D0 to D7 here
/* ### end interrupt handler and callback routines ### */
 
 
/* ------------------------------------------------------------ here we start */
/* pins used in this program */
const int button = 7;
const int notAus = 8;
const int poti =  A1;

const int ledBlue =   9;
const int ledRed =   10;
const int ledGreen = 11;

 
/* Luminescence values */ 
int intensity_delta =        -1; // how much to add/remove per interval
int intensity =               0; // the intensity the light shall shine
int last_intensity =  intensity;
const int min_intensity =     0; // minimum
const int max_intensity =   255; // maximum
const int cycleDivFactor =    7; // Dividing factor for wait for the cycle time : for faster or slower fading
const int minCycleWaitTime = 50; // Minimum waittime for the cycle don't touch them


/* when arduino has been restarted */
char restartFlag = false;

 
/* finally: the arduino style main functions */
void setup() {
  // Setup DMX and initialize the headlights off
  dmx_master.enable ();
  dmx_master.setChannelRange ( LIGHTCHAN, LIGHTCHAN, min_intensity ); // Set channel n - n @ 0%
  // setup pinmodes
  pinMode(button, INPUT);    
  pinMode(notAus, INPUT);
  pinMode(poti, INPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
 
  pciSetup(button);       // interrupt: register pin to go to ISR()-Method when raising/falling

  // All LED on when Startup
  digitalWrite(ledBlue, HIGH);
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, HIGH); 
  delay(1000);
  digitalWrite(ledBlue, LOW);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW); 

  restartFlag = true;
}
 
void loop() {

  /* This is a Feature if the Arduino has been restated and the light was fully on 
   * then is possible hit both keys for a fast switch on of the lights
   */
  if (digitalRead(notAus) and digitalRead(button) and restartFlag) {
    intensity = max_intensity;
    last_intensity = intensity;
    dmx_master.setChannelValue ( LIGHTCHAN, intensity );   
    digitalWrite(ledBlue, HIGH);
    digitalWrite(ledRed, HIGH);
    digitalWrite(ledGreen, HIGH); 
    delay(1000);
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW); 
  }

  // Not Aus if fadeing in, but just possible whe the light is not off  
  if (digitalRead(notAus) and intensity != min_intensity) { 
    last_interrupt_port = false;
    intensity = min_intensity;
    last_intensity = intensity;
    dmx_master.setChannelValue ( LIGHTCHAN, intensity ); 
    digitalWrite(ledRed, HIGH);
    delay(1000);
    digitalWrite(ledRed, LOW);         
  }
      
  if (last_interrupt_port) {              // was there a interrupt while i was sleeping?
    if (not digitalRead(button)) {        // was the interrupt upon a release of button?
      last_interrupt_port = false;        // reset button-trigger
      intensity_delta = -intensity_delta; // flip from positive to negative
      last_intensity = -1;                // trigger recalculation
    }
  }

  // This part set and swith the intensity for the lights
  if (intensity != last_intensity) {      // did the intensity change?
    restartFlag = false;
    last_intensity = intensity;           // save last value and recalculate fading
 
    intensity += intensity_delta;         // add/remove from intensity
    if (intensity <= min_intensity) { intensity = min_intensity; }
    if (intensity >= max_intensity) { intensity = max_intensity; }
 
    if (intensity_delta < 0) {
      digitalWrite(ledBlue, HIGH);
      digitalWrite(ledGreen, LOW);              
    } else {
      digitalWrite(ledBlue, LOW);
      digitalWrite(ledGreen, HIGH);                            
    }
            
    dmx_master.setChannelValue ( LIGHTCHAN, intensity ); 
  } else {
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledGreen, LOW);                      
  }

 
  // now delay according the the poti setting
  int waitTime = analogRead(poti) / cycleDivFactor;
  if ( waitTime <= minCycleWaitTime ) { waitTime = minCycleWaitTime; }
  delay( waitTime );
}


