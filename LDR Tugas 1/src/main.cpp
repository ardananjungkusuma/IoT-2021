#include <Arduino.h>

#define sensorLDR A0
int nilaiSensor;

#define RED_LED D2   //led warna merah
#define GREEN_LED D5 //led warna hijau
#define BLUE_LED D6  //led warna biru

void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("Penggunaan Sensor LDR Smart Home");
  delay(3000);
}

void loop()
{
  // declare variabel untuk mengassign hasil return fungsi analogRead yang membaca cahaya
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor: ");
  Serial.println(nilaiSensor);
  // Jika sensor bernilai kurang dari 700 alias terang maka lampu mati
  if (nilaiSensor < 700)
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, LOW);
  }
  // jika lebih dari sama dengan 700 maka lampu menyala karena dideteksi sebagai gelap
  else
  {
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
  }
  delay(1000);
}