
#define TX_PIN 4
#define RX_PIN 2

#include <iarduino_RF433_Transmitter.h> 
#include <iarduino_RF433_Receiver.h>

iarduino_RF433_Transmitter radioTX(TX_PIN);
iarduino_RF433_Receiver    radioRX(RX_PIN);

char  j[14]; 
char  i[]="Hello World!!!"; 

void setup() { 
  Serial.begin(9600);   
  
  radioTX.begin();
  radioTX.setDataRate(i433_1KBPS);
  radioTX.openWritingPipe(5); 

  radioRX.begin(1000); 
  radioRX.setDataRate(i433_1KBPS);
  radioRX.openReadingPipe (5);   
  radioRX.startListening  ();
}
 uint8_t tube;
void loop() {
 
  if(radioRX.available(&tube)){                            
    radioRX.read(&j, sizeof(j));                       
    Serial.print(j);                                   
    Serial.println((String)" (Pipe="+ tube +")");           
  }
//  radioTX.write(&i, sizeof(i));
//  delay(1000);
}
