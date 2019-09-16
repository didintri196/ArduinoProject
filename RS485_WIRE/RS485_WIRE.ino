
#include <Wire.h>
#include <SoftwareSerial.h>
#include "RS485_protocol.h"

SoftwareSerial rs485 (0, 1);  // receive pin, transmit pin
const byte ENABLE_PIN = 2;
int clientid=5;
String data="";
void fWrite (const byte what)
  {
  rs485.write (what);  
  }
  
int fAvailable ()
  {
  return rs485.available ();  
  }

int fRead ()
  {
  return rs485.read();  
  }
  
void setup()
{
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Starting");
  rs485.begin(28800); 
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
}

void loop()
{
  byte buf [10];
  
  byte received = recvMsg (fAvailable, fRead, buf, sizeof (buf));
  
  if (received)
    {
    if (buf [0] == clientid){
        Serial.print(char(buf[1]));
        Serial.print(buf[2]);
        Serial.print(buf[3]);
        Serial.println(buf[4]);
        data=char(buf[1]);
        data+=char('|');
        data+=buf[2];
        data+=buf[3];
        data+=buf[4];
        delay (1);  // give the master a moment to prepare to receive
        //String data="B|001";
        Serial.println(data);
        Wire.beginTransmission(1);
        Wire.write(data.c_str());
        Wire.endTransmission();
        //delay(500);
      }
   }  // end if something received
   
}  // end of loop
