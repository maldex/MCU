#include "Arduino.h"
#include "Servo.h"

/*
   Object, Pin and value definitions
*/

// input pins
const int button1 =	A5;
const int poti1 =	A4;
const int poti2 =	A3;

// ultrasonic sensor
const int echoOut = 34;
const int echoIn = 32;
boolean contact = false;

// leds
const uint8_t fakelPin 	= 2;
const uint8_t ledPin[7] = { 4, 6, 8, 9, 10, 11, 12 };  // digital out Pins on Arduino

// servos
const uint8_t servoPin[4] =      { 22, 24, 26, 28 };  // digital out Pins on Arduino
// calibration data
const uint8_t servoCalibMin[4] = { 161, 180,  180,  117 };  // baum , fackel , himmel , tor
const uint8_t servoCalibMax[4] = { 42, 25, 19, 24};         // baum , fackel , himmel , tor

// arduino servo objects
Servo ServoObj[4];

/*
   Scene related definitions and data
*/

/* scene related definitions */
struct Scene {   /* definition of scene-data structure */
  long scene_duration;  //in milliseconds
  bool fakel;
  uint8_t servo0; // servo baum
  uint8_t servo1; // servo fackel
  uint8_t servo2; // servo himmel
  uint8_t servo3; // servo tor
  uint8_t led0; // led0 - Spot1
  uint8_t led1; // led1 - Spot2
  uint8_t led2; // led2 - strip Bogen
  uint8_t led3; // led3 - strip Hintergrund
  uint8_t led4; // led4 - strip Vorgergrund
  uint8_t led5; // led5 - strip Links
  uint8_t led6; // led6 - strip Rechts
};

