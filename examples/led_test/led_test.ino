#define LED_PIN 12

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(LED_PIN , HIGH);
 delay(1000);
 digitalWrite(LED_PIN , LOW);
 delay(1000);
}
