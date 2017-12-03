#define RX_PIN 3
#include <iarduino_RF433_Receiver.h>  

iarduino_RF433_Receiver    radioRX(RX_PIN);
char                     j[12]; 
uint8_t                  tube;

void setup() {
  Serial.begin(9600);
  radioRX.begin(1000); 
  radioRX.setDataRate(i433_1KBPS);
  radioRX.openReadingPipe (5);   
  radioRX.startListening  ();
//  pinMode(RX_PIN, INPUT);
}

void loop() {
  if(radioRX.available(&tube)){                            
    radioRX.read(&j, sizeof(j));                       
    Serial.print(j);                                   
    Serial.println((String)" (Pipe="+ tube +")");           
  }
}
