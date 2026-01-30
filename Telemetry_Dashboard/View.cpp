#include "View.h"

TelemetryView::TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m) {
  tft = display;
  pixels = leds;
  model = m;

  lastGear = -99;
  lastSpeed = -1;
  lastLedsOn = -1;
}

void TelemetryView::init() {

  tft->begin();
  tft->setRotation(DISPLAY_ROTATION);
  tft->fillScreen(COLOR_BLACK);

  pixels->begin();
  pixels->setBrightness(LED_BRIGHTNESS_DEFAULT);
  pixels->clear();
  pixels->show();

  showSplashScreen();
}

void TelemetryView::showSplashScreen() {
  tft->fillScreen(COLOR_BLACK);

  UserSettings* settings = model->getSettings();

  tft->setCursor(10, 50);
  tft->setTextColor(COLOR_GREEN);
  tft->setTextSize(2);
  tft->println("WIFI ACTIVE:");
  tft->println(settings->wifiSSID);

  tft->println();
  tft->setTextColor(COLOR_YELLOW);
  tft->println("Waiting for F1 23...");
}

void TelemetryView::render() {
  updateGear();
  updateSpeed();
  updateLEDs();
}

void TelemetryView::drawStaticLayout() {
  tft->fillScreen(COLOR_BLACK);

  tft->drawFastVLine(160, 0, 240, COLOR_DARKGREY);

  tft->drawRect(10, 40, 140, 160, COLOR_WHITE);
  tft->setTextSize(2);
  tft->setTextColor(COLOR_YELLOW);
  tft->setCursor(55, 15);
  tft->print("GEAR");

  tft->drawRect(170, 40, 140, 160, COLOR_WHITE);
  tft->setCursor(205, 15);
  tft->print("SPEED");

  tft->setCursor(115, 215);
  tft->setTextSize(1);
  tft->setTextColor(COLOR_DARKGREY);
  tft->print("REV LIGHTS");

  tft->setTextSize(2);
  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(203, 165);
  tft->print("km/h");
}

void TelemetryView::updateGear() {
  int8_t gear = model->getGear();

  if (gear != lastGear) {
    tft->fillRect(11, 41, 138, 158, COLOR_BLACK);

    tft->setTextSize(15);

    if (gear == 0) {
      tft->setTextColor(COLOR_GREEN);
      tft->setCursor(45, 75);
      tft->print("N");
    } else if (gear == -1) {
      tft->setTextColor(COLOR_RED);
      tft->setCursor(45, 75);
      tft->print("R");
    } else {
      tft->setTextColor(COLOR_CYAN);
      if (gear < 10) {
        tft->setCursor(50, 75);
      } else {
        tft->setCursor(35, 75);
      }
      tft->print(gear);
    }

    lastGear = gear;
  }
}

void TelemetryView::updateSpeed() {
  uint16_t speed = model->getSpeed();

  if (speed != lastSpeed) {
    tft->fillRect(171, 41, 138, 120, COLOR_BLACK);

    tft->setTextColor(COLOR_WHITE);
    tft->setTextSize(7);

    if (speed < 10) {
      tft->setCursor(220, 75);
    } else if (speed < 100) {
      tft->setCursor(200, 75);
    } else {
      tft->setCursor(185, 75);
    }
    tft->print(speed);

    lastSpeed = speed;
  }
}

void TelemetryView::updateLEDs() {
  uint8_t revPercent = model->getRevLightsPercent();

  int ledsOn = map(revPercent, 0, 100, 0, NUM_LEDS);

  if (ledsOn != lastLedsOn) {
    pixels->clear();

    for (int i = 0; i < ledsOn; i++) {
      if (i < LED_GREEN_COUNT) {
        pixels->setPixelColor(i, pixels->Color(0, 150, 0));
      } else if (i < LED_GREEN_COUNT + LED_YELLOW_COUNT) {
        pixels->setPixelColor(i, pixels->Color(150, 150, 0));
      } else {
        pixels->setPixelColor(i, pixels->Color(150, 0, 0));
      }
    }

    pixels->show();
    lastLedsOn = ledsOn;
  }
}