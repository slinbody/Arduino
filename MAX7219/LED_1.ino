// max7219_two

// Based on https://gist.github.com/nrdobie/8193350

#include <SPI.h>

#define SS_PIN 10



// MAX7219 SPI LED Driver

#define MAX7219_TEST 0x0f // in real code put into a .h file
#define MAX7219_BRIGHTNESS 0x0a // in real code put into a .h file
#define MAX7219_SCAN_LIMIT 0x0b // in real code put into a .h file
#define MAX7219_DECODE_MODE 0x09 // in real code put into a .h file
#define MAX7219_SHUTDOWN 0x0C // in real code put into a .h file



void maxTransferCMD(uint8_t address, uint8_t value) {  

uint8_t i;

  digitalWrite(SS_PIN, LOW);   
  SPI.transfer(address);      // Send address.
  SPI.transfer(value);        // Send the value.
  SPI.transfer(address);      // Send address. 
  SPI.transfer(value);        // Send the value.
  digitalWrite(SS_PIN, HIGH); // Finish transfer.
}



//void maxTransferDATA(uint8_t address, uint8_t value, uint8_t v2) {  
void maxTransferDATA(uint8_t address, uint8_t value) {
uint8_t i;
  //digitalWrite(SS_PIN, LOW);   
  SPI.transfer(address);      // Send address.
  SPI.transfer(value);        // Send the value.
//  SPI.transfer(address);      // Send address. 
//  SPI.transfer(v2);        // Send the value.
  //digitalWrite(SS_PIN, HIGH); // Finish transfer.
}

   

void setup() { 

  Serial.begin(9600);
  Serial.println("Debug MAX7219");
  pinMode(SS_PIN, OUTPUT);  
  SPI.setBitOrder(MSBFIRST);   // Reverse the SPI Data o/p. 
  SPI.begin();                 // Start SPI
  // Run test - All LED segments lit. 

  maxTransferCMD(MAX7219_TEST, 0x01);  delay(1000);  
  maxTransferCMD(MAX7219_TEST, 0x00);        // Finish test mode.
  maxTransferCMD(MAX7219_DECODE_MODE, 0x00); // Disable BCD mode. 
  //maxTransferCMD(MAX7219_BRIGHTNESS, 0x00);  // Use lowest intensity.
        digitalWrite(SS_PIN, LOW);
        maxTransferDATA(MAX7219_BRIGHTNESS,0x00);             
        maxTransferDATA(MAX7219_BRIGHTNESS,0x00);
        digitalWrite(SS_PIN, HIGH);   
  maxTransferCMD(MAX7219_SCAN_LIMIT, 0x0f);  // Scan all digits.
  maxTransferCMD(MAX7219_SHUTDOWN, 0x01);    // Turn on chip.
}

 

void loop() {  

uint8_t row=0;
char data[][8] = {{0x00,0x04,0x7E,0x7F,0x7E,0x04,0x00,0x00},  //向上的箭頭
                {0x00,0x00,0x40,0x7F,0x7F,0x42,0x00,0x00},  //數字 1
                {0x00,0x00,0x4E,0x51,0x51,0x62,0x00,0x00},  //數字 2
                {0x00,0x00,0x36,0x49,0x41,0x22,0x00,0x00},  //數字 3
                {0x00,0x00,0x00,0x7F,0x7E,0x24,0x38,0x00},  //數字 4
                {0x00,0x7E,0x7E,0x7E,0x7E,0x7E,0x7E,0x00}};
                
byte obj[16]={0x00,0x00,0x4E,0x51,0x51,0x62,0x00,0x00,0x00,0x00,0x40,0x7F,0x7F,0x42,0x00,0x00}; //21
                
   for(row = 1;row<9;row++)
      maxTransferDATA(row, 0);
   for(;;) {
    
           
       for(row=1;row<9;row++)      
       {
        digitalWrite(SS_PIN, LOW);
        maxTransferDATA(row,obj[row-1]);    //因為MAX7219一次讀一行         
        maxTransferDATA(row,obj[row-1+8]);  //所以要一行一行寫入
        digitalWrite(SS_PIN, HIGH);
        delay(200);
       }
       //digitalWrite(SS_PIN, LOW);
       //digitalWrite(SS_PIN, HIGH);
      
      
      delay(1000);
      for(row = 1;row<9;row++)
      {
        digitalWrite(SS_PIN, LOW);
        maxTransferDATA(row,0xff);
        maxTransferDATA(row,0xff);
        digitalWrite(SS_PIN, HIGH);
        delay(200);                   
      }
      delay(1000);
   } // end for(;;)
}
