// http://www.sachsendreier.com/asw/projekteundexperimente/webserver_led_steuerung/index.php
// https://www.hobbytronics.co.uk/arduino-serial-buffer-size

#include <Arduino.h>
#include <Ethernet.h>
#include <Servo.h>

Servo ServoR;
Servo ServoL;

#define pin_servoL	A4
#define pin_servoR	A5
#define pin_btnL	A3
#define pin_btnR	A2
#define pin_ledL	5
#define pin_ledR	6

const uint8_t calibL[] = {2, 180};
const uint8_t calibR[] = {180, 2};


byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x02 };

#define HTML_HEADER F("HTTP/1.1 200 OK\n\Content-type: text/html\n\Connnection: close\n")

#define HTML_BODY F("<html><body>\n\
<style>p { color: green; }</style>\n\
<p id='left'>Click me.</p> <p id='right'>Click me.</p>\n\
<script>\n\
function urlOpen(param) { var my = new XMLHttpRequest(); my.open('GET', param); my.send(); }\n\
document.getElementById('left').addEventListener('mousedown', leftDown);\n\
document.getElementById('left').addEventListener('mouseup', leftUp);\n\
document.getElementById('right').addEventListener('mousedown', rightDown);\n\
document.getElementById('right').addEventListener('mouseup', rightUp);\n\
function leftDown() { urlOpen('leftDown');\n\
	document.getElementById('left').innerHTML = 'leftDown'; }\n\
function leftUp() { urlOpen('leftUp');\n\
	document.getElementById('left').innerHTML = 'leftUp'; }\n\
function rightDown() { urlOpen('rightDown');\n\
	document.getElementById('right').innerHTML = 'rightDown'; }\n\
function rightUp() { urlOpen('rightUp');\n\
	document.getElementById('right').innerHTML = 'rightUp'; }\n\
</script>\n\
</body></html>")

EthernetServer WebServer(80);


void maintain_ethernet() {
	switch (Ethernet.maintain()) {
	case 1:
		Serial.println(F("Error: DHCP renew fail"));
		break;
	case 2:
		Serial.println(F("Renewed success"));
		Serial.print(F("My IP address: "));
		Serial.println(Ethernet.localIP());
		break;
	case 3:
		Serial.println(F("Error: rebind fail"));
		break;
	case 4:
		Serial.println(F("Rebind success"));
		Serial.print(F("My IP address: "));
		Serial.println(Ethernet.localIP());
		break;
	default:
		break;

	}

}


void setup() {
	Serial.begin(9600);
	while (!Serial) {;}

	ServoL.attach(pin_servoL); ServoL.write(calibL[0]);
	ServoR.attach(pin_servoR); ServoR.write(calibR[0]);

	pinMode(pin_ledL, OUTPUT);
	pinMode(pin_ledR, OUTPUT);

	pinMode(pin_btnL, INPUT);
	pinMode(pin_btnR, INPUT);

	char dataString[50] = {0};
	sprintf(dataString, "\n%02X:%02X:%02X:%02X:%02X:%02X dhcp ",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
	Serial.print(dataString);

	if (Ethernet.begin(mac) == 0) {
		Serial.println(F("DHCP failed"));
		if (Ethernet.hardwareStatus() == EthernetNoHardware) {
			Serial.println(F("W5100 not found"));
		} else if (Ethernet.linkStatus() == LinkOFF) {
			Serial.println(F("No Link")); //broken - TODO
		}
		// no ip - no point in carrying on, so do nothing forevermore:
		while (true) {
			delay(1);
		}
	}

	Serial.println(Ethernet.localIP());
	WebServer.begin();
}

bool StateL = false;
bool StateR = false;

void loop() {
	maintain_ethernet();

	EthernetClient client = WebServer.available();

	if (client.available()) {
		String get_line = "";
		while (client.available()) {
			String read_tcp_line = client.readStringUntil('\n');
			if (read_tcp_line.substring(0,5) == "GET /") {
				get_line = read_tcp_line.substring(4, read_tcp_line.indexOf(" HTTP/") );
			}
		}
		Serial.print("GETLINE: "); Serial.println(get_line);

//		if (get_line == "/") { ; }
//		else if (get_line == "/rightDown") { R = true; }
//		else if (get_line == "/rightUp") { R = false; }

		if (get_line == "/leftDown") {StateL = true;}
		if (get_line == "/leftUp") {StateL = false;}
		if (get_line == "/rightDown") {StateR = true;}
		if (get_line == "/rightUp") {StateR = false;}

//		if (get_line.indexOf("PRINTLN=") > 0) {
//			String line = get_line.substring(get_line.indexOf("line=") + 5);
//			Serial.println(line);
//			Serial.println("--------------PRINTLN------------------");
//		}
//
//		if (get_line.indexOf("CURSOR=") > 0) {
//			uint8_t row_idx = get_line.indexOf("row=") + 4;
//			uint8_t col_idx = get_line.indexOf("col=") + 4;
//			uint8_t row_val = get_line.substring(row_idx, row_idx + 3).toInt();
//			uint8_t col_val = get_line.substring(col_idx, col_idx + 3).toInt();
//
//			Serial.print(row_val);	Serial.print(" ");	Serial.print(col_val);
//			Serial.println("--------------CURSOR------------------");
//
//		}
//
//		if (get_line.substring(0,7) == F("/?CLEAR")) {
//			Serial.println("--------------CLEAR------------------");
//		}
//

		client.println(HTML_HEADER);
		client.println(HTML_BODY);

		client.flush();
		client.stop();
	}


	bool L = (StateL or digitalRead(pin_btnL));
	bool R = (StateR or digitalRead(pin_btnR));

	digitalWrite(pin_ledL, L);
	digitalWrite(pin_ledR, R);

	ServoL.write(calibL[L]);
	ServoR.write(calibR[R]);

	Serial.flush();
}

//////////////////////////////////////////////////////////////////
//Servo ServoR;
//Servo ServoL;
//
//#define pin_servoL	8
//#define pin_servoR	7
//#define pin_btnL	A2
//#define pin_btnR	A3
//#define pin_ledL	5
//#define pin_ledR	6

void setup_2() {
	Serial.begin(9600);
	Serial.println("setup");

	pinMode(pin_btnL, INPUT);
	pinMode(pin_btnR, INPUT);

	pinMode(pin_ledL, OUTPUT);
	pinMode(pin_ledR, OUTPUT);

	ServoL.attach(pin_servoL);
	ServoR.attach(pin_servoR);
}

void loop_2(){
	bool L = digitalRead(pin_btnL);
	bool R = digitalRead(pin_btnR);

	digitalWrite(pin_ledL, L);
	digitalWrite(pin_ledR, R);

	ServoL.write(180 * L);
	ServoR.write(180 * R);

	Serial.print(L); Serial.println(R);

}
