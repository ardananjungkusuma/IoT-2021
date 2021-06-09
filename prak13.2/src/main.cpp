#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>

const char *ssid = "synxhronous";   //silakan disesuaikan sendiri
const char *password = "kjkszpj29"; //silakan disesuaikan sendiri

const char *mqtt_server = "broker.sinaungoding.com";

#define RED_LED D5   //led warna merah
#define GREEN_LED D6 //led warna hijau
#define BLUE_LED D8  //led warnah biru
#define sensorLDR A0
int nilaiSensorLdr;

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

// Function for led
void ledOff()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, LOW);
}

void ledRedOn()
{
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(RED_LED, HIGH);
}

void ledGreenOn()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
}

void ledBlueOn()
{
  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(BLUE_LED, HIGH);
}

// Callback untuk listen jika ada publish dari nodered
void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String messageTemp;
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }

  Serial.println();

  // Filter by topic
  if (String(topic) == "1841720041room/lampred")
  {
    if (messageTemp == "Nyala")
    {
      Serial.println("Nyala");
      ledRedOn();
    }
    else
    {
      Serial.println("Mati");
      ledOff();
    }
  }
  else if (String(topic) == "1841720041room/lampgreen")
  {
    if (messageTemp == "Nyala")
    {
      Serial.println("Nyala");
      ledGreenOn();
    }
    else
    {
      Serial.println("Mati");
      ledOff();
    }
  }
  else if (String(topic) == "1841720041room/lampblue")
  {
    if (messageTemp == "Nyala")
    {
      Serial.println("Nyala");
      ledBlueOn();
    }
    else
    {
      Serial.println("Mati");
      ledOff();
    }
  }
}

void reconnect()
{
  while (!client.connected())
  {
    if (client.connect("ESP8266Client"))
    {
      Serial.println("Connected.");
      // Subscribe agar listen ke noderednya
      Serial.println(client.subscribe("1841720041room/lampred"));
      Serial.println(client.subscribe("1841720041room/lampgreen"));
      Serial.println(client.subscribe("1841720041room/lampblue"));
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
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
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

    // Reading LDR Censors
    nilaiSensorLdr = analogRead(sensorLDR);
    byte temperature = 0;
    byte humidity = 0;
    if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
    {
      Serial.print("Pembacaan DHT11 gagal, err=");
      Serial.println(err);
      delay(1000);
      return;
    }
    // instansiasi variabel char
    static char temperatureTemp[7];
    static char humString[8];
    static char light[9];

    // Convert
    dtostrf(humidity, 1, 2, humString);
    dtostrf(temperature, 4, 2, temperatureTemp);
    dtostrf(nilaiSensorLdr, 4, 2, light);

    Serial.println(temperatureTemp);
    Serial.println(humString);
    Serial.println(light);

    // Publish hasil output dari tiap sensor berdasarkan topic
    client.publish("1841720041room/kelembaban", humString);
    client.publish("1841720041room/suhu", temperatureTemp);
    client.publish("1841720041room/ldr", light);
  }
}