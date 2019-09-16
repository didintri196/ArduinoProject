// Insert file Library
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial_Black_16.h>
#include <Wire.h>

const byte slaveId = 1;
SoftDMD dmd(1,1);
// Number of P10 panels used X, Y
DMD_TextBox box(dmd, 3, 1, 32, 16);
DMD_TextBox box2(dmd, 11, 1, 32, 16);  
// Set Box (dmd, x, y, Height, Width) 
String val1="-"; 
String val2=""; 

void setup() {
  //WIRE
  Wire.begin(slaveId); 
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Starting Engine...");

  //LED
  dmd.setBrightness(255); // Set brightness 0 - 255 
  dmd.selectFont(Arial_Black_16); // Font used
  dmd.begin();     // Start DMD 
  dmd.fillScreen(true);
  delay(500);
  dmd.clearScreen();
}
void loop() {
  box2.print(val1); // Display TEXT SFE
  delay(1500);
  box2.clear();
  box.print(val2); // Display TEXT SFE
  delay(2000);
  box.clear();
}

void receiveEvent(int howMany)
{
  char inChar;
  String codepost;
  int i =1;
  while(Wire.available() > 0)
  {
    inChar = Wire.read();  
    //Serial.print(inChar);
    //Serial.print("\n");
    codepost+=inChar;
      if(i==5){
        Serial.println(codepost);
        settext(codepost);
        //val2=codepost;
        codepost="";
        i==1;
      }
    i++;
  }
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
