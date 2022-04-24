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

const uint8_t calibL[] = {2, 178};
const uint8_t calibR[] = {178, 2};


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

void calibdata(Servo ThisServo) {
	while(true){
		uint8_t val=map(analogRead(A0), 0, 1023, 0, 180);
		Serial.println(val);
		Serial.flush();
		ThisServo.write(val);
	}}

void setup() {
	Serial.begin(9600);
	while (!Serial) {;}

	ServoL.attach(pin_servoL); ServoL.write(calibL[0]);
	ServoR.attach(pin_servoR); ServoR.write(calibR[0]);

	pinMode(pin_ledL, OUTPUT); digitalWrite(pin_ledL, true);
	pinMode(pin_ledR, OUTPUT); digitalWrite(pin_ledR, true);

	pinMode(pin_btnL, INPUT);
	pinMode(pin_btnR, INPUT);

	if (digitalRead(pin_btnL)) { digitalWrite(pin_ledR, false); calibdata(ServoL); }
	if (digitalRead(pin_btnR)) { digitalWrite(pin_ledL, false); calibdata(ServoR); }

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
		while (true) { delay(1); } // no ip - no point in carrying on, so do nothing forevermore:
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

		if (get_line == "/leftDown") {StateL = true;}
		if (get_line == "/leftUp") {StateL = false;}
		if (get_line == "/rightDown") {StateR = true;}
		if (get_line == "/rightUp") {StateR = false;}

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
