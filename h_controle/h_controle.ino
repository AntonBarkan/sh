#define JVCPower     0xC5E8
#define TVPower      0x4FB4AB5

#define BUTTON_PIN  12
#define LED_PIN   10
#define RECV_PIN 2

#define ReciverPowerComandCode 00000001
#define TVPowerComandCode 00000002
#define TVAndReciverPowerComandCode 00000003



#include "IRremote.h"
IRsend irsend;

IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  irrecv.enableIRIn();
}

boolean buttonWasUp = true;



void loop() {
  recive();
}

void receiveIR() {
  detachInterrupt(0);
  recive();
  attachInterrupt(0, receiveIR, CHANGE);
}

int recive() {
  unsigned long value = 0; 
  
  if (irrecv.decode(&results)) {
      Serial.println(results.value, HEX);    
      if (results.value == 0xFF6897) {
        value = ReciverPowerComandCode;
      } else if (results.value == 0xFF9867){
        value = TVPowerComandCode;
      } else if (results.value == 0xFF02FD) {
        value = TVAndReciverPowerComandCode;
      }
      irrecv.resume();
   } else {
     value = 0;
   }

   if (!value) {
    value = readButton(); 
   }
  
  if (value !=0) {
      digitalWrite(LED_PIN, HIGH );
      if (value == ReciverPowerComandCode) {
        Serial.println("JVC");
        sendJVC(JVCPower);        
      }else if (value == TVPowerComandCode) {
        sendNEC(TVPower);
      } else if (value == TVAndReciverPowerComandCode){
        Serial.println("JVC");
        sendJVC(JVCPower); 
        Serial.println("tv");
        sendNEC(TVPower);
      }
      digitalWrite(LED_PIN, LOW);
      irrecv.enableIRIn();
  }

}

unsigned long readButton() { 
  if (digitalRead(BUTTON_PIN) == HIGH) {
    
    digitalWrite(LED_PIN, LOW);
    return 0;
  } else {
    digitalWrite(LED_PIN, HIGH); 
    return ReciverPowerComandCode;
  }
}

void sendNEC(long command) {
  for(int i = 0; i< 15; i++) {
      irsend.sendNEC(command, 32);
      delay(20);
  }
  delay(480);
}

void sendJVC(long command) {
  for(int i = 0; i< 2; i++) {
      irsend.sendJVC(command, 16, 0);
      delay(10);
  }
  delay(490);
}






