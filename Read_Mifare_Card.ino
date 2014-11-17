//Mifare卡，例如：悠遊卡
unsigned long card = 0 ;
unsigned long inByte[4];
unsigned long mifare_mask = 1073741823;
void setup()
{
  // initialize both serial ports:
  Serial.begin(9600); 
}

void loop()
{
  int index=0;
   // read from port 0, send to port 1:
  
  while (Serial.available()) {  //讀卡片
  delay(100);
    // get incoming byte:
    inByte[index] = Serial.read();
    Serial.print("data received: ");
    Serial.print(index);
    Serial.print("--->");
    Serial.print(inByte[index], BIN);
    Serial.println(" (BIN), ");
    index = index + 1;
  }//end of while read card
  if (index != 0) {             //計算卡號
  card = inByte[0] << 0 | inByte[1] << 8 | inByte[2] << 16 | inByte[3] << 24 & mifare_mask;
  Serial.println(card,DEC);
  Serial.println("end of read");
  } //end of if
} //end of loop
