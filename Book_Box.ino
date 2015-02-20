void SaveCard() //
{
  int EmptyNumber=0;
  int EmptyBox[11];
  int tmp;
  if (CardSaveNumber != 11) {
    
    for(int i=1;i<11;i++)
      if(card[i] == 0)  //找出空的櫃子，並亮燈
      {
        digitalWrite(LED[i],HIGH);
        EmptyNumber++;
        EmptyBox[EmptyNumber]=i;
      }

    int timeout=5;
    
    while(timeout)  //五秒內有放書(Sensor偵測到LOW??)就持續等
    {
      delay(1000);
      for(int i=EmptyNumber;i>0;i--)
      {
        if(digitalRead(Sensor[EmptyBox[i]]) == LOW)
	{
          EmptyNumber--;  
	  CardSaveNumber++; 
	  card[EmptyBox[i]] = card[0];
	  digitalWrite(LED[EmptyBox[i]],LOW);
	  timeout=5000;	  
	}

      }

      timeout = timeout - 1;
    }
    MemToSD();
    for(int i=1;i<EmptyNumber;i++) //滅掉全部的燈
      digitalWrite(LED[EmptyBox[i]],LOW);
  }
}





void RemoveCard(int i)  
{
  int UserBox[11];
  int UserBoxNumber = 0;
  for(int t=1;t<11;i++) //找出借書者所有的櫃子
  {
    if (card[t] == card [i])
    {
      UserBoxNumber++;
      UserBox[UserBoxNumber] = t;
    }
  
  }

  while(UserBoxNumber)  //書沒拿完(Sensor偵測到HIGH??)，就一直等
  {
    for(int t=UserBoxNumber;t>0;t--)
      if(digitalRead(Sensor[UserBox[UserBoxNumber]]) == HIGH)
      {
        UserBoxNumber--;
        CardSaveNumber--;
      }
    for(int t=UserBoxNumber;t>0;t--)
      digitalWrite(LED[t],HIGH);
    delay(1000);
    for(int t=UserBoxNumber;t>0;t--)
      digitalWrite(LED[t],LOW);
  }



  card[i] = 0;
  MemToSD();


}
