#include <PubSubClient.h>
#include "IPAddress.h"

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>

#include <Adafruit_NeoPixel.h>

const char* wifiName = "<wifiname>";
const char* wifiPassword = "<wifi password>";
const IPAddress server(172, 17, 108, 246);
const int STRIPLENGTH = 1;
const int PIN = 12;

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPLENGTH, PIN, NEO_GRB + NEO_KHZ800);

String clientName = "SerialKillersClient";
String subTopic = "SerialKillers";
int toPub = 1;
boolean ledLit = false;
void setup() {
  Serial.begin(115200);
  Serial.println("Setting up the business");
  boolean wifiUp = WiFi.begin(wifiName, wifiPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi Set up");

  strip.begin();
  strip.setPixelColor(0, 0, 255, 0);
  strip.setBrightness(50);
  strip.show();

  mqttClient.setServer(server, 1883);
  mqttClient.setCallback(mqtt_arrived);

  // a simple connect and monitor the mqtt broker setup
  if (!connectMQTT()) {
    delay(200);
    Serial.println("Did not connect to MQ");
  }
  if (mqttClient.connected()) {
    mqtt_handler();
    Serial.println("Connected to MQ");
  }

}

void loop() {
  mqttClient.loop();

}

boolean connectMQTT() {
  if (mqttClient.connected()) {
    return true;
  }

  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += "-";
  clientName += macToStr(mac);

  Serial.print("Connecting to MQTT server ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);

  if (mqttClient.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    if (mqttClient.subscribe((char*)subTopic.c_str())) {
      Serial.print("Subsribed to topic: ");
      Serial.println((char*)subTopic.c_str());
    } else {
      Serial.print("NOT subsribed to topic!");
      Serial.println((char*)subTopic.c_str());
    }

    return true;
  }
  else {
    Serial.println("MQTT connect failed! ");
    return false;
  }
}

void disconnectMQTT() {
  mqttClient.disconnect();
}

void mqtt_handler() {
  if (toPub == 1) {
    if (pubState()) {
      toPub = 0;
    }
  }

  mqttClient.loop();
  delay(100); //let things happen in background
}


void mqtt_arrived(char* subTopic, byte* payload, unsigned int length) { // handle messages arrived
  Serial.print("Message arrived [");
  Serial.print(subTopic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  toggleLight();

}

void toggleLight() {
  Serial.print("Toggling light: ");
  Serial.println(ledLit);
  if (ledLit) {
    strip.setPixelColor(0, 0, 0, 0);
  } else {
    strip.setPixelColor(0, 255, 0, 255);
  }
  strip.show();
  ledLit = !ledLit;
  Serial.print("ledLit now: ");
  Serial.println(ledLit);
}

boolean pubState() { //Publish to teh mqtt broker

  if (!connectMQTT()) {
    delay(100);
    if (!connectMQTT) {
      Serial.println("Could not connect MQTT.");
      Serial.println("Publish state NOK");
      return false;
    }
  }

}


String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

