
#include <Wire.h>

void setup()
{
  Wire.begin(); 
}

void loop()
{
  String data="B|001";
  Wire.beginTransmission(1);
  Wire.write(data.c_str());
  Wire.endTransmission();
  delay(500);
 
}
