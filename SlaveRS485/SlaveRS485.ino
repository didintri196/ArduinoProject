//Slave code (Arduino NANO)
//Serial Communication Between Two Arduinos using RS-485
//Circuit Digest

#include <LiquidCrystal.h>          //Include LCD library for using LCD display functions 

int enablePin = 8; 
int ledpin = 10;

void setup() 
{
  lcd.begin(16,2);
  lcd.print("CIRCUIT DIGEST");
  lcd.setCursor(0,1);
  lcd.print("RS485 ARDUINO");
  delay(3000);
  lcd.clear();
  Serial.begin(9600);                   // initialize serial at baudrate 9600:
  pinMode(ledpin,OUTPUT);
  pinMode(enablePin, OUTPUT);
  delay(10);
  digitalWrite(enablePin, LOW);        //  (Pin 8 always LOW to receive value from Master)
  
}

void loop() 

{                                                  
  while (Serial.available())                   //While have data at Serial port this loop executes
     {
       
    }
 } 
