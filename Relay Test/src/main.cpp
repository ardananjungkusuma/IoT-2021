#include <Arduino.h>
#define relay D1
int nyala = LOW;
int mati = HIGH;
char perintah;
void setup()
{
  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, mati);
  // put your setup code here, to run once:
}

void loop()
{
  if (Serial.available() > 0)
  {
    perintah = Serial.read();
    if (perintah == ('A'))
    {
      digitalWrite(relay, nyala);
      Serial.print("A – ");
      Serial.println("Lampu Nyala");
    }
    else if (perintah == ('B'))
    {
      digitalWrite(relay, mati);
      Serial.print("B – ");
      Serial.println("Lampu Mati");
    }
    else
    {
      Serial.println("Masukan keyword yang benar");
    }
  }
  // put your main code here, to run repeatedly:
}