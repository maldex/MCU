#include "Arduino.h"
#include "Servo.h"

/*
 * Object, Pin and value definitions
 */

// input pins
const int button1 =	A5;
const int poti1 =	A4;
const int poti2 =	A3;

// leds
const uint8_t fakelPin 	= 11;
const uint8_t ledPin[5] = { 2, 3,   8, 9, 10 };  // digital out Pins on Arduino

// servos
const uint8_t servoPin[4] =      {   4,   5,   6,   7 };  // digital out Pins on Arduino
// calibration data
const uint8_t servoCalibMin[4] = {   0,  20,  40,  60 };  // one side
const uint8_t servoCalibMax[4] = { 180, 160, 140, 120 };  // the other side

// arduino servo objects
Servo ServoObj[4];

/*
 * Scene related definitions and data
 */

/* scene related definitions */
struct Scene {   /* definition of scene-data structure */
	long scene_duration;  //in milliseconds
	bool fakel;
	uint8_t servo0;
	uint8_t servo1;
	uint8_t servo2;
	uint8_t servo3;
	uint8_t led0;
	uint8_t led1;
};

Scene scene0 = { /* default power-on scene */
		-1,					// scene_duration
		false,				// fakel
		servoCalibMin[0],	// servo0
		servoCalibMin[1],	// servo1
		servoCalibMin[2],	// servo2
		servoCalibMin[3],	// servo3
		0,					// led0
		0,					// led1
};
Scene scene1 = {
		1000,				// scene_duration
		true,				// fakel
		servoCalibMin[0],	// servo0
		servoCalibMax[1],	// servo1
		servoCalibMax[2],	// servo2
		servoCalibMin[3],	// servo3
		255,				// led0
		255,				// led1
};
Scene scene2 = {
		2500,				// scene_duration
		true,				// fakel
		servoCalibMax[0],	// servo0
		servoCalibMax[1],	// servo1
		servoCalibMin[2],	// servo2
		servoCalibMax[3],	// servo3
		0,					// led0
		128,				// led1
};

unsigned long lastSceneTransition;
volatile uint8_t currentSceneCounter = -1;
Scene currentScene = scene0;

/*
 * Functions
 */
void scene_transition(){
	currentSceneCounter += 1;
	if (currentSceneCounter > 2 )
		currentSceneCounter=0;
	switch(currentSceneCounter) {
	case 0:
		currentScene = scene0;
		break;
	case 1:
		currentScene = scene1;
		break;
	case 2:
		currentScene = scene2;
		break;
	}

	Serial.print("Transitioning into scene ");	Serial.print(currentSceneCounter);
	Serial.print(" for ");	Serial.println(currentScene.scene_duration);
	lastSceneTransition = millis(); // this will cause stall/rounding issues after few up-days

	ServoObj[0].write(currentScene.servo0);
	ServoObj[1].write(currentScene.servo1);
	ServoObj[2].write(currentScene.servo2);
	ServoObj[3].write(currentScene.servo3);

	analogWrite(ledPin[0], currentScene.led0);
	analogWrite(ledPin[1], currentScene.led1);
}

void calibrate() {
	Serial.println("Calibration: use poti1 for ALL servos!");
	while (true) {
		int raw = analogRead(poti1);
		int val = raw * 180.0 / 1023.0 ;
		for (int i=0; i < sizeof(servoPin); i++){
			ServoObj[i].write(val);
		}
		Serial.print(raw); Serial.print(" -> "); Serial.print(val);Serial.println('°');
		delay(25);
	}
}

/*
 * finally here we go: the arduino standart functions
 */
void setup() {
	Serial.begin(115200); Serial.println("hi");

	// setup pinmodes
	pinMode(button1, INPUT);
	pinMode(poti1, INPUT);
	pinMode(poti2, INPUT);
	pinMode(fakelPin, OUTPUT);
	for (int i=0; i < sizeof(ledPin); i++){
		pinMode(ledPin[i], OUTPUT);
	}

	// attaching Servos
	for (int i=0; i < sizeof(servoPin); i++){
		ServoObj[i].attach(servoPin[i]);
	}

	// enter calibration mode: hold button1 while pressing reset
	if (digitalRead(button1)){
		calibrate();
	}

	// transitioning into scene 0 (from -1)
	scene_transition();
}

void loop() {
	if (currentScene.fakel) {
		// flicker fackel with intensity of poti1
		analogWrite(fakelPin, random(0, analogRead(poti1) ));
		delay(random(0,10));
	}

	// scene transition: if button was pressed or scene-time runs out or ???
	if (digitalRead(button1) or
			( (millis() - lastSceneTransition) > currentScene.scene_duration) ){

		while(digitalRead(button1)) delay(10); // cheap debounce - make sure button1 is NOT pressed

		scene_transition();
	}
}
