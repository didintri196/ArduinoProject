#include "Adafruit_Thermal.h"

#include "SoftwareSerial.h"
#define TX_PIN 11  // Pin 11 Arduino ke RX Printer
#define RX_PIN 10  // Pin 10 Arduino ke TX Printer

SoftwareSerial mySerial(RX_PIN, TX_PIN); 
Adafruit_Thermal printer(&mySerial);  

void setup() {
  
  mySerial.begin(9600);  // Initialize Baudrate SoftwareSerial
  printer.begin();        

  printer.setSize('L');  
  printer.justify('C');
  printer.println(F("Printer Test\n\n\n\n"));

  printer.sleep();      // printer to sleep
  delay(3000L);         // Sleep for 3 seconds
  printer.wake();       
  printer.setDefault(); 
}

void loop() {
}
