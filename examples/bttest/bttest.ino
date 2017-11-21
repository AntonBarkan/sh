#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8); // RX, TX

void setup() {
  // put your setup code here, to run once:
  pinMode(10, OUTPUT);
  mySerial.begin(9600);
  Serial.begin(9600);

  mySerial.print("AT\r\n");
  delay(500);
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  Serial.print("end");
}

void loop() {

  while (mySerial.available()) {
    char c =mySerial.read();
    Serial.write(c);
    if (c =='1')   {
      digitalWrite(10 , HIGH);
    }else if(c =='6'){
      digitalWrite(10, LOW);
    }
      
  }
}
