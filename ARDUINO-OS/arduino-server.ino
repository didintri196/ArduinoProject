#include "RS485_protocol.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//SAMPLE INPUT = CALL|1|B|0|0|1

SoftwareSerial rs485(2,3);  // receive pin, transmit pin

LiquidCrystal_I2C lcd(0x27,16,4);

// callback routines
  
void fWrite(const byte what)
  {
  rs485.write(what);  
  }
  
int fAvailable()
  {
  return rs485.available();  
  }

int fRead()
  {
  return rs485.read();  
  }

String CmdText = "";
const byte ENABLE_PIN = 4;

void setup() 
{
  pinMode(ENABLE_PIN, OUTPUT);  // driver output enable
  Serial.begin(9600);
  rs485.begin(28800);
  
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("SELAMAT DATANG"); 

  lcd.setCursor(0,1);
  lcd.print("ANTRIAN-OS V1");
   
  lcd.setCursor(-4,2); // set cursor to 1 symbol of 2 line
  lcd.print("ACK: STARTING"); 
  
 
}

void loop() 
{  
  lcd.setCursor(-4,2);
  lcd.print("ACK: LISTENING");
  Serial.print("LN\n"); 
  while (Serial.available() == 0) {}  
  CmdText = Serial.readString(); 
  Serial.print("EX\n");
  
  lcd.setCursor(-4,2);
  lcd.print("ACK: EXEC"); 

  lcd.setCursor(-4,3);
  lcd.print(">");
  lcd.print(CmdText); 
  
String command=getValue(CmdText,'|',0);
   
  if(command=="CALL"){
    SendSignalLED(CmdText);
  }
  
}

//FUNGSI EXPLODE
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

void SendSignalLED(String leddata){
  int addrs_led=getValue(leddata,'|',1).toInt();
      String tipeantrian=getValue(leddata,'|',2);
      int digit_1=getValue(leddata,'|',3).toInt();
      int digit_2=getValue(leddata,'|',4).toInt();
      int digit_3=getValue(leddata,'|',5).toInt();
      // assemble message
      char as='-';
      if(tipeantrian=="A"){
        as='A';
      }else if(tipeantrian=="B"){
        as='B';
      }
        byte msg [] = { 
           addrs_led,    // device 1
           as,    // HURUF
           digit_1,
           digit_2,
           digit_3
        };
        // send to slave  
        digitalWrite (ENABLE_PIN, HIGH);  // enable sending
        sendMsg (fWrite, msg, sizeof msg);
        digitalWrite (ENABLE_PIN, LOW);  // disable sending
}
