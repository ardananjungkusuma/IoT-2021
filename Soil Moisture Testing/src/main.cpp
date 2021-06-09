#include <Arduino.h>
const int SensorPin = A0;
int soilMoistureValue = 0;
int soilmoisturepercent = 0;
const int AirValue = 600;
const int WaterValue = 350;
void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop()
{
  // put your main code here, to run repeatedly:
  soilMoistureValue = analogRead(SensorPin); //put Sensor insert into soil
  // Serial.println(soilMoistureValue);
  soilmoisturepercent = map(soilMoistureValue, AirValue, WaterValue, 0, 100);
  Serial.println(soilmoisturepercent + String("%"));
  if (soilmoisturepercent > 70)
  {
    Serial.println("Lembab");
  }
  else
  {
    Serial.println("Kering");
  }
  delay(1000);
}