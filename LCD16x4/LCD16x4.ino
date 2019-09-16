#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 4 line display

void setup()
{
  lcd.init();   // initialize the lcd
  lcd.blink_off();
 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("11111111");
  lcd.setCursor(0,1);
  lcd.print("22222222");
  lcd.setCursor(0,2);
  lcd.print("33333333");
  lcd.setCursor(0,3);
  lcd.print("44444444");
}

void loop()
{
}
