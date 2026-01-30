#include <WiFi.h>
#include <WiFiUdp.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>

#include "Config.h"
#include "Model.h"
#include "View.h"
#include "Controller.h"

Adafruit_ILI9341 tft = Adafruit_ILI9341(PIN_TFT_CS, PIN_TFT_DC, PIN_TFT_RST);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN_LED_STRIP, NEO_GRB + NEO_KHZ800);
WiFiUDP udp;

TelemetryModel model;
TelemetryView view(&tft, &pixels, &model);
TelemetryController controller(&model, &view, &udp);

void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);

  controller.init();
}

void loop() {
  controller.update();
}