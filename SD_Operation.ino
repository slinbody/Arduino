/*
操作SD卡
a.創建LOG.TXT
b.刪除LOG.TXT
c.加入字串到LOG.TXT
d.清除LOG.TXT內容
default.讀出LOG.TXT內容
*/
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

File myFile;
char ltr;
String BigString;
char filename[]="EXAMPLE.TXT";

void setup()
{
   Serial.begin(9600);
   //pinMode(53, OUTPUT);
   if(SD.begin(4) == 0) 
      Serial.println("SD failed");
   else
      Serial.println("SD ok");
      
   delay(2000);

   Serial.println("Setup done");
}

void loop()
{
   
  while(Serial.available()>0) {
    ltr =  Serial.read();
  }
  switch (ltr) {
      case 'a':  //Create LOG.TXT
        if (SD.exists(filename)) 
        {
          Serial.println(String(filename) + " already exists.");  
        }
        else
        {
          if(SD.open(filename, FILE_WRITE))
          {
              Serial.println(String(filename) + " is Created Successfully.");
          }    
          else
              Serial.println(String(filename) + " is Failed to Create.");
        }
	ltr = 'z';
        break;
      case 'b':
        if (SD.exists(filename)) 
        {
            Serial.println(String(filename) + " is existed!!removeing...");
            SD.remove(filename);
            Serial.println(String(filename) + " is removed successfully");
        }
        else
            Serial.println(String(filename) + " is not existed");
	ltr = 'z';
        break;
      case 'c': //Insert Content to File
        Serial.println("Insert Content to File stage:");
        delay(5000);
        if (SD.exists(filename))
        {
              BigString = "";
              while(Serial.available()>0) {
                  
                  BigString +=  char(Serial.read());
                  delay(50);
              }
              myFile = SD.open(filename,FILE_WRITE);
              if( myFile )
              {
                  Serial.println("Start to write data");
                  if(myFile.println(BigString))
                  {
                    Serial.print("Write data: ");
                    Serial.println(BigString);
                    Serial.print(" to ");
                    Serial.print(filename);
                    Serial.println("successfully.");
                    myFile.close();
                  }
                  else
                  {
                      Serial.print("Fail to write data to ");
                      Serial.println(filename);
                  }
              } //end of if-myFile
              else 
                  Serial.print("Fail to open ");
                  Serial.println(filename);
          
        }
        else
            Serial.println(String(filename)+" isn't existed");
        break;
      case 'd':
           if (SD.exists(filename)) 
           {
                if(SD.remove(filename))
                    if(SD.open(filename, FILE_WRITE))
                      Serial.println(String(filename)+" Content is cleard.");
           }else
           {
             Serial.println(String(filename)+" isn't existed");
           }
           ltr = 'z';
          break; 
      default:  //Reading LOG.TXT Conten
        if (SD.exists(filename)) 
        {
        BigString="";
        myFile = SD.open(filename);
        while(myFile.available()>0) {
            ltr = myFile.read();
            BigString += ltr;
        }
        myFile.close();
        Serial.print("The Content of " );
        Serial.println(String(filename) +":");
        Serial.println(BigString);
        }
        else
          Serial.println(String(filename)+" isn't existed");
    
  }// end of switch
  delay(5000);
}
