#include <Arduino.h>
#include <Wire.h>
#include <Time.h>
#include <LiquidCrystal_I2C.h>

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

char ssid[] = "synxhronous";
char pass[] = "kjkszpj29";

#define RED_LED D2   //led warna merah
#define GREEN_LED D5 //led warna hijau
#define BLUE_LED D6  //led warna biru

#define DHTTYPE DHT11

int timezone = 7 * 3600; // timezone
int dst = 0;             // date swing

DHT dht(D7, DHTTYPE);
WiFiClientSecure client;
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT); //atur pin-pin digital sebagai output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Wire.begin(2, 0); // GPIO 2, 0
  lcd.init();       // initialize the lcd
  lcd.backlight();

  lcd.home();

  // Wifi stuff
  client.setInsecure();
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  configTime(timezone, dst, "pool.ntp.org", "time.nist.gov");

  Serial.println("Waiting for NTP");

  while (!time(nullptr))
  {
    Serial.print("*");
    delay(1000);
  }

  Serial.println("Time Response Ok");
}

void loop()
{
  delay(2000);
  time_t now = time(nullptr);
  struct tm *p_tm = localtime(&now);

  float c = dht.readTemperature();     // Read temp celcius
  float f = dht.readTemperature(true); // Read temp farenheit
  lcd.home();
  if (isnan(c) || isnan(f))
  {
    // set baris 1 start dari kolom 1
    lcd.setCursor(0, 0);
    lcd.print("Failed Read Temperature!");
    // set baris 2 start dari kolom 2
    lcd.setCursor(0, 1);
    // get data dari NTP
    lcd.print(String(p_tm->tm_mday) + String("-") + String(p_tm->tm_mon + 1) + String("-") + String(p_tm->tm_year + 1900) + String(" ") + String(p_tm->tm_hour) + String(":") + String(p_tm->tm_min));
    return;
  }
  else
  {
    lcd.setCursor(0, 0);
    // get data suhu
    lcd.print(String(c) + String("C | ") + String(f) + String("F "));
    lcd.setCursor(0, 1);
    // get data dari NTP
    lcd.print(String(p_tm->tm_mday) + String("-") + String(p_tm->tm_mon + 1) + String("-") + String(p_tm->tm_year + 1900) + String(" ") + String(p_tm->tm_hour) + String(":") + String(p_tm->tm_min));
    if (c >= 30.00 && c <= 32.00)
    {
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, HIGH);
        delay(200);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
      }
    }
    else if (c > 31.00)
    {
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
      }
    }
    else if (c < 30.00)
    {
      for (int i = 0; i < 3; i++)
      {
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
        digitalWrite(RED_LED, LOW);
        digitalWrite(GREEN_LED, LOW);
        digitalWrite(BLUE_LED, LOW);
        delay(200);
      }
    }
  }
}