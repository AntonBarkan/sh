#include "Rf.h"
#include <iarduino_RF433_Transmitter.h> 
#include <iarduino_RF433_Receiver.h> 

#define MESSAGE_SIZE 14

Rf::Rf(const char* id, byte txPin, byte rxPin) {
  radioTX = new iarduino_RF433_Transmitter(txPin);
  radioTX->begin();  
  radioTX->setDataRate(i433_1KBPS);
  radioTX->openWritingPipe(5);
    
  radioRX = new iarduino_RF433_Receiver(rxPin)    ;
  radioRX->begin();
  radioRX->setDataRate(i433_1KBPS); 
  radioRX->openReadingPipe();
  radioRX->startListening(); 
  
  _id[0] = id[0];
  _id[1] = id[1];
}

void Rf::sendCommand(char* to, char* command) {
  char j[MESSAGE_SIZE];
  j[0] = to[0];
  j[1] = to[1];
  j[2] = _id[0];
  j[3] = _id[1];
  for (int i = 4 ; i < MESSAGE_SIZE ; i++) {
    j[i] = command[i - 4];
  }
  radioTX->write(&j, MESSAGE_SIZE);
}

char* Rf::readCommand() {
  char message[MESSAGE_SIZE];
  if(radioRX->available()){
    radioRX->read(&message, MESSAGE_SIZE);
  }

  if (message[0] == _id[0] && message[1] == _id[1]) {
    char* command = new char[10];
    for (int i = 4 ; i < MESSAGE_SIZE ; i++) {
      command[i - 4] = message[i];
    }
    return command;
  }
  return 0;
}
