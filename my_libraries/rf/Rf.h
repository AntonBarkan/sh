
#ifndef Rf_h
#define Rf_h

#include <iarduino_RF433_Transmitter.h> 
#include <iarduino_RF433_Receiver.h>

class Rf {
  public: Rf(const char* id, byte txPin, byte rxPin); 
    void sendCommand(char* to, char* command);
    char* readCommand();

  private: 
    char _id[2];
    iarduino_RF433_Transmitter* radioTX;
    iarduino_RF433_Receiver*    radioRX;
};

#endif
