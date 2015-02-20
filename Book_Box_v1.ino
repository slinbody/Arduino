

/*

透過網路校正時間
紀錄櫃子狀態Box.txt
紀錄使用紀錄LOG.txt


CONFIG.TXT內 
"Pin:"設定要用的腳位
"Delay:" 設定門通電的秒數(ms)

*/
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <Time.h>
#include<SD.h>

int Max_Door=3;   //總門數
unsigned long Box_Door[58] = {0};//紀錄每門的使用者卡號
int LED[58] = {4,5,6};  //每櫃門的使用腳位

unsigned long user_card_UL;
int Box_status = 0;  		//紀錄目前使用門數
unsigned long inByte[4];
File myFile;
int delay_tt=500;



void setup()
{
    Serial.begin(9600);

    if(SD.begin(4) == 0) 
      Serial.println("SD failed");
    else
      Serial.println("SD ok");
    adjusttime();
    delay(2000);  
    Config_Load();
    Box_Load();
    Serial.println("Setup Done!");
}


void loop()
{  
/*  read from Serial
    while (Serial.available() > 0) {
        delay(10);    //for read smooth
        char ltr = Serial.read();
        user_card += ltr;
    }
*/
    int index=0;
    while (Serial.available()) {
    delay(10);
    inByte[index] = Serial.read();
    index = index + 1;
  }//end of while read card    

    if ( index != 0 ) {
         user_card_UL = inByte[0] << 0 | inByte[1] << 8 | inByte[2] << 16 | inByte[3] << 24;
        if ( check_exist(user_card_UL) ) 
        {   
            //Serial.println(" take his/her item");
            Remove_Box_Door(user_card_UL);
            
        }
        else 
        {
            //Serial.println(" Put his/her item");
            Insert_Box_Door(user_card_UL);
        }    
        for(int i=0;i<Max_Door;i++)
          Serial.println(Box_Door[i]);    
    }    

}  //end of loop



void Remove_Box_Door (unsigned long a)
{
  int tmp = check_exist(a)-1;
  Box_Door[tmp] = 0 ;
  Box_status--;
  digitalWrite(LED[tmp],HIGH);
  delay(delay_tt);
  digitalWrite(LED[tmp],LOW);
  Serial.print(" User : ");  
  Serial.print(a);  
  Serial.print(" take item from door: ");  
  Serial.println(LED[tmp]);
  
  if(SD.exists("LOG.TXT")) 
  {
        //Serial.println("LOG.TXT is existed");
  }
  else
  {
        Serial.println("LOG.TXT isn't existed.");
        if(SD.open("LOG.TXT", FILE_WRITE))
              Serial.println("LOG.TXT is Created.");
  }
     
     myFile = SD.open("LOG.TXT",FILE_WRITE);
     myFile.print(clockDisplay());
     myFile.print(" User:");
     myFile.print(a);
     myFile.print(" take from Door: ");
     myFile.println(LED[tmp]);
     myFile.close();
     
     if(SD.remove("BOX.TXT"))
     {
        if(SD.open("BOX.TXT",FILE_WRITE))
          {   myFile = SD.open("BOX.TXT",FILE_WRITE);
              for(int i=0;i<Max_Door;i++)
              {
                  if(Box_Door[i] != 0)
                  {
                    myFile.print(Box_Door[i]);
                    myFile.print(',');
                    myFile.print(LED[i]);
                    myFile.println(",");
                  }
              }
              myFile.close();
          }
     }
     
} // end of Remove_Box_Door


