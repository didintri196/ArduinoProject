#include <SoftwareSerial.h>

#include "thermalprinter.h"

#define ledPin 13
#define rxPin 5
#define txPin 6

int printStatus = 0;

Epson TM88 = Epson(rxPin, txPin); // init the Printer with Output-Pin

void setup()
{

  Serial.begin(115200); //open the USB connection too
  TM88.start();
  TM88.feed(20);
  TM88.cut();
  
}

void loop()                     // run over and over again
{
  
  printStatus = TM88.getStatus();     // get the current status of the TM88 printer
  if (printStatus == 22) {            // if status is 22 then we are good
    Serial.println("printer online");     // debug that we are online
  } else {
    Serial.print("printer offline: ");    // debug that we are offline
    Serial.println(printStatus);          // debug the returned status code  
  } 
  //digitalWrite(ledPin, LOW);    // sets the LED off
  delay(1000);                  // waits for a second
}

void printtiket(){
  TM88.start();
  TM88.justifyCenter();
  TM88.doubleHeightOn();
  TM88.println("UPTD PUSKESMAS KANDANGAN"); 
  TM88.doubleHeightOff();
  TM88.println("Jl. Malang No.109, Kacangan, Kandangan, Kec. Kandangan, Kediri, Jawa Timur 64294"); 
  TM88.feed(1);
  TM88.setSize(60);
  TM88.println("B-001");    
  TM88.reset();  
  TM88.println("28-08-2019");
  TM88.feed(1);  
  
  TM88.justifyRight();
  TM88.println("*Tiket dibawa ke kasir");  
  
  TM88.cut();  
  }
