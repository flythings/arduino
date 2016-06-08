#include <SPI.h>
#include <ESP8266WiFi.h>
#include <FlyThings.h>
#include "DHT.h"

#define DHTPIN 5     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE, 15);

#define SSID "YOUR_WIFI"
#define SSID_PASSWORD "YOUR_WIFI_PASS"

#define TOKEN "YOUR_TOKEN"
#define WORKSPACE "YOUR_WORKSPACE"

FlyThings flythings (TOKEN, WORKSPACE);

void setup() {
  dht.begin();
  Serial.begin(115200);
  delay(10);
  flythings.add_wifi(SSID, SSID_PASSWORD);
}

void loop() {

  //Prueba
  flythings.sendData(dht.readHumidity(), "Test", "DHT22", "Humidity", "%");
  flythings.sendData(dht.readTemperature(), "Test", "DHT22", "Temp", "cel");

  delay(300000);
}
