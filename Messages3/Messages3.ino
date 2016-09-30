#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Arduino.h>
#include <ESP8266mDNS.h>


const char* wifiName = "<wifi name>";
const char* wifiPassword = "<wifi password>";
void setup() {
 Serial.begin(115200);
 Serial.println("Setting up the business");
 boolean wifiUp = WiFi.begin(wifiName, wifiPassword);
 while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
 }
}

void loop() {
  delay(3000);
 Serial.print("Connection status: ");
 Serial.println(WiFi.localIP());
}
