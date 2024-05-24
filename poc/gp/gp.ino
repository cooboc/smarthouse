void setup() {
  Serial.begin(115200);
  Serial.println("hello world!\r\n");
  pinMode(9, INPUT_PULLUP);
}

void loop() {
  while (true) {
    Serial.print(digitalRead(9));
    Serial.println(".");
    delay(20);
  }
}