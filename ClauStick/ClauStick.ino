void setup() {
  Serial.begin(115200);
  pinMode(5, INPUT_PULLUP);
}

void loop() {
  Serial.print("X:"); Serial.print(analogRead(A0));
  Serial.print(" Y:"); Serial.print(analogRead(A1));
  Serial.print(" Z:"); Serial.print(analogRead(A2));
  Serial.print(" A:"); Serial.print(digitalRead(5));
  Serial.println();
  delay(10);
}
