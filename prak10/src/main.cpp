#include <Arduino.h>
#include <ESP8266WiFi.h>

const char *ssid = "synxhronous";                               // nama SSID untuk koneksi Anda
const char *password = "kjkszpj29";                             // password akses point WIFI Anda
const uint16_t port = 9001;                                     // diganti dengan port serve Anda
const char *host = "ec2-18-207-155-14.compute-1.amazonaws.com"; //diganti dengan host server Anda, bisa ip ataupun domain

WiFiClient client;

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
  delay(250);
}

void connect_server()
{
  while (!client.connect(host, port))
  {
    Serial.printf("\n[Connecting to %s ... ", host);
    delay(1000);
    return;
  }
  Serial.println("connected]");
  delay(1000);
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Contoh penggunaan socket client");
  connect_wifi();
  connect_server();
}

void loop()
{
  if (client.connected())
  {
    Serial.print("[Response:]");
    String line = client.readStringUntil('\n');
    Serial.println(line);
    if (line.equalsIgnoreCase("led-on"))
    {
      pinMode(LED_BUILTIN, HIGH);
      delay(3000);
      pinMode(LED_BUILTIN, LOW);
    }
  }
  else
  {
    connect_server();
  }
  delay(250);
}