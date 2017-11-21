#define JVCPower     0xC5E8
#define TVPower      0x4FB4AB5

#define BUTTON_PIN  12
#define LED_PIN   10
#define RECV_PIN 2
#define BT_SERIAL_RX 7
#define BT_SERIAL_TX 8

#define COMMAND_SIZE 8

#define ReciverPowerComandCode 00000001
#define TVPowerComandCode 00000002
#define TVAndReciverPowerComandCode 00000003

#include "IRremote.h"
#include <SoftwareSerial.h>

SoftwareSerial BTSerial(BT_SERIAL_RX, BT_SERIAL_TX);

IRsend irsend;
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  BTSerial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  recive();
}

void receiveIR() {
  detachInterrupt(0);
  recive();
  attachInterrupt(0, receiveIR, CHANGE);
}

int recive() {
  unsigned long value = readIR();
  if (!value) {
    value = readButton(); 
  }
  if (!value) {
    value = readBTSerial();
  }  
  if (value !=0) {
    digitalWrite(LED_PIN, HIGH );
    if (value == ReciverPowerComandCode) {
      sendJVC(JVCPower);        
    }else if (value == TVPowerComandCode) {
      sendNEC(TVPower);
    } else if (value == TVAndReciverPowerComandCode){
      sendJVC(JVCPower); 
      sendNEC(TVPower);
    }
    digitalWrite(LED_PIN, LOW);
    irrecv.enableIRIn();
  }
}

unsigned long readButton() { 
  return digitalRead(BUTTON_PIN) == HIGH ? 0 : ReciverPowerComandCode;
}



unsigned long readBTSerial(){
  byte i = 0;
  unsigned long command = 0;
  while (BTSerial.available() && i < COMMAND_SIZE) {
    char c = BTSerial.read();
    command = (command * 10) + (c - '0');
  }
  return command;
}

unsigned long readIR(){
  if (irrecv.decode(&results)) { 
    if (results.value == 0xFF6897) {
      return ReciverPowerComandCode;
    } else if (results.value == 0xFF9867){
      return TVPowerComandCode;
    } else if (results.value == 0xFF02FD) {
    return TVAndReciverPowerComandCode;
   }
   irrecv.resume();
  }
  return 0;
}

void sendNEC(long command) {
  for(int i = 0; i< 15; i++) {
    Serial.println("tv");
    irsend.sendNEC(command, 32);
    delay(20);
  }
  delay(480);
}

void sendJVC(long command) {
  for(int i = 0; i< 2; i++) {
      Serial.println("JVC");
      irsend.sendJVC(command, 16, 0);
      delay(10);
  }
  delay(490);
}






