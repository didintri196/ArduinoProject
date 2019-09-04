// Insert file Library
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>
#include <SoftwareSerial.h>
#include "RS485_protocol.h"

SoftwareSerial rs485 (0, 1);  // receive pin, transmit pin
const byte ENABLE_PIN = 2;


const byte slaveId = 2;
SoftDMD dmd(1,1);
// Number of P10 panels used X, Y
DMD_TextBox box(dmd, 3, 1, 32, 16);
DMD_TextBox box2(dmd, 11, 1, 32, 16);  
// Set Box (dmd, x, y, Height, Width) 
String val1="-"; 
String val2=""; 


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
  //byte bus=  rs485.read();
   //Serial.println(bus[0]);
  return rs485.read();  
  }

void setup() {
  //WIRE
  Serial.begin(9600);
  Serial.println("Starting Engine...");
  
  
  
  
  //LED
  dmd.setBrightness(255); // Set brightness 0 - 255 
  dmd.selectFont(Arial_Black_16); // Font used
  //dmd.begin();     // Start DMD 
  rs485.begin(28800);
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  dmd.fillScreen(true);
  delay(500);
  dmd.clearScreen();
}
void loop() {
  
  
  delay(1000);
  byte buf [10];
  
  byte received = recvMsg(fAvailable, fRead, buf, sizeof(buf));
  Serial.println("AW");
   dmd.begin();
    box2.print(val1); // Display TEXT SFE
  delay(1500);
  box2.clear();
  box.print(val2); // Display TEXT SFE
  delay(2000);
  box.clear();
  box.print(val1); // Display TEXT SFE
  dmd.end();

  
  if (received)
    { 
    if (buf [0] == 1){
      
       Serial.print(char(buf[1]));
        Serial.print(buf[2]);
        Serial.print(buf[3]);
        Serial.println(buf[4]);
        val1=buf[3];
        val1+=buf[4];
        Serial.println(val1);
        delay (1000);  // give the master a moment to prepare to receive
              }
      
   }
  
  
   // end if something received
}



String getValue(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

void settext(String data){
    String abjad = getValue(data, '|', 0);
    String angka = getValue(data, '|', 1);
    val1=abjad;
    val2=angka;
  }