void Insert_Box_Door (unsigned long a)
{

     if(SD.exists("LOG.TXT")) 
     {
        //Serial.println("LOG.TXT is existed");
     }
     else
     {
        Serial.println("LOG.TXT isn't existed.");
        if(SD.open("LOG.TXT", FILE_WRITE))
            Serial.println("LOG.TXT is Created.");
     }
     
     if(SD.exists("BOX.TXT")) 
     {
        //Serial.println("BOX.TXT is existed");
     }
     else
     {
        Serial.println("BOX.TXT isn't existed.");
        if(SD.open("BOX.TXT", FILE_WRITE))
            Serial.println("BOX.TXT is Created.");
     }
     
     
  
  if (Box_status == Max_Door) {
    myFile = SD.open("LOG.TXT",FILE_WRITE);
    myFile.print(clockDisplay());
    myFile.print(" User:");
    myFile.print(a);
    myFile.println(" Failed because Door is Full");
    Serial.println("Door is Full");
  }
  else 
  {
      for(int i=0; i < Max_Door; i++) 
      {
         if (Box_Door[i] == 0 )
            {
                Box_Door[i] = a;
                Box_status++;
                digitalWrite(LED[i],HIGH);
                delay(delay_tt);
                digitalWrite(LED[i],LOW);
                Serial.print("User : ");  
                Serial.print(a);  
                Serial.print(" Put item into door: ");  
                Serial.println(LED[i]);
                myFile = SD.open("LOG.TXT",FILE_WRITE);
                myFile.print(clockDisplay());
                myFile.print(" User:");
                myFile.print(a);
                myFile.print(" Put into Door: ");
                myFile.println(LED[i]);
                myFile.close();
                
                
                myFile = SD.open("BOX.TXT",FILE_WRITE);
                myFile.print(a);
                myFile.print(",");
                myFile.print(LED[i]);
                myFile.println(",");
                break;
            }
      }
  }
  myFile.close();
}// end of Insert_Box_Door

int check_exist(unsigned long a)
{  
  int tmp= Max_Door;
  for(int i = 0; i<tmp ; i++)
      if (a == Box_Door[i])
          return i+1;
  return 0;
}


unsigned long StoUL (String a)
{
  unsigned long answer=0;
  String tmp = "";
  unsigned long tmp2;
  for (int i=0; i<a.length() ; i++ ) 
  {
        tmp += a[i];
        tmp2 = tenpow(a.length()-i-1);
        answer +=tmp.toInt() * tmp2;
        tmp = "";    
  }
  return answer;
}

unsigned long tenpow(int a)
{
  unsigned long b=1;
  for (int i=1; i <=a ; i++ )
   {
    b *= 10;
   } 
  return b;
}

void Box_Load()
{ 
  String BigString="";
  String BoxString="";
  char ltr;
  int flag = 0;  
  unsigned long tmp;
  if(SD.exists("BOX.TXT"))
  { 
      myFile = SD.open("BOX.TXT");
      while(myFile.available()>0) {
          ltr = myFile.read();
          delay(10);
          if(ltr == ',')
           {
               if(flag%2 == 1) {
                for(int i=0;i <Max_Door;i++) 
                {
                     if(LED[i] == (int)StoUL(BoxString))
                     Box_Door[i] = tmp;    
                }            
                BoxString = "";
                flag++;
                Box_status++;
                continue;
               }
               if(flag%2== 0) {
                    //Serial.print("User: ");
                    //Serial.print(StoUL(BigString));
                    tmp = StoUL(BigString);
                    BigString = "";
                    flag++;
                    
                    continue;
               }
           } //end of if(ltr==',')
           else if(flag%2 == 0)
           {
                    if(ltr != 13 & ltr != 10) 
                    {
                        BigString += ltr;
                    }                                     
           }
           else if(flag%2 == 1)
           {
                    BoxString += ltr;
           }                
//                      tmp = ltr-'0';
            }//end of while
            myFile.close();
    
    Serial.println("BOX.TXT status: ");
    for(int i=0;i<Max_Door;i++) {
        if (Box_Door[i] != 0 ) {
            Serial.print(Box_Door[i]);
            Serial.print(" in Door: ");
            Serial.println(LED[i]);
            //digitalWrite(LED[i],HIGH);
        }
    }

  } // end of if
  else
  {
    Serial.println("BOX.TXT isn't existed.");
    if(SD.open("BOX.TXT",FILE_WRITE))
        Serial.println("BOX.TXT is Created Successfully.");
  }
}


