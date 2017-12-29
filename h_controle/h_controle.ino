#define JVCPower     0xC5E8
#define TVPower      0x4FB4AB5


#define RADIO_RX_PIN 2
#define RECV_PIN_0 4
#define RECV_PIN_1 5
#define RECV_PIN_2 6
#define RECV_PIN_3 7
#define BT_SERIAL_RX 8
#define BT_SERIAL_TX 9
#define RADIO_TX_PIN 10


#define BUTTON_PIN  12
#define LED_PIN   10



#define COMMAND_SIZE 10

#define ReciverPowerComandCode "0000000001"
#define TVPowerComandCode "0000000002"
#define TVAndReciverPowerComandCode "0000000003"
#define VolumeUp "0000000004"
#define VolumeDown "0000000005"

#include "IRremote.h"
#include <SoftwareSerial.h>
#include <Rf.h>

SoftwareSerial BTSerial(BT_SERIAL_RX, BT_SERIAL_TX);

IRsend irsend;
IRrecv irrecv0(RECV_PIN_0);
IRrecv irrecv1(RECV_PIN_1);
//IRrecv irrecv2(RECV_PIN_2);
//IRrecv irrecv3(RECV_PIN_3);
decode_results results;
Rf rf("01", RADIO_TX_PIN, RADIO_RX_PIN);


void setup() {
  Serial.begin(9600);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  BTSerial.begin(9600);
  irrecv0.enableIRIn();
  irrecv1.enableIRIn();
//  irrecv2.enableIRIn();
//  irrecv3.enableIRIn();
}

void loop() {
  recive();
}

void recive() {
  const char* value = readSources();

  if (value !=0) {
    Serial.print(value);
    Serial.println("---");
    digitalWrite(LED_PIN, HIGH );
    if (isEqualsCommands(value, ReciverPowerComandCode)) {
      sendJVC(JVCPower);        
    }else if (isEqualsCommands(value, TVPowerComandCode)) {
      sendNEC(TVPower);
    } else if (isEqualsCommands(value, TVAndReciverPowerComandCode)) {
      sendJVC(JVCPower); 
      sendNEC(TVPower);
    }
    digitalWrite(LED_PIN, LOW);
    irrecv0.enableIRIn();
    irrecv1.enableIRIn();
//    irrecv2.enableIRIn();
//    irrecv3.enableIRIn();
  }
}

bool isEqualsCommands(const char* s0, const char* s1) {
  for (int i = 0 ; i< COMMAND_SIZE ; i++) {
    if (s0[i] != s1[i]) {
      return false;
    }
  }
  return true;
}

const char* readSources() {
  const char* value = readIR();
  if (!value) {
    value = readButton(); 
  }
  if (!value) {
    value = readBTSerial();
  }
  if (!value) {
    value = readRf();
  }
  return value;
}

const char* readButton() {    
  return digitalRead(BUTTON_PIN) == HIGH ? 0 : ReciverPowerComandCode;
}

const char* readRf() {
  return rf.readCommand();
}

const char* readBTSerial(){
  char* command = 0;
  if (BTSerial.available()) {
    command = new char[COMMAND_SIZE]; 
    for (int i = 0 ; i < COMMAND_SIZE; i++) {
      command[i] = BTSerial.read();
    }
  }
  return command;
}

const char* readIR(){
  const char* value = readNextIR(irrecv0, 0);
//  value = readNextIR(irrecv1, value);
//  value = readNextIR(irrecv2, value);
  return readNextIR(irrecv1, value);
}

const char* readNextIR(IRrecv irrecv, const char* value) {
  if (!value) {
    transformeIRSignal(irrecv);
    return value;
  } else {
    return transformeIRSignal(irrecv);
  }
}

const char* transformeIRSignal(IRrecv irrecv){
  const char* value = 0;
  if (irrecv.decode(&results)) { 
    Serial.println(results.value);
    if (results.value == 0xFF6897) {
      value = ReciverPowerComandCode;
    } else if (results.value == 0xFF9867){
      value = TVPowerComandCode;
    } else if (results.value == 0xFF02FD) {
      value = TVAndReciverPowerComandCode;
   }
   irrecv.resume();
  }
  return value;
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






