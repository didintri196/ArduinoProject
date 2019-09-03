#include <Wire.h>

void setup()
{
  Wire.begin(); 
}

void loop()
{
  Wire.beginTransmission(1);
  Wire.write("B|107");
  Wire.endTransmission();
// 
//  delay(500);
//  
//  Wire.beginTransmission(8);
//  Wire.write('\n');
//  Wire.endTransmission();
 
  delay(500);
  
  Wire.beginTransmission(2);
  Wire.write('H');
  Wire.endTransmission();
 
  delay(500);
  
  Wire.beginTransmission(2);
  Wire.write('L');
  Wire.endTransmission();
 
  delay(500);
}
