#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

const int STRIPLENGTH = 1;
const int PIN = 12;
  // Parameter 1 = number of pixels in strip
  // Parameter 2 = pin number (most are valid)
  // Parameter 3 = pixel type flags, add together as needed:
  //   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
  //   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
  //   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
  //   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPLENGTH, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  Serial.begin(115200);
  Serial.println("Setting up the business");
  strip.begin();
  strip.setBrightness(50);
  Serial.println("Strip begin");
  strip.show();
}

int color1 = 0;
int color2 = 85;
int color3 = 170;
void loop() {
  Serial.print("Color 1: ");
  Serial.println(color1);
  Serial.print("Color 2: ");
  Serial.println(color2);
  Serial.print("Color 3: ");
  Serial.println(color3);
  strip.setPixelColor(0, color1, color2, color3);
  strip.show();
  color1++;
  color2++;
  color3++;
  if (color1 > 255) {
    color1 = 0;
  }
  if (color2 > 255) {
    color2 = 0;
  }
  if (color3 > 255) {
    color3 = 0;
  }
  delay(30);
}
