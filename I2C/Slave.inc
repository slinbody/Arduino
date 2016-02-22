#include <Wire.h>

const int SLAVE_ADDRESS = 1;
char incomingByte = 'B';

void setup() {  
  pinMode(13,OUTPUT);
  pinMode(12,OUTPUT);
  Wire.begin(SLAVE_ADDRESS);    // join I2C bus as a slave with address 1
  Wire.onReceive(receiveEvent); // register event
  
  Serial.begin(9600);
  Serial.println("Received data:");
}

void loop() {

}

void receiveEvent(int howMany)
{
  while (Wire.available()) 
  {
    // receive one byte from Master
    incomingByte = Wire.read();
  if(incomingByte == 'A')
    digitalWrite(13,HIGH);
  else if (incomingByte == 'B')
    digitalWrite(12,HIGH);
  else {
    digitalWrite(13,LOW);
    digitalWrite(12,LOW);
  }
  delay(100);    
    Serial.print(incomingByte);
  }
}
