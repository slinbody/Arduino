#include <Wire.h>

const int SLAVE_ADDRESS = 1;
char incomingByte = 0;

void setup() {
  Wire.begin();         // join I2C bus as a Master

  Serial.begin(9600);
  Serial.println("Type something to send:");
}

void loop() {
}

void serialEvent()
{
  // read one byte from serial port
  incomingByte = Serial.read();
  Serial.println("Type something to send: ");
  Serial.println(incomingByte);

  // send the received data to slave
  Wire.beginTransmission(SLAVE_ADDRESS);
  Wire.write(incomingByte);
  Wire.endTransmission();
}
