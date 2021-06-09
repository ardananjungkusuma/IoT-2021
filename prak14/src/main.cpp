#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>

const char *ssid = "synxhronous";   //silakan disesuaikan sendiri
const char *password = "kjkszpj29"; //silakan disesuaikan sendiri

const char *mqtt_server = "ec2-54-209-208-238.compute-1.amazonaws.com";

WiFiClient espClient;
PubSubClient client(espClient);

SimpleDHT11 dht11(D7);

long now = millis();
long lastMeasure = 0;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) //tambahkan parameter untuk username dan password mqtt misalkan ada
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Mqtt Node-RED");
  setup_wifi();
  client.setServer(mqtt_server, 2909);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
  {
    client.connect("ESP8266Client");
  }
  now = millis();
  if (now - lastMeasure > 5000)
  {
    lastMeasure = now;
    int err = SimpleDHTErrSuccess;

    byte temperature = 0;
    byte humidity = 0;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
      Serial.print("Pembacaan DHT11 gagal, err=");
      Serial.println(err);
      delay(1000);
      return;
    }
    static char temperatureTemp[7];
    dtostrf(temperature, 4, 2, temperatureTemp);
    Serial.println(temperatureTemp);

    client.publish("room/suhu", temperatureTemp);
  }
}