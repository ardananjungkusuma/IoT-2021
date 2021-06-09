#include <Arduino.h>

#define sensorLDR A0
int nilaiSensor;

void setup()
{
  Serial.begin(115200);
  Serial.println("Contoh Penggunaan Sensor LDR");
  delay(3000);
}

void loop()
{
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor: ");
  Serial.println(nilaiSensor);
  delay(1000);
}