#define LED_PIN 4

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(LED_PIN , HIGH);
 delay(600);
 digitalWrite(LED_PIN , LOW);
 delay(100);
}
