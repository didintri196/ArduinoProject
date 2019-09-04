
#include "RS485_protocol.h"
#include <SoftwareSerial.h>

const byte ENABLE_PIN = 4;
const byte LED_PIN = 13;

SoftwareSerial rs485 (2, 3);  // receive pin, transmit pin

// callback routines
  
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
  return rs485.read ();  
  }

void setup()
{
  rs485.begin (28800);
  pinMode (ENABLE_PIN, OUTPUT);  // driver output enable
  pinMode (LED_PIN, OUTPUT);  // built-in LED



    // assemble message
  byte msg [] = { 
     1,    // device 1
     'B',    // HURUF
     7,
     0,
     5
  };

  // send to slave  
  digitalWrite (ENABLE_PIN, HIGH);  // enable sending
  sendMsg (fWrite, msg, sizeof msg);
  digitalWrite (ENABLE_PIN, LOW);  // disable sending

}  // end of setup
  
byte old_level = 0;

void loop()
{
      

//delay(1000);
}  // end of loop
