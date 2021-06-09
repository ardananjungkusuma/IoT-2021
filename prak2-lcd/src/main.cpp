#include <Arduino.h>
#include <Wire.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup()
{
  Wire.begin(2, 0); // GPIO 2, 0
  lcd.init();       // initialize the lcd
  lcd.backlight();

  lcd.home();
}

void scrollText(int row, String message, int delayTime, int lcdColumns)
{
  for (int i = 0; i < lcdColumns; i++)
  {
    message = " " + message;
  }
  message = message + " ";
  for (int pos = 0; pos < message.length(); pos++)
  {
    lcd.setCursor(0, row);
    lcd.print(message.substring(pos, pos + lcdColumns));
    delay(delayTime);
  }
}

void loop()
{
  lcd.home();
  float c = 15.00;
  lcd.print(String(c) + String(" °C"));
  scrollText(1, "Suhu saat ini", 250, 16);
}