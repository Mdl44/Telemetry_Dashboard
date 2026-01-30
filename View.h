#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "Model.h"

class TelemetryView {
private:
  Adafruit_ILI9341* tft;
  Adafruit_NeoPixel* pixels;
  TelemetryModel* model;

  int8_t lastGear;
  uint16_t lastSpeed;
  uint8_t lastLedsOn;

  // Helper methods
  void updateGear();
  void updateSpeed();
  void updateLEDs();

public:
  TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m);

  void init();
  void render();
  void showSplashScreen();
  void drawStaticLayout();
};

#endif