Scene scene0 = { /* default power-on scene */
  1000,					// scene_duration
  false,				// fakel
  servoCalibMin[0],	 // servo baum
  servoCalibMin[1],	 // servo fackel
  servoCalibMin[2],	 // servo himmel
  servoCalibMin[3],	 // servo tor
  0,					// led0 - Spot1
  0,					// led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time to scene1: 5000
Scene scene1 = {
  5000,				// scene_duration
  true,				// fakel
  servoCalibMin[0],	 // servo baum
  servoCalibMin[1],	 // servo fackel
  servoCalibMin[2],	 // servo himmel
  servoCalibMin[3],	 // servo tor
  0,          // led0 - Spot1
  0,          // led1 - Spot2
  0,          // led2 - strip Bogen
  255,          // led3 - strip Hintergrund
  255,         // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time to scene2: 3000

Scene scene2 = {
  6000,				// scene_duration
  false,				// fakel
  servoCalibMin[0],	 // servo baum
  servoCalibMin[1],	 // servo fackel
  servoCalibMin[2],	 // servo himmel
  servoCalibMin[3],	 // servo tor
  255,          // led0 - Spot1
  255,          // led1 - Spot2
  0,          // led2 - strip Bogen
  120,          // led3 - strip Hintergrund
  120,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time: 4000
Scene scene3 = {
  6000,          // scene_duration
  false,        // fakel
  servoCalibMin[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  255,          // led0 - Spot1
  255,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  255,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time: 2000
Scene scene4 = {
  2000,          // scene_duration
  true,        // fakel
  servoCalibMin[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  255,          // led0 - Spot1
  255,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  255,          // led5 - strip Links
  255,          // led6 - strip Rechts
};
// transition time: 5000
Scene scene5 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMin[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  120,          // led0 - Spot1
  120,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  120,          // led5 - strip Links
  120,          // led6 - strip Rechts
};

// transition time: 5000
Scene scene6 = {
  5000,          // scene_duration
  true,        // fakel
  servoCalibMin[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  255,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  255,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 3000                 
Scene scene7 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  120,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 3000                 
Scene scene8 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  255,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 5000                 
Scene scene9 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMax[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  255,          // led2 - strip Bogen
  120,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 3000                 
Scene scene10 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMax[1],  // servo fackel
  servoCalibMax[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  255,          // led2 - strip Bogen
  120,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 10000                 
Scene scene11 = {
  2000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMax[1],  // servo fackel
  servoCalibMax[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  255,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 50000                 
Scene scene12 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMax[1],  // servo fackel
  servoCalibMax[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  255,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 20000                 
Scene scene13 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  255,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 20000                 
Scene scene14 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMax[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  255,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 20000                 
Scene scene15 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMax[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  255,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
// transition time : 20000                 
Scene scene16 = {
  1000,          // scene_duration
  true,        // fakel
  servoCalibMin[0],  // servo baum
  servoCalibMin[1],  // servo fackel
  servoCalibMin[2],  // servo himmel
  servoCalibMin[3],  // servo tor
  0,          // led0 - Spot1
  0,         // led1 - Spot2
  0,          // led2 - strip Bogen
  0,          // led3 - strip Hintergrund
  0,          // led4 - strip Vordergrund
  0,          // led5 - strip Links
  0,          // led6 - strip Rechts
};
int sceneTotal = 16;

unsigned long lastSceneTransition;
volatile uint8_t currentSceneCounter = -1;
Scene currentScene = scene0;

/*
   Functions
*/
void scene_transition() {
  currentSceneCounter += 1;
  if (currentSceneCounter > sceneTotal ) {
    currentSceneCounter = 0;
    contact = false;
  }
  switch (currentSceneCounter) {          // 
    case 0:
      currentScene = scene0;
      break;
    case 1:
      currentScene = scene1;
      break;
    case 2:
      currentScene = scene2;
      break;
    case 3:
      currentScene = scene3;
      break;
    case 4:
      currentScene = scene4;
      break;
    case 5:
      currentScene = scene5;
      break;
    case 6:
      currentScene = scene6;
      break;
    case 7:
      currentScene = scene7;
      break;
    case 8:
      currentScene = scene8;
      break;
    case 9:
      currentScene = scene9;
      break;
    case 10:
      currentScene = scene10;
      break;
    case 11:
      currentScene = scene11;
      break;
    case 12:
      currentScene = scene12;
      break;
    case 13:
      currentScene = scene13;
      break;
    case 14:
      currentScene = scene14;
      break;
    case 15:
      currentScene = scene15;
      break;
    case 16:
      currentScene = scene16;
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
  analogWrite(ledPin[2], currentScene.led2);
  analogWrite(ledPin[3], currentScene.led3);
  analogWrite(ledPin[4], currentScene.led4);
  analogWrite(ledPin[5], currentScene.led5);
  analogWrite(ledPin[6], currentScene.led6);
}

void calibrate() {
  Serial.println("Calibration: use poti1 for ALL servos!");
  while (true) {
    int raw = analogRead(poti1);
    int val = raw * 180.0 / 1023.0 ;
    for (int i = 0; i < sizeof(servoPin); i++) {
      ServoObj[i].write(val);
    }
    Serial.print(raw); Serial.print(" -> "); Serial.print(val); Serial.println('�');
    delay(25);
  }
}

void echoSensor() {
  long echoDur;
  long dis;

  digitalWrite(echoOut, LOW);
  delayMicroseconds(2);
  digitalWrite(echoOut, HIGH);
  delayMicroseconds(10);
  digitalWrite(echoOut, LOW);
  echoDur = pulseIn(echoIn, HIGH);
  if (echoDur < 7000) {
    contact = true;
  }
 // Serial.print('.');
  delay(100);
}

/*
   finally here we go: the arduino standart functions
*/
void setup() {
  Serial.begin(115200); Serial.println("hi");

  // setup pinmodes
  pinMode(button1, INPUT);
  pinMode(poti1, INPUT);
  pinMode(poti2, INPUT);

  pinMode(echoOut, OUTPUT);
  pinMode(echoIn, INPUT);

  pinMode(fakelPin, OUTPUT);
  for (int i = 0; i < sizeof(ledPin); i++) {
    pinMode(ledPin[i], OUTPUT);
  }

  // attaching Servos
  for (int i = 0; i < sizeof(servoPin); i++) {
    ServoObj[i].attach(servoPin[i]);
  }

  /*
    // enter calibration mode: hold button1 while pressing reset
    if (digitalRead(button1)) {
      calibrate();
    }
  */
  // transitioning into scene 0 (from -1)
  scene_transition();
}

void loop() {
  while (contact == false) {
    echoSensor();
  }
  if (currentScene.fakel) {
    // flicker fackel with intensity of poti1
    analogWrite(fakelPin, random(0, analogRead(poti1) ));
    delay(random(0, 10));
  } else {
    analogWrite(fakelPin, 0);
  }


  // scene transition: if button was pressed or scene-time runs out or ???
  //  if (digitalRead(button1) or
  if  ( (millis() - lastSceneTransition) > currentScene.scene_duration)  {

    while (digitalRead(button1)) delay(10); // cheap debounce - make sure button1 is NOT pressed

    scene_transition();
  }
}
