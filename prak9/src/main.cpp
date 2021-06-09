#include <Arduino.h>
#include <ESP8266WiFi.h>

#define LED D4

const char *ssid = "synxhronous";                               // nama SSID untuk koneksi Anda
const char *password = "kjkszpj29";                             // password akses point WIFI Anda
const uint16_t port = 9001;                                     // diganti dengan port serve Anda
const char *host = "ec2-18-207-155-14.compute-1.amazonaws.com"; //diganti dengan host server Anda, bisa ip ataupun domain

void connect_wifi()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
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
  if (client.connect(host, port))
  {
    Serial.println("connected]");

    Serial.println("[Sending a request]");
    client.print("Hai from ESP8266");

    Serial.println("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
    client.stop();
    Serial.println("\n[Disconnected]");
  }
  else
  {
    Serial.println("connection failed!]");
    client.stop();
  }
  delay(3000);
}

void setup()
{
  Serial.begin(115200);
  connect_wifi();
}

void loop()
{
  connect_server();
}