#include <Arduino.h>

#define ONBOARD_LED D0
void setup()
{
  Serial.begin(115200); // Set pada speed 115200
  pinMode(ONBOARD_LED, OUTPUT);
}

void loop()
{
  digitalWrite(ONBOARD_LED, HIGH);
  Serial.println("LED MENYALA");
  delay(3000); // Delay 3 detik
  digitalWrite(ONBOARD_LED, LOW);
  Serial.println("LED MATI");
  delay(3000);
}