void adjusttime()
{

    Serial.println("SET UP NETWORK.....");
   int i = 0;
   int DHCP = 0;
   byte mac[] = { 0xAA, 0xBB, 0xCC, 0xDD, 0x15, 0xFF };
   unsigned int localPort = 8888;
   IPAddress timeServer(203, 71, 84, 1);
   const long timeZoneOffset = 28800L;  
   const int NTP_PACKET_SIZE= 48;   
   byte packetBuffer[NTP_PACKET_SIZE];  
   EthernetUDP Udp;  
   
   
   DHCP = Ethernet.begin(mac);
   //Try to get dhcp settings 30 times before giving up
   while( DHCP == 0 && i < 30){
     delay(1000);
     DHCP = Ethernet.begin(mac);
     Serial.print(i);
     Serial.println(" FAILED to get IP");
     i++;
   }
   if(!DHCP){
    Serial.println("DHCP FAILED");
     for(;;); //Infinite loop because DHCP Failed
   }
   Serial.print("DHCP Success, IP:");
   Serial.println(Ethernet.localIP());
   
   Serial.println("Set up Time......");
   Udp.begin(localPort);
   memset(packetBuffer, 0, NTP_PACKET_SIZE);
   packetBuffer[0] = 0b11100011;
   packetBuffer[1] = 0;
   packetBuffer[2] = 6;
   packetBuffer[3] = 0xEC;
   packetBuffer[12]  = 49;
   packetBuffer[13]  = 0x4E;
   packetBuffer[14]  = 49;
   packetBuffer[15]  = 52;                  
   Udp.beginPacket(timeServer, 123);
   Udp.write(packetBuffer,NTP_PACKET_SIZE);
   Udp.endPacket();
   delay(1000);
   if (Udp.parsePacket()){
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer
    unsigned long highWord, lowWord, epoch;
    highWord = word(packetBuffer[40], packetBuffer[41]);
    lowWord = word(packetBuffer[42], packetBuffer[43]);  
    epoch = highWord << 16 | lowWord;
    epoch = epoch - 2208988800 + timeZoneOffset;

    setTime(epoch);
   }
    clockDisplay();
    Serial.println("Time is ready.");
} //end of adjusttime

String clockDisplay(){ 
  return String(year())+"/"+String(month())+"/"+String(day())+" "+String(hour())+":"+printDigits(minute())+":"+printDigits(second());
}

String printDigits(int digits){

  if(digits < 10) 
      return '0'+String(+digits);
  return String(digits);
}

void Config_Load()
{    char ltr;
     File myFile;
     String BigString="";
    if(SD.exists("CONFIG.TXT"))
    {
          myFile = SD.open("CONFIG.TXT");
          while(myFile.available()) 
          {
             ltr = myFile.read();
             if (ltr != 13 & ltr!=10)
             {
                BigString += ltr;
             }
             else 
             {        
                if(BigString.length()>0 & BigString[0] == '#') {
                  BigString="";
                  continue;  
                }
                if(BigString.length()>0 & ltr==13) 
                {
                if (BigString.indexOf("Pin") == 0)
                    StoPin(BigString);
                else if (BigString.indexOf("Delay") == 0)
                    StoDelay(BigString);  
                    BigString="";
                }
              }
          } //end of while
    } else
    {
      Max_Door = 3;
    }
  
}

void StoPin(String String1)
{
    Serial.print("Pin Select: ");
    Max_Door = 0;
    String tmp="";
    for(int i=4;i<=String1.length();i++)
    {
        if( i == String1.length() | String1[i] == ',')
         {
             Serial.print(String(tmp.toInt())+',');
             LED[Max_Door] = tmp.toInt();
             pinMode(LED[Max_Door],OUTPUT);
             Max_Door++;
             tmp = "";
         }
         else
         {
            tmp += String1[i]; 
         }
    }
    Serial.println();
}

void StoDelay(String String2)
{
    String tmp;
    tmp = String2.substring(6,String2.length());
    Serial.print("Delay time is :");
    Serial.println(tmp);
    delay_tt = tmp.toInt();
}

