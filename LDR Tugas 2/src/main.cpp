#include <Arduino.h>
#include <DHT.h>

#define sensorLDR A0
int nilaiSensor;

#define RED_LED D2   //led warna merah
#define GREEN_LED D5 //led warna hijau
#define BLUE_LED D6  //led warna biru

#define DHTTYPE DHT11

DHT dht(D7, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("Penggunaan Sensor LDR & DHT11 Smart Home");
  delay(3000);
}

void loop()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  float celcius = dht.readTemperature();
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor: ");
  Serial.println(nilaiSensor);

  if (isnan(celcius)) // Jika gagal baca suhu
  {
    // lampu hijau menyala
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, LOW);
  }
  else
  {
    Serial.println(F("Temperature: "));
    Serial.println(celcius);
    if (nilaiSensor > 700 && celcius <= 30.00) // Jika cahaya redup dan suhu dingin
    {
      // biru kedip"
      for (int x = 0; x < 5; x++)
      {
        digitalWrite(BLUE_LED, HIGH); // LED nyala
        delay(400);                   // delay selama 400ms
        digitalWrite(BLUE_LED, LOW);  // LED mati
        delay(100);                   // delay selama 100ms
      }
    }
    else if (nilaiSensor < 700 && celcius > 30.00) // Jika Cahaya terang dan suhu tergolong tinggi
    {
      // merah menyala
      digitalWrite(RED_LED, HIGH);
      delay(1000);
    }
    else // Jika tidak memenuhi kondisi diatas
    {
      //lampu hijau menyala
      digitalWrite(GREEN_LED, HIGH);
      delay(1000);
    }
  }
  delay(2000);
}