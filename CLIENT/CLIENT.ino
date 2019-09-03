#include <Wire.h>

const byte slaveId = 8;

void setup()
{
  Wire.begin(slaveId); 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW);    
}

void loop()
{
}

void receiveEvent(int howMany)
{
  char inChar;
  
  while(Wire.available() > 0)
  {
    inChar = Wire.read();  
    Serial.print(inChar);
  }
}
