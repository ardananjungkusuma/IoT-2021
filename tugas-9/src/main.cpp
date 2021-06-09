#include <Arduino.h>
#include <ESP8266WiFi.h>

#include <DHT.h> // Import library
#define DHTTYPE DHT11
DHT dht(D7, DHTTYPE);

const char *ssid = "synxhronous";                               // nama SSID untuk koneksi Anda
const char *password = "kjkszpj29";                             // password akses point WIFI Anda
const uint16_t port = 9001;                                     // diganti dengan port serve Anda
const char *host = "ec2-18-207-155-14.compute-1.amazonaws.com"; //diganti dengan host server Anda, bisa ip ataupun domain

void connect_wifi()
{
  Serial.begin(115200);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) // connect wifi
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void connect_server()
{
  WiFiClient client;

  Serial.printf("\n[Connecting to %s ... ", host);
  while (!client.connect(host, port)) // connect to server
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("Connected.");
}

void setup()
{
  Serial.begin(115200);
  connect_wifi();
  connect_server();
}

void loop()
{
  float h = dht.readHumidity();        // Read Humidity
  float c = dht.readTemperature();     // Read temp celcius
  float f = dht.readTemperature(true); // Read temp farenheit
  WiFiClient client;
  if (client.connect(host, port))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    if (isnan(c) || isnan(f) || isnan(h)) // jika failed mengirimkan failed to read
    {
      client.print("Failed to read from DHT sensor!");
    }
    else // jika sukses mengirimkan data temperature dan humidity
    {
      client.print(String("Temperature : ") + String(c) + String("C | ") + String(f) + String("F | Humidity : ") + String(h) + String("% "));
    }

    Serial.println("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  else
  {
    Serial.println("connection failed!]");
    Serial.println("Trying to connect server");
    connect_server(); // jika gagal maka mencoba konek ke server lagi
  }
  delay(3000);
}