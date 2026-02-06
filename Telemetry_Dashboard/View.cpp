#include "View.h"

// ============================================
// CONSTRUCTOR
// ============================================
TelemetryView::TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m) {
  tft = display;
  pixels = leds;
  model = m;

  // Screen Management
  currentScreen = SCREEN_GENERAL;
  screenChanged = true;
  bootInfoDrawn = false;

  // ============================================
  // SCREEN 1: GENERAL - Initialize Dirty Tracking
  // ============================================
  lastPosition = 255;
  lastDeltaFront = 65535;
  lastDeltaLeader = 65535;
  lastDeltaLive = -999.0f;
  lastLapTime = 0;
  lastCurrentLapTime = 0;
  lastSector1 = 65535;
  lastSector2 = 65535;
  lastCurrentLapNum = 255;
  lastCornerCuttingWarnings = 255;

  lastSpeed = 65535;
  lastThrottle = -999.0f;
  lastBrake = -999.0f;
  lastGear = -99;
  lastRPM = 65535;

  lastDRS = 255;
  lastRevLightsPercent = 255;
  lastSuggestedGear = -99;
  lastFrontBrakeBias = 255;
  lastDiffOnThrottle = 255;

  lastFuelInTank = -999.0f;
  lastFuelRemainingLaps = -999.0f;
  lastERSEnergy = -999.0f;
  lastERSMode = 255;

  lastLedsOn = 255;

  // ============================================
  // SCREEN 2: TYRE INFO - Initialize Dirty Tracking
  // ============================================
  lastTyresAgeLaps = 255;

  lastBrakeTempFL = 65535;
  lastBrakeTempFR = 65535;
  lastBrakeTempRL = 65535;
  lastBrakeTempRR = 65535;

  lastTyreSurfTempFL = 255;
  lastTyreSurfTempFR = 255;
  lastTyreSurfTempRL = 255;
  lastTyreSurfTempRR = 255;

  lastTyreInnerTempFL = 255;
  lastTyreInnerTempFR = 255;
  lastTyreInnerTempRL = 255;
  lastTyreInnerTempRR = 255;

  lastTyrePressureFL = -1.0f;
  lastTyrePressureFR = -1.0f;
  lastTyrePressureRL = -1.0f;
  lastTyrePressureRR = -1.0f;

  lastTyreWearFL = -1.0f;
  lastTyreWearFR = -1.0f;
  lastTyreWearRL = -1.0f;
  lastTyreWearRR = -1.0f;

  lastTyreDamageFL = 255;
  lastTyreDamageFR = 255;
  lastTyreDamageRL = 255;
  lastTyreDamageRR = 255;

  lastBrakeDamageFL = 255;
  lastBrakeDamageFR = 255;
  lastBrakeDamageRL = 255;
  lastBrakeDamageRR = 255;

  // ============================================
  // SCREEN 3: CAR INFO - Initialize Dirty Tracking
  // ============================================
  lastICEPower = -1.0f;
  lastMGUKPower = -1.0f;
  lastERSPercent = -1.0f;
  lastEngineTemp = 65535;
  lastDRSFault = 255;
  lastERSFault = 255;

  lastWingDamageFL = 255;
  lastWingDamageFR = 255;
  lastWingDamageRear = 255;
  lastFloorDamage = 255;
  lastDiffuserDamage = 255;
  lastSidepodDamage = 255;

  lastGearboxDamage = 255;
  lastICEDamage = 255;
  lastMGUHDamage = 255;
  lastMGUKDamage = 255;
  lastTCDamage = 255;
  lastERSDamage = 255;
  lastCEDamage = 255;

  // ============================================
  // SCREEN 4: RACE OVERVIEW - Initialize Dirty Tracking
  // ============================================
  lastWeather = 255;
  lastTrackTemp = -99;
  lastAirTemp = -99;

  lastSessionType = 255;
  lastCurrentLapInfo = 255;
  lastTotalLaps = 255;
  lastSessionTimeLeft = 65535;
  lastSafetyCarStatus = 255;

  lastBestLapTime = 0;
  lastLastLapTime = 0;
  lastSector1Time = 65535;
  lastSector2Time = 65535;

  lastFuelRemaining = -1.0f;
  lastTyresAge = 255;
  lastEngineTempCritical = 65535;
  lastOverallDamage = 255;
}

// ============================================
// INIT
// ============================================
void TelemetryView::init() {
  tft->begin();
  tft->setRotation(DISPLAY_ROTATION);
  tft->fillScreen(COLOR_BLACK);

  pixels->begin();
  pixels->setBrightness(LED_BRIGHTNESS_DEFAULT);
  pixels->clear();
  pixels->show();
}

// ============================================
// RENDER - Main Loop
// ============================================
void TelemetryView::render() {
  switch (currentScreen) {
    case SCREEN_GENERAL:
      if (screenChanged) {
        drawGeneralScreen();
        resetGeneralDirtyTracking();
        screenChanged = false;
      }
      updatePosition();
      updateDeltaFront();
      updateDeltaLeader();
      updateDeltaLive();
      updateLastLapTime();
      updateCurrentLapTime();
      updateSector1();
      updateSector2();
      updateCurrentLapNum();
      updateCornerCuttingWarnings();
      updateSpeed();
      updateThrottle();
      updateBrake();
      updateGear();
      updateRPM();
      updateDRS();
      updateRevLights();
      updateSuggestedGear();
      updateFrontBrakeBias();
      updateDiffOnThrottle();
      updateFuelInTank();
      updateFuelRemainingLaps();
      updateERSEnergy();
      updateERSMode();
      break;

    case SCREEN_TYRE_INFO:
      if (screenChanged) {
        drawTyreInfoScreen();
        resetTyreInfoDirtyTracking();
        screenChanged = false;
      }
      updateTyresAgeLaps();
      updateBrakeTemps();
      updateTyreSurfaceTemps();
      updateTyreInnerTemps();
      updateTyrePressures();
      updateTyreWear();
      updateTyreDamage();
      updateBrakeDamage();
      break;

    case SCREEN_CAR_INFO:
      if (screenChanged) {
        drawCarInfoScreen();
        resetCarInfoDirtyTracking();
        screenChanged = false;
      }
      updatePowerUnit();
      updateAeroDamage();
      updateComponentWear();
      break;

    case SCREEN_SESSION_INFO:
      if (screenChanged) {
        drawSessionInfoScreen();
        resetSessionInfoDirtyTracking();
        screenChanged = false;
      }
      updateWeather();
      updateTrackTemp();
      updateAirTemp();

      updateSessionType();
      updateLapInfo();
      updateSessionTimeLeft();
      updateSafetyCarStatus();

      updateBestLap();
      updateLastLap();
      updateSectorTimes();

      updateFuelStatus();
      updateTyreStatus();
      updateEngineStatus();
      updateDamageStatus();
      break;
  }
  updateLEDs();
}

void TelemetryView::drawLayout() {
  screenChanged = true;
  render();
}

void TelemetryView::nextScreen() {
  currentScreen = (Screen)((currentScreen + 1) % 4);
  screenChanged = true;
}

// ============================================
// SCREEN DRAWING METHODS
// ============================================

void TelemetryView::drawGeneralScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->drawRect(0, 0, 40, 240, COLOR_WHITE);

  tft->drawRect(280, 0, 40, 240, COLOR_WHITE);

  for (int i = 0; i < 9; i++) {
    tft->drawRect(42, i * 24, 68, 24, COLOR_WHITE);
  }

  tft->drawRect(112, 0, 76, 24, COLOR_WHITE);
  tft->drawRect(112, 24, 76, 96, COLOR_WHITE);
  tft->drawRect(112, 120, 76, 96, COLOR_WHITE);

  for (int i = 0; i < 9; i++) {
    tft->drawRect(190, i * 24, 88, 24, COLOR_WHITE);
  }

  tft->drawRect(42, 216, 236, 24, COLOR_WHITE);
}

void TelemetryView::drawTyreInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->drawRect(0, 0, 320, 24, COLOR_WHITE);

  tft->drawRect(0, 24, 160, 108, COLOR_WHITE);
  tft->drawRect(160, 24, 160, 108, COLOR_WHITE);
  tft->drawRect(0, 132, 160, 108, COLOR_WHITE);
  tft->drawRect(160, 132, 160, 108, COLOR_WHITE);

  tft->setTextSize(1);

  tft->setTextColor(COLOR_CYAN);
  tft->setCursor(30, 28);
  tft->print("FRONT LEFT");

  tft->setCursor(190, 28);
  tft->print("FRONT RIGHT");

  tft->setCursor(35, 136);
  tft->print("REAR LEFT");

  tft->setCursor(190, 136);
  tft->print("REAR RIGHT");
}

void TelemetryView::drawCarInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->drawRect(2, 2, 156, 104, COLOR_CYAN);
  tft->setTextSize(1);
  tft->setTextColor(COLOR_CYAN);
  tft->setCursor(40, 8);
  tft->print("POWER UNIT");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(8, 25);
  tft->print("ICE:");
  tft->setCursor(8, 40);
  tft->print("MGU-K:");
  tft->setCursor(8, 55);
  tft->print("ERS:");
  tft->setCursor(8, 70);
  tft->print("TEMP:");
  tft->setCursor(8, 85);
  tft->print("STATUS:");

  tft->drawRect(162, 2, 156, 104, COLOR_ORANGE);
  tft->setTextColor(COLOR_ORANGE);
  tft->setCursor(185, 8);
  tft->print("AERODYNAMICS");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(168, 25);
  tft->print("Wing FL:");
  tft->setCursor(168, 37);
  tft->print("Wing FR:");
  tft->setCursor(168, 49);
  tft->print("Rear Wing:");
  tft->setCursor(168, 61);
  tft->print("Floor:");
  tft->setCursor(168, 73);
  tft->print("Diffuser:");
  tft->setCursor(168, 85);
  tft->print("Sidepod:");

  tft->drawRect(2, 110, 316, 128, COLOR_MAGENTA);
  tft->setTextColor(COLOR_MAGENTA);
  tft->setCursor(70, 116);
  tft->print("COMPONENT WEAR");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(8, 130);
  tft->print("Gearbox:");
  tft->setCursor(8, 145);
  tft->print("ICE:");
  tft->setCursor(8, 160);
  tft->print("MGU-H:");
  tft->setCursor(8, 175);
  tft->print("MGU-K:");
  tft->setCursor(8, 190);
  tft->print("TC:");
  tft->setCursor(8, 205);
  tft->print("ES:");
  tft->setCursor(8, 220);
  tft->print("CE:");
}

void TelemetryView::drawSessionInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->drawRect(2, 2, 156, 75, COLOR_CYAN);
  tft->setTextSize(1);
  tft->setTextColor(COLOR_CYAN);
  tft->setCursor(38, 8);
  tft->print("CONDITIONS");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(8, 25);
  tft->print("Weather:");
  tft->setCursor(8, 42);
  tft->print("Track:");
  tft->setCursor(8, 59);
  tft->print("Air:");

  tft->drawRect(162, 2, 156, 75, COLOR_MAGENTA);
  tft->setTextColor(COLOR_MAGENTA);
  tft->setCursor(205, 8);
  tft->print("SESSION");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(168, 25);
  tft->print("Type:");
  tft->setCursor(168, 37);
  tft->print("Lap:");
  tft->setCursor(168, 49);
  tft->print("Time:");
  tft->setCursor(168, 61);
  tft->print("SC:");

  tft->drawRect(2, 81, 316, 75, COLOR_ORANGE);
  tft->setTextColor(COLOR_ORANGE);
  tft->setCursor(88, 87);
  tft->print("LAP PERFORMANCE");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(8, 105);
  tft->print("Best Lap:");
  tft->setCursor(8, 127);
  tft->print("Last Lap:");

  tft->setCursor(165, 105);
  tft->print("Sector 1:");
  tft->setCursor(165, 127);
  tft->print("Sector 2:");

  tft->drawRect(2, 160, 316, 78, COLOR_YELLOW);
  tft->setTextColor(COLOR_YELLOW);
  tft->setCursor(85, 166);
  tft->print("VEHICLE STATUS");

  tft->setTextColor(COLOR_DARKGREY);
  tft->setCursor(8, 181);
  tft->print("Fuel:");
  tft->setCursor(8, 196);
  tft->print("Tyres:");
  tft->setCursor(8, 211);
  tft->print("Engine:");
  tft->setCursor(8, 226);
  tft->print("Damage:");
}

// ============================================
// RESET DIRTY TRACKING METHODS
// ============================================

void TelemetryView::resetGeneralDirtyTracking() {
  lastPosition = 255;
  lastDeltaFront = 65535;
  lastDeltaLeader = 65535;
  lastDeltaLive = -999.0f;
  lastLapTime = 0;
  lastCurrentLapTime = 0;
  lastSector1 = 65535;
  lastSector2 = 65535;
  lastCurrentLapNum = 255;
  lastCornerCuttingWarnings = 255;

  lastSpeed = 65535;
  lastThrottle = -999.0f;
  lastBrake = -999.0f;
  lastGear = -99;
  lastRPM = 65535;

  lastDRS = 255;
  lastRevLightsPercent = 255;
  lastSuggestedGear = -99;
  lastFrontBrakeBias = 255;
  lastDiffOnThrottle = 255;

  lastFuelInTank = -999.0f;
  lastFuelRemainingLaps = -999.0f;
  lastERSEnergy = -999.0f;
  lastERSMode = 255;

  lastLedsOn = 255;
}

void TelemetryView::resetTyreInfoDirtyTracking() {
  lastTyresAgeLaps = 255;

  lastBrakeTempFL = 65535;
  lastBrakeTempFR = 65535;
  lastBrakeTempRL = 65535;
  lastBrakeTempRR = 65535;

  lastTyreSurfTempFL = 255;
  lastTyreSurfTempFR = 255;
  lastTyreSurfTempRL = 255;
  lastTyreSurfTempRR = 255;

  lastTyreInnerTempFL = 255;
  lastTyreInnerTempFR = 255;
  lastTyreInnerTempRL = 255;
  lastTyreInnerTempRR = 255;

  lastTyrePressureFL = -1.0f;
  lastTyrePressureFR = -1.0f;
  lastTyrePressureRL = -1.0f;
  lastTyrePressureRR = -1.0f;

  lastTyreWearFL = -1.0f;
  lastTyreWearFR = -1.0f;
  lastTyreWearRL = -1.0f;
  lastTyreWearRR = -1.0f;

  lastTyreDamageFL = 255;
  lastTyreDamageFR = 255;
  lastTyreDamageRL = 255;
  lastTyreDamageRR = 255;

  lastBrakeDamageFL = 255;
  lastBrakeDamageFR = 255;
  lastBrakeDamageRL = 255;
  lastBrakeDamageRR = 255;
}

void TelemetryView::resetCarInfoDirtyTracking() {
  lastICEPower = -1.0f;
  lastMGUKPower = -1.0f;
  lastERSPercent = -1.0f;
  lastEngineTemp = 65535;
  lastDRSFault = 255;
  lastERSFault = 255;

  lastWingDamageFL = 255;
  lastWingDamageFR = 255;
  lastWingDamageRear = 255;
  lastFloorDamage = 255;
  lastDiffuserDamage = 255;
  lastSidepodDamage = 255;

  lastGearboxDamage = 255;
  lastICEDamage = 255;
  lastMGUHDamage = 255;
  lastMGUKDamage = 255;
  lastTCDamage = 255;
  lastERSDamage = 255;
  lastCEDamage = 255;
}

void TelemetryView::resetSessionInfoDirtyTracking() {
  lastWeather = 255;
  lastTrackTemp = -99;
  lastAirTemp = -99;

  lastSessionType = 255;
  lastCurrentLapInfo = 255;
  lastTotalLaps = 255;
  lastSessionTimeLeft = 65535;
  lastSafetyCarStatus = 255;

  lastBestLapTime = 0;
  lastLastLapTime = 0;
  lastSector1Time = 65535;
  lastSector2Time = 65535;

  lastFuelRemaining = -1.0f;
  lastTyresAge = 255;
  lastEngineTempCritical = 65535;
  lastOverallDamage = 255;
}

// ============================================
// UPDATE METHODS - GENERAL SCREEN
// ============================================

void TelemetryView::updatePosition() {
  uint8_t pos = model->getCarPosition();
  if (pos != lastPosition) {
    tft->fillRect(43, 1, 66, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_CYAN);

    uint8_t cursorX;
    if (pos < 10) {
      cursorX = 64;
    } else {
      cursorX = 58;
    }
    tft->setCursor(cursorX, 4);

    if (pos > 0) {
      tft->print("P");
      tft->print(pos);
    } else {
      tft->setCursor(64, 4);
      tft->print("--");
    }
    lastPosition = pos;
  }
}

void TelemetryView::updateDeltaFront() {
  uint16_t delta = model->getDeltaToCarInFrontMS();
  if (delta != lastDeltaFront) {
    tft->fillRect(43, 25, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);

    if (delta > 0 && delta < 30000) {
      tft->setCursor(54, 32);
      tft->print("+");
      tft->print(delta / 1000.0f, 2);
    } else {
      tft->setCursor(68, 32);
      tft->print("--");
    }
    lastDeltaFront = delta;
  }
}

void TelemetryView::updateDeltaLeader() {
  uint16_t delta = model->getDeltaToRaceLeaderMS();
  if (delta != lastDeltaLeader) {
    tft->fillRect(43, 49, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);

    if (delta == 0) {
      tft->setTextColor(COLOR_GREEN);
      tft->setCursor(60, 56);
      tft->print("LEAD");
    } else if (delta < 30000) {
      tft->setTextColor(COLOR_ORANGE);
      tft->setCursor(54, 56);
      tft->print("+");
      tft->print(delta / 1000.0f, 1);
    } else {
      tft->setTextColor(COLOR_DARKGREY);
      tft->setCursor(68, 56);
      tft->print("--");
    }
    lastDeltaLeader = delta;
  }
}

void TelemetryView::updateDeltaLive() {
  float delta = model->getDeltaLive();
  uint32_t bestLap = model->getBestLapTimeMS();
  float lapDist = model->getLapDistance();

  bool shouldUpdate = (abs(delta - lastDeltaLive) > 0.001f) || (lastDeltaLive == -999.0f);

  if (shouldUpdate) {
    tft->fillRect(43, 73, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);

    if (bestLap == 0 || lapDist < 10.0f) {
      tft->setTextColor(COLOR_DARKGREY);
      tft->setCursor(68, 80);
      tft->print("--");
    } else if (delta > 0.1f) {
      tft->setTextColor(COLOR_RED);
      tft->setCursor(50, 80);
      tft->print("+");
      tft->print(delta, 3);
    } else if (delta < -0.1f) {
      tft->setTextColor(COLOR_GREEN);
      tft->setCursor(54, 80);
      tft->print(delta, 3);
    } else {
      tft->setTextColor(COLOR_WHITE);
      tft->setCursor(54, 80);
      tft->print("0.000");
    }

    lastDeltaLive = delta;
  }
}

void TelemetryView::updateLastLapTime() {
  uint32_t time = model->getLastLapTimeMS();
  if (time != lastLapTime) {
    tft->fillRect(43, 97, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(52, 105);

    if (time > 0) {
      uint8_t mins = time / 60000;
      uint8_t secs = (time % 60000) / 1000;
      uint16_t ms = time % 1000;

      if (mins > 0) {
        tft->print(mins);
        tft->print(":");
        if (secs < 10) tft->print("0");
      }
      tft->print(secs);
      tft->print(".");
      if (ms < 100) tft->print("0");
      if (ms < 10) tft->print("0");
      tft->print(ms);
    } else {
      tft->print("--");
    }
    lastLapTime = time;
  }
}

void TelemetryView::updateSector1() {
  uint16_t time = model->getSector1TimeMS();
  if (time != lastSector1) {
    tft->fillRect(43, 121, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);

    if (time > 0) {
      tft->setCursor(54, 128);
      tft->print(time / 1000.0f, 3);
    } else {
      tft->setCursor(68, 128);
      tft->print("--");
    }
    lastSector1 = time;
  }
}

void TelemetryView::updateSector2() {
  uint16_t time = model->getSector2TimeMS();
  if (time != lastSector2) {
    tft->fillRect(43, 145, 66, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);

    if (time > 0) {
      tft->setCursor(54, 152);
      tft->print(time / 1000.0f, 3);
    } else {
      tft->setCursor(68, 152);
      tft->print("--");
    }
    lastSector2 = time;
  }
}

void TelemetryView::updateCurrentLapNum() {
  uint8_t lap = model->getCurrentLapNum();
  if (lap != lastCurrentLapNum) {
    tft->fillRect(43, 169, 66, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_WHITE);
    uint8_t cursorX = (lap < 10) ? 69 : 63;
    tft->setCursor(cursorX, 172);
    tft->print(lap);
    lastCurrentLapNum = lap;
  }
}

void TelemetryView::updateCornerCuttingWarnings() {
  uint8_t warnings = model->getCornerCuttingWarnings();
  if (warnings != lastCornerCuttingWarnings) {
    tft->fillRect(43, 193, 66, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(warnings > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(70, 199);
    tft->print(warnings);
    lastCornerCuttingWarnings = warnings;
  }
}

void TelemetryView::updateRPM() {
  uint16_t rpm = model->getEngineRPM();
  if (rpm != lastRPM) {
    tft->fillRect(113, 1, 74, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_YELLOW);
    uint8_t cursorX = (rpm < 10000) ? 130 : 122;
    tft->setCursor(cursorX, 6);
    tft->print(rpm);
    lastRPM = rpm;
  }
}

void TelemetryView::updateGear() {
  int8_t gear = model->getGear();
  if (gear != lastGear) {
    tft->fillRect(113, 25, 74, 94, COLOR_BLACK);
    tft->setTextSize(8);
    tft->setCursor(130, 45);

    if (gear == -1) {
      tft->setTextColor(COLOR_RED);
      tft->print("R");
    } else if (gear == 0) {
      tft->setTextColor(COLOR_WHITE);
      tft->print("N");
    } else {
      tft->setTextColor(COLOR_WHITE);
      tft->print(gear);
    }

    lastGear = gear;
  }
}

void TelemetryView::updateSpeed() {
  uint16_t speed = model->getSpeed();
  if (speed != lastSpeed) {
    tft->fillRect(113, 121, 74, 94, COLOR_BLACK);

    tft->setTextSize(3);
    tft->setTextColor(COLOR_WHITE);

    uint8_t cursorX;
    if (speed < 10) {
      cursorX = 142;
    } else if (speed < 100) {
      cursorX = 131;
    } else {
      cursorX = 122;
    }

    tft->setCursor(cursorX, 140);
    tft->print(speed);

    tft->setTextSize(1);
    tft->setTextColor(COLOR_DARKGREY);
    tft->setCursor(138, 180);
    tft->print("km/h");

    lastSpeed = speed;
  }
}

void TelemetryView::updateCurrentLapTime() {
  uint32_t time = model->getCurrentLapTimeMS();
  if (time != lastCurrentLapTime) {
    tft->fillRect(191, 1, 86, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(210, 8);
    if (time > 0) {
      char buffer[16];
      model->formatLapTime(time, buffer);
      tft->print(buffer);
    } else {
      tft->print("--:--.---");
    }
    lastCurrentLapTime = time;
  }
}

void TelemetryView::updateDRS() {
  uint8_t drs = model->getDRS();
  if (drs != lastDRS) {
    tft->fillRect(191, 25, 86, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(drs == 1 ? COLOR_GREEN : COLOR_DARKGREY);
    tft->setCursor(216, 30);
    tft->print(drs == 1 ? "DRS" : "---");
    lastDRS = drs;
  }
}

void TelemetryView::updateRevLights() {
  uint8_t rev = model->getRevLightsPercent();
  if (rev != lastRevLightsPercent) {
    tft->fillRect(191, 49, 86, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_ORANGE);
    uint8_t cursorX = (rev < 10) ? 224 : (rev < 100 ? 218 : 212);
    tft->setCursor(cursorX, 54);
    tft->print(rev);
    tft->print("%");
    lastRevLightsPercent = rev;
  }
}

void TelemetryView::updateSuggestedGear() {
  int8_t gear = model->getSuggestedGear();
  if (gear != lastSuggestedGear) {
    tft->fillRect(191, 73, 86, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_MAGENTA);
    uint8_t cursorX = (gear > 0) ? 228 : 224;
    tft->setCursor(cursorX, 78);
    if (gear > 0) tft->print(gear);
    else tft->print("--");
    lastSuggestedGear = gear;
  }
}

void TelemetryView::updateFrontBrakeBias() {
  uint8_t bias = model->getFrontBrakeBias();
  if (bias != lastFrontBrakeBias) {
    tft->fillRect(191, 97, 86, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_MAGENTA);
    uint8_t cursorX = (bias < 10) ? 224 : (bias < 100 ? 218 : 212);
    tft->setCursor(cursorX, 102);
    tft->print(bias);
    tft->print("%");
    lastFrontBrakeBias = bias;
  }
}

void TelemetryView::updateDiffOnThrottle() {
  uint8_t diff = model->getDiffOnThrottle();
  if (diff != lastDiffOnThrottle) {
    tft->fillRect(191, 121, 86, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_CYAN);
    uint8_t cursorX = (diff < 10) ? 224 : (diff < 100 ? 218 : 212);
    tft->setCursor(cursorX, 126);
    tft->print(diff);
    tft->print("%");
    lastDiffOnThrottle = diff;
  }
}

void TelemetryView::updateFuelInTank() {
  float fuel = model->getFuelInTank();
  if (abs(fuel - lastFuelInTank) > 0.1f) {
    tft->fillRect(191, 145, 86, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(215, 153);
    tft->print(fuel, 1);
    tft->print("kg");
    lastFuelInTank = fuel;
  }
}

void TelemetryView::updateFuelRemainingLaps() {
  float laps = model->getFuelRemainingLaps();
  if (abs(laps - lastFuelRemainingLaps) > 0.1f) {
    tft->fillRect(191, 169, 86, 22, COLOR_BLACK);
    tft->setTextSize(1);

    if (laps > 0.1f) {
      uint16_t color = laps < 1.0f ? COLOR_RED : (laps < 3.0f ? COLOR_YELLOW : COLOR_GREEN);
      tft->setTextColor(color);
      tft->setCursor(222, 177);
      tft->print(laps, 1);
    } else {
      tft->setTextColor(COLOR_DARKGREY);
      tft->setCursor(228, 177);
      tft->print("--");
    }
    lastFuelRemainingLaps = laps;
  }
}

void TelemetryView::updateERSMode() {
  uint8_t mode = model->getERSDeployMode();
  if (mode != lastERSMode) {
    tft->fillRect(191, 193, 86, 22, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(220, 201);
    switch (mode) {
      case 0: tft->print("NONE"); break;
      case 1: tft->print("MED"); break;
      case 2: tft->print("HOT"); break;
      case 3: tft->print("OVR"); break;
      default: tft->print("--"); break;
    }
    lastERSMode = mode;
  }
}

void TelemetryView::updateERSEnergy() {
  float ers = model->getERSPercent();
  if (abs(ers - lastERSEnergy) > 0.1f) {
    int barWidth = (int)((ers / 100.0f) * 234);

    if (lastERSEnergy < 0.0f) {
      tft->fillRect(43, 217, 234, 22, COLOR_BLACK);
      tft->fillRect(43, 217, barWidth, 22, COLOR_YELLOW);
    } else {
      int oldBarWidth = (int)((lastERSEnergy / 100.0f) * 234);

      if (barWidth > oldBarWidth) {
        tft->fillRect(43 + oldBarWidth, 217, barWidth - oldBarWidth, 22, COLOR_YELLOW);
      } else if (barWidth < oldBarWidth) {
        tft->fillRect(43 + barWidth, 217, oldBarWidth - barWidth, 22, COLOR_BLACK);
      }
    }

    lastERSEnergy = ers;
  }
}

void TelemetryView::updateThrottle() {
  float throttle = model->getThrottle();
  if (abs(throttle - lastThrottle) > 0.01f) {
    int barHeight = (int)(throttle * 238);

    if (lastThrottle < 0.0f) {
      tft->fillRect(281, 1, 38, 238, COLOR_BLACK);
      tft->fillRect(281, 239 - barHeight, 38, barHeight, COLOR_GREEN);
    } else {
      int oldBarHeight = (int)(lastThrottle * 238);

      if (barHeight > oldBarHeight) {
        tft->fillRect(281, 239 - barHeight, 38, barHeight - oldBarHeight, COLOR_GREEN);
      } else if (barHeight < oldBarHeight) {
        tft->fillRect(281, 239 - oldBarHeight, 38, oldBarHeight - barHeight, COLOR_BLACK);
      }
    }

    lastThrottle = throttle;
  }
}

void TelemetryView::updateBrake() {
  float brake = model->getBrake();
  if (abs(brake - lastBrake) > 0.01f) {
    int barHeight = (int)(brake * 238);

    if (lastBrake < 0.0f) {
      tft->fillRect(1, 1, 38, 238, COLOR_BLACK);
      tft->fillRect(1, 239 - barHeight, 38, barHeight, COLOR_RED);
    } else {
      int oldBarHeight = (int)(lastBrake * 238);

      if (barHeight > oldBarHeight) {
        tft->fillRect(1, 239 - barHeight, 38, barHeight - oldBarHeight, COLOR_RED);
      } else if (barHeight < oldBarHeight) {
        tft->fillRect(1, 239 - oldBarHeight, 38, oldBarHeight - barHeight, COLOR_BLACK);
      }
    }

    lastBrake = brake;
  }
}

void TelemetryView::updateLEDs() {
  uint8_t revPercent = model->getRevLightsPercent();
  int ledsOn = map(revPercent, 0, 100, 0, NUM_LEDS);

  if (ledsOn != lastLedsOn) {
    pixels->clear();
    for (int i = 0; i < ledsOn; i++) {
      if (i < LED_GREEN_COUNT) {
        pixels->setPixelColor(i, pixels->Color(0, 255, 0));
      } else if (i < LED_GREEN_COUNT + LED_YELLOW_COUNT) {
        pixels->setPixelColor(i, pixels->Color(255, 255, 0));
      } else {
        pixels->setPixelColor(i, pixels->Color(255, 0, 0));
      }
    }
    pixels->show();
    lastLedsOn = ledsOn;
  }
}

// ============================================
// UPDATE METHODS - TYRE INFO SCREEN
// ============================================

void TelemetryView::updateTyresAgeLaps() {
  uint8_t age = model->getTyresAgeLaps();
  if (age != lastTyresAgeLaps) {
    tft->fillRect(1, 1, 318, 22, COLOR_BLACK);
    tft->setTextSize(2);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(90, 4);
    tft->print("AGE: ");
    tft->print(age);
    tft->print(" LAPS");
    lastTyresAgeLaps = age;
  }
}

void TelemetryView::updateBrakeTemps() {
  uint16_t fl = model->getBrakeTemp(2);
  uint16_t fr = model->getBrakeTemp(3);
  uint16_t rl = model->getBrakeTemp(0);
  uint16_t rr = model->getBrakeTemp(1);

  if (fl != lastBrakeTempFL) {
    tft->fillRect(5, 40, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 600 ? COLOR_RED : COLOR_ORANGE);
    tft->setCursor(5, 40);
    tft->print("Brake: ");
    tft->print(fl);
    tft->print("C");
    lastBrakeTempFL = fl;
  }

  if (fr != lastBrakeTempFR) {
    tft->fillRect(165, 40, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 600 ? COLOR_RED : COLOR_ORANGE);
    tft->setCursor(165, 40);
    tft->print("Brake: ");
    tft->print(fr);
    tft->print("C");
    lastBrakeTempFR = fr;
  }

  if (rl != lastBrakeTempRL) {
    tft->fillRect(5, 148, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rl > 600 ? COLOR_RED : COLOR_ORANGE);
    tft->setCursor(5, 148);
    tft->print("Brake: ");
    tft->print(rl);
    tft->print("C");
    lastBrakeTempRL = rl;
  }

  if (rr != lastBrakeTempRR) {
    tft->fillRect(165, 148, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rr > 600 ? COLOR_RED : COLOR_ORANGE);
    tft->setCursor(165, 148);
    tft->print("Brake: ");
    tft->print(rr);
    tft->print("C");
    lastBrakeTempRR = rr;
  }
}

void TelemetryView::updateTyreSurfaceTemps() {
  uint8_t fl = model->getTyreSurfaceTemp(2);
  uint8_t fr = model->getTyreSurfaceTemp(3);
  uint8_t rl = model->getTyreSurfaceTemp(0);
  uint8_t rr = model->getTyreSurfaceTemp(1);

  if (fl != lastTyreSurfTempFL) {
    tft->fillRect(5, 52, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(5, 52);
    tft->print("Surf:  ");
    tft->print(fl);
    tft->print("C");
    lastTyreSurfTempFL = fl;
  }

  if (fr != lastTyreSurfTempFR) {
    tft->fillRect(165, 52, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(165, 52);
    tft->print("Surf:  ");
    tft->print(fr);
    tft->print("C");
    lastTyreSurfTempFR = fr;
  }

  if (rl != lastTyreSurfTempRL) {
    tft->fillRect(5, 160, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(5, 160);
    tft->print("Surf:  ");
    tft->print(rl);
    tft->print("C");
    lastTyreSurfTempRL = rl;
  }

  if (rr != lastTyreSurfTempRR) {
    tft->fillRect(165, 160, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(165, 160);
    tft->print("Surf:  ");
    tft->print(rr);
    tft->print("C");
    lastTyreSurfTempRR = rr;
  }
}

void TelemetryView::updateTyreInnerTemps() {
  uint8_t fl = model->getTyreInnerTemp(2);
  uint8_t fr = model->getTyreInnerTemp(3);
  uint8_t rl = model->getTyreInnerTemp(0);
  uint8_t rr = model->getTyreInnerTemp(1);

  if (fl != lastTyreInnerTempFL) {
    tft->fillRect(5, 64, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(5, 64);
    tft->print("Inner: ");
    tft->print(fl);
    tft->print("C");
    lastTyreInnerTempFL = fl;
  }

  if (fr != lastTyreInnerTempFR) {
    tft->fillRect(165, 64, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(165, 64);
    tft->print("Inner: ");
    tft->print(fr);
    tft->print("C");
    lastTyreInnerTempFR = fr;
  }

  if (rl != lastTyreInnerTempRL) {
    tft->fillRect(5, 172, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(5, 172);
    tft->print("Inner: ");
    tft->print(rl);
    tft->print("C");
    lastTyreInnerTempRL = rl;
  }

  if (rr != lastTyreInnerTempRR) {
    tft->fillRect(165, 172, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(165, 172);
    tft->print("Inner: ");
    tft->print(rr);
    tft->print("C");
    lastTyreInnerTempRR = rr;
  }
}

void TelemetryView::updateTyrePressures() {
  float fl = model->getTyrePressure(2);
  float fr = model->getTyrePressure(3);
  float rl = model->getTyrePressure(0);
  float rr = model->getTyrePressure(1);

  if (abs(fl - lastTyrePressureFL) > 0.01f) {
    tft->fillRect(5, 76, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(5, 76);
    tft->print("Press: ");
    tft->print(fl, 2);
    lastTyrePressureFL = fl;
  }

  if (abs(fr - lastTyrePressureFR) > 0.01f) {
    tft->fillRect(165, 76, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(165, 76);
    tft->print("Press: ");
    tft->print(fr, 2);
    lastTyrePressureFR = fr;
  }

  if (abs(rl - lastTyrePressureRL) > 0.01f) {
    tft->fillRect(5, 184, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(5, 184);
    tft->print("Press: ");
    tft->print(rl, 2);
    lastTyrePressureRL = rl;
  }

  if (abs(rr - lastTyrePressureRR) > 0.01f) {
    tft->fillRect(165, 184, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(165, 184);
    tft->print("Press: ");
    tft->print(rr, 2);
    lastTyrePressureRR = rr;
  }
}

void TelemetryView::updateTyreWear() {
  float fl = model->getTyreWear(2);
  float fr = model->getTyreWear(3);
  float rl = model->getTyreWear(0);
  float rr = model->getTyreWear(1);

  if (abs(fl - lastTyreWearFL) > 0.1f) {
    tft->fillRect(5, 88, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(5, 88);
    tft->print("Wear:  ");
    tft->print((int)fl);
    tft->print("%");
    lastTyreWearFL = fl;
  }

  if (abs(fr - lastTyreWearFR) > 0.1f) {
    tft->fillRect(165, 88, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(165, 88);
    tft->print("Wear:  ");
    tft->print((int)fr);
    tft->print("%");
    lastTyreWearFR = fr;
  }

  if (abs(rl - lastTyreWearRL) > 0.1f) {
    tft->fillRect(5, 196, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(5, 196);
    tft->print("Wear:  ");
    tft->print((int)rl);
    tft->print("%");
    lastTyreWearRL = rl;
  }

  if (abs(rr - lastTyreWearRR) > 0.1f) {
    tft->fillRect(165, 196, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(165, 196);
    tft->print("Wear:  ");
    tft->print((int)rr);
    tft->print("%");
    lastTyreWearRR = rr;
  }
}

void TelemetryView::updateTyreDamage() {
  uint8_t fl = model->getTyreDamage(2);
  uint8_t fr = model->getTyreDamage(3);
  uint8_t rl = model->getTyreDamage(0);
  uint8_t rr = model->getTyreDamage(1);

  if (fl != lastTyreDamageFL) {
    tft->fillRect(5, 100, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(5, 100);
    tft->print("T.Dmg: ");
    tft->print(fl);
    tft->print("%");
    lastTyreDamageFL = fl;
  }

  if (fr != lastTyreDamageFR) {
    tft->fillRect(165, 100, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(165, 100);
    tft->print("T.Dmg: ");
    tft->print(fr);
    tft->print("%");
    lastTyreDamageFR = fr;
  }

  if (rl != lastTyreDamageRL) {
    tft->fillRect(5, 208, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(5, 208);
    tft->print("T.Dmg: ");
    tft->print(rl);
    tft->print("%");
    lastTyreDamageRL = rl;
  }

  if (rr != lastTyreDamageRR) {
    tft->fillRect(165, 208, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(165, 208);
    tft->print("T.Dmg: ");
    tft->print(rr);
    tft->print("%");
    lastTyreDamageRR = rr;
  }
}

void TelemetryView::updateBrakeDamage() {
  uint8_t fl = model->getBrakeDamage(2);
  uint8_t fr = model->getBrakeDamage(3);
  uint8_t rl = model->getBrakeDamage(0);
  uint8_t rr = model->getBrakeDamage(1);

  if (fl != lastBrakeDamageFL) {
    tft->fillRect(5, 112, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(5, 112);
    tft->print("B.Dmg: ");
    tft->print(fl);
    tft->print("%");
    lastBrakeDamageFL = fl;
  }

  if (fr != lastBrakeDamageFR) {
    tft->fillRect(165, 112, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(165, 112);
    tft->print("B.Dmg: ");
    tft->print(fr);
    tft->print("%");
    lastBrakeDamageFR = fr;
  }

  if (rl != lastBrakeDamageRL) {
    tft->fillRect(5, 220, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(5, 220);
    tft->print("B.Dmg: ");
    tft->print(rl);
    tft->print("%");
    lastBrakeDamageRL = rl;
  }

  if (rr != lastBrakeDamageRR) {
    tft->fillRect(165, 220, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(165, 220);
    tft->print("B.Dmg: ");
    tft->print(rr);
    tft->print("%");
    lastBrakeDamageRR = rr;
  }
}

// ============================================
// UPDATE METHODS - CAR DAMAGE SCREEN
// ============================================

void TelemetryView::updatePowerUnit() {
  float icePower = model->getEnginePowerICE();
  float mgukPower = model->getEnginePowerMGUK();
  float ersPercent = model->getERSPercent();
  uint16_t engineTemp = model->getEngineTemp();
  uint8_t drsFault = model->getDRSFault();
  uint8_t ersFault = model->getERSFault();

  if (abs(icePower - lastICEPower) > 1.0f) {
    tft->fillRect(60, 25, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(60, 25);
    tft->print((int)icePower);
    tft->print(" kW");
    lastICEPower = icePower;
  }

  if (abs(mgukPower - lastMGUKPower) > 1.0f) {
    tft->fillRect(60, 40, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(60, 40);
    tft->print((int)mgukPower);
    tft->print(" kW");
    lastMGUKPower = mgukPower;
  }

  if (abs(ersPercent - lastERSPercent) > 1.0f) {
    tft->fillRect(60, 55, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    uint16_t color = ersPercent < 20 ? COLOR_RED : (ersPercent < 50 ? COLOR_YELLOW : COLOR_GREEN);
    tft->setTextColor(color);
    tft->setCursor(60, 55);
    tft->print((int)ersPercent);
    tft->print("%");
    lastERSPercent = ersPercent;
  }

  if (engineTemp != lastEngineTemp) {
    tft->fillRect(60, 70, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(engineTemp >= 160 ? COLOR_RED : (engineTemp >= 140 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(60, 70);
    tft->print(engineTemp);
    tft->print("C");
    lastEngineTemp = engineTemp;
  }

  if (drsFault != lastDRSFault || ersFault != lastERSFault) {
    tft->fillRect(60, 85, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    if (drsFault == 1 || ersFault == 1) {
      tft->setTextColor(COLOR_RED);
      tft->setCursor(60, 85);
      if (drsFault == 1) tft->print("DRS FAULT");
      else tft->print("ERS FAULT");
    } else {
      tft->setTextColor(COLOR_GREEN);
      tft->setCursor(60, 85);
      tft->print("OK");
    }
    lastDRSFault = drsFault;
    lastERSFault = ersFault;
  }
}

void TelemetryView::updateAeroDamage() {
  uint8_t fl = model->getFrontLeftWingDamage();
  uint8_t fr = model->getFrontRightWingDamage();
  uint8_t rear = model->getRearWingDamage();
  uint8_t floor = model->getFloorDamage();
  uint8_t diffuser = model->getDiffuserDamage();
  uint8_t sidepod = model->getSidepodDamage();

  if (fl != lastWingDamageFL) {
    tft->fillRect(240, 25, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 40 ? COLOR_RED : (fl > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 25);
    tft->print(fl);
    tft->print("%");
    lastWingDamageFL = fl;
  }

  if (fr != lastWingDamageFR) {
    tft->fillRect(240, 37, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 40 ? COLOR_RED : (fr > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 37);
    tft->print(fr);
    tft->print("%");
    lastWingDamageFR = fr;
  }

  if (rear != lastWingDamageRear) {
    tft->fillRect(240, 49, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rear > 40 ? COLOR_RED : (rear > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 49);
    tft->print(rear);
    tft->print("%");
    lastWingDamageRear = rear;
  }

  if (floor != lastFloorDamage) {
    tft->fillRect(240, 61, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(floor > 40 ? COLOR_RED : (floor > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 61);
    tft->print(floor);
    tft->print("%");
    lastFloorDamage = floor;
  }

  if (diffuser != lastDiffuserDamage) {
    tft->fillRect(240, 73, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(diffuser > 40 ? COLOR_RED : (diffuser > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 73);
    tft->print(diffuser);
    tft->print("%");
    lastDiffuserDamage = diffuser;
  }

  if (sidepod != lastSidepodDamage) {
    tft->fillRect(240, 85, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(sidepod > 40 ? COLOR_RED : (sidepod > 20 ? COLOR_YELLOW : COLOR_GREEN));
    tft->setCursor(240, 85);
    tft->print(sidepod);
    tft->print("%");
    lastSidepodDamage = sidepod;
  }
}

void TelemetryView::updateComponentWear() {
  uint8_t gearbox = model->getGearBoxDamage();
  uint8_t ice = model->getEngineICEWear();
  uint8_t mguh = model->getEngineMGUHWear();
  uint8_t mguk = model->getEngineMGUKWear();
  uint8_t tc = model->getEngineTCWear();
  uint8_t es = model->getEngineESWear();
  uint8_t ce = model->getEngineCEWear();

  auto drawWearBar = [this](uint8_t wear, uint8_t lastWear, int y) -> uint8_t {
    if (wear != lastWear) {
      tft->fillRect(70, y, 240, 10, COLOR_BLACK);
      tft->drawRect(70, y + 2, 150, 6, COLOR_DARKGREY);

      int barWidth = (wear * 150) / 100;
      uint16_t color = wear > 90 ? COLOR_RED : (wear > 70 ? COLOR_YELLOW : COLOR_GREEN);
      tft->fillRect(71, y + 3, barWidth, 4, color);

      tft->setTextSize(1);
      tft->setTextColor(color);
      tft->setCursor(230, y);
      tft->print(wear);
      tft->print("%");
    }
    return wear;
  };

  lastGearboxDamage = drawWearBar(gearbox, lastGearboxDamage, 130);
  lastICEDamage = drawWearBar(ice, lastICEDamage, 145);
  lastMGUHDamage = drawWearBar(mguh, lastMGUHDamage, 160);
  lastMGUKDamage = drawWearBar(mguk, lastMGUKDamage, 175);
  lastTCDamage = drawWearBar(tc, lastTCDamage, 190);
  lastERSDamage = drawWearBar(es, lastERSDamage, 205);
  lastCEDamage = drawWearBar(ce, lastCEDamage, 220);
}

// ============================================
// UPDATE METHODS - SCREEN 4: RACE OVERVIEW
// ============================================


void TelemetryView::updateWeather() {
  uint8_t weather = model->getWeather();
  if (weather != lastWeather) {
    tft->fillRect(70, 25, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);

    const char* weatherText;
    uint16_t color;
    switch (weather) {
      case 0:
        weatherText = "CLEAR";
        color = COLOR_CYAN;
        break;
      case 1:
        weatherText = "LT CLOUD";
        color = COLOR_WHITE;
        break;
      case 2:
        weatherText = "OVERCAST";
        color = COLOR_DARKGREY;
        break;
      case 3:
        weatherText = "LT RAIN";
        color = COLOR_YELLOW;
        break;
      case 4:
        weatherText = "HVY RAIN";
        color = COLOR_YELLOW;
        break;
      case 5:
        weatherText = "STORM";
        color = COLOR_RED;
        break;
      default:
        weatherText = "UNKNOWN";
        color = COLOR_WHITE;
        break;
    }

    tft->setTextColor(color);
    tft->setCursor(70, 25);
    tft->print(weatherText);
    lastWeather = weather;
  }
}

void TelemetryView::updateTrackTemp() {
  int8_t trackTemp = model->getTrackTemperature();
  if (trackTemp != lastTrackTemp) {
    tft->fillRect(70, 40, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    uint16_t color = trackTemp > 40 ? COLOR_RED : (trackTemp > 30 ? COLOR_YELLOW : COLOR_GREEN);
    tft->setTextColor(color);
    tft->setCursor(70, 40);
    tft->print(trackTemp);
    tft->print("C");
    lastTrackTemp = trackTemp;
  }
}

void TelemetryView::updateAirTemp() {
  int8_t airTemp = model->getAirTemperature();
  if (airTemp != lastAirTemp) {
    tft->fillRect(70, 55, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    uint16_t color = airTemp > 35 ? COLOR_RED : (airTemp > 25 ? COLOR_YELLOW : COLOR_GREEN);
    tft->setTextColor(color);
    tft->setCursor(70, 55);
    tft->print(airTemp);
    tft->print("C");
    lastAirTemp = airTemp;
  }
}


void TelemetryView::updateSessionType() {
  uint8_t type = model->getSessionType();
  if (type != lastSessionType) {
    tft->fillRect(210, 25, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(210, 25);

    switch (type) {
      case 0: tft->print("UNKNOWN"); break;
      case 1: tft->print("FP1"); break;
      case 2: tft->print("FP2"); break;
      case 3: tft->print("FP3"); break;
      case 4: tft->print("FP SHORT"); break;
      case 5: tft->print("Q1"); break;
      case 6: tft->print("Q2"); break;
      case 7: tft->print("Q3"); break;
      case 8: tft->print("Q SHORT"); break;
      case 9: tft->print("OSQ"); break;
      case 10: tft->print("RACE"); break;
      case 11: tft->print("RACE 2"); break;
      case 12: tft->print("RACE 3"); break;
      case 13: tft->print("TIME TRIAL"); break;
      default: tft->print("SESSION"); break;
    }
    lastSessionType = type;
  }
}

void TelemetryView::updateLapInfo() {
  uint8_t currentLap = model->getCurrentLapNum();
  uint8_t totalLaps = model->getTotalLaps();

  if (currentLap != lastCurrentLapInfo || totalLaps != lastTotalLaps) {
    tft->fillRect(210, 37, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(210, 37);
    tft->print(currentLap);
    tft->print(" / ");
    tft->print(totalLaps);
    lastCurrentLapInfo = currentLap;
    lastTotalLaps = totalLaps;
  }
}

void TelemetryView::updateSessionTimeLeft() {
  uint16_t timeLeft = model->getSessionTimeLeft();
  if (timeLeft != lastSessionTimeLeft) {
    tft->fillRect(210, 49, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(timeLeft < 300 ? COLOR_RED : COLOR_WHITE);
    tft->setCursor(210, 49);

    uint16_t minutes = timeLeft / 60;
    uint16_t seconds = timeLeft % 60;

    if (minutes < 10) tft->print("0");
    tft->print(minutes);
    tft->print(":");
    if (seconds < 10) tft->print("0");
    tft->print(seconds);

    lastSessionTimeLeft = timeLeft;
  }
}

void TelemetryView::updateSafetyCarStatus() {
  uint8_t status = model->getSafetyCarStatus();
  if (status != lastSafetyCarStatus) {
    tft->fillRect(190, 61, 120, 10, COLOR_BLACK);
    tft->setTextSize(1);

    const char* statusText;
    uint16_t color;
    switch (status) {
      case 0:
        statusText = "NO";
        color = COLOR_GREEN;
        break;
      case 1:
        statusText = "FULL";
        color = COLOR_YELLOW;
        break;
      case 2:
        statusText = "VIRTUAL";
        color = COLOR_YELLOW;
        break;
      case 3:
        statusText = "FORMATION";
        color = COLOR_CYAN;
        break;
      default:
        statusText = "NO";
        color = COLOR_GREEN;
        break;
    }

    tft->setTextColor(color);
    tft->setCursor(210, 61);
    tft->print(statusText);
    lastSafetyCarStatus = status;
  }
}

void TelemetryView::updateBestLap() {
  uint32_t bestLap = model->getBestLapTimeMS();
  if (bestLap != lastBestLapTime) {
    tft->fillRect(8, 115, 150, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(8, 115);

    if (bestLap > 0) {
      char buffer[16];
      model->formatLapTime(bestLap, buffer);
      tft->print(buffer);
    } else {
      tft->print("--:--.---");
    }
    lastBestLapTime = bestLap;
  }
}

void TelemetryView::updateLastLap() {
  uint32_t lastLap = model->getLastLapTimeMS();
  uint32_t bestLap = model->getBestLapTimeMS();

  if (lastLap != lastLastLapTime) {
    tft->fillRect(8, 137, 150, 17, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setCursor(8, 137);

    if (lastLap > 0) {
      char buffer[16];
      model->formatLapTime(lastLap, buffer);

      if (bestLap > 0 && lastLap >= bestLap) {
        uint32_t delta = lastLap - bestLap;
        tft->setTextColor(COLOR_YELLOW);
        tft->print(buffer);
        tft->setCursor(8, 147);
        tft->print("+");
        tft->print(delta / 1000.0f, 3);
      } else if (bestLap > 0 && lastLap < bestLap) {
        tft->setTextColor(COLOR_GREEN);
        tft->print(buffer);
        tft->setCursor(8, 147);
        tft->print("BEST!");
      } else {
        tft->setTextColor(COLOR_WHITE);
        tft->print(buffer);
      }
    } else {
      tft->setTextColor(COLOR_DARKGREY);
      tft->print("--:--.---");
    }
    lastLastLapTime = lastLap;
  }
}

void TelemetryView::updateSectorTimes() {
  uint16_t sector1 = model->getSector1TimeMS();
  uint16_t sector2 = model->getSector2TimeMS();

  if (sector1 != lastSector1Time) {
    tft->fillRect(165, 115, 145, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(165, 115);
    if (sector1 > 0) {
      tft->print(sector1 / 1000.0f, 3);
      tft->print("s");
    } else {
      tft->print("--");
    }
    lastSector1Time = sector1;
  }

  if (sector2 != lastSector2Time) {
    tft->fillRect(165, 137, 145, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(165, 137);
    if (sector2 > 0) {
      tft->print(sector2 / 1000.0f, 3);
      tft->print("s");
    } else {
      tft->print("--");
    }
    lastSector2Time = sector2;
  }
}


void TelemetryView::updateFuelStatus() {
  float fuelLaps = model->getFuelRemainingLaps();
  if (abs(fuelLaps - lastFuelRemaining) > 0.1f) {
    tft->fillRect(55, 181, 255, 10, COLOR_BLACK);

    tft->drawRect(55, 182, 150, 8, COLOR_DARKGREY);

    int barWidth = (int)((fuelLaps / 20.0f) * 150);
    if (barWidth > 150) barWidth = 150;

    uint16_t color = fuelLaps < 3 ? COLOR_RED : (fuelLaps < 5 ? COLOR_YELLOW : COLOR_GREEN);
    if (barWidth > 0) {
      tft->fillRect(56, 183, barWidth, 6, color);
    }

    tft->setTextSize(1);
    tft->setTextColor(color);
    tft->setCursor(210, 181);
    tft->print(fuelLaps, 1);
    tft->print(" laps");

    lastFuelRemaining = fuelLaps;
  }
}

void TelemetryView::updateTyreStatus() {
  uint8_t tyreAge = model->getTyresAgeLaps();
  if (tyreAge != lastTyresAge) {
    tft->fillRect(55, 196, 255, 10, COLOR_BLACK);

    tft->drawRect(55, 197, 150, 8, COLOR_DARKGREY);

    int barWidth = (int)((tyreAge / 30.0f) * 150);
    if (barWidth > 150) barWidth = 150;

    uint16_t color = tyreAge > 25 ? COLOR_RED : (tyreAge > 15 ? COLOR_YELLOW : COLOR_GREEN);
    if (barWidth > 0) {
      tft->fillRect(56, 198, barWidth, 6, color);
    }

    tft->setTextSize(1);
    tft->setTextColor(color);
    tft->setCursor(210, 196);
    tft->print(tyreAge);
    tft->print(" laps");

    lastTyresAge = tyreAge;
  }
}

void TelemetryView::updateEngineStatus() {
  uint16_t engineTemp = model->getEngineTemp();
  if (engineTemp != lastEngineTempCritical) {
    tft->fillRect(55, 211, 255, 10, COLOR_BLACK);

    tft->drawRect(55, 212, 150, 8, COLOR_DARKGREY);

    int barWidth = (int)(((engineTemp - 80) / 100.0f) * 150);
    if (barWidth < 0) barWidth = 0;
    if (barWidth > 150) barWidth = 150;

    uint16_t color = engineTemp >= 160 ? COLOR_RED : (engineTemp >= 140 ? COLOR_YELLOW : COLOR_GREEN);
    if (barWidth > 0) {
      tft->fillRect(56, 213, barWidth, 6, color);
    }

    tft->setTextSize(1);
    tft->setTextColor(color);
    tft->setCursor(210, 211);
    tft->print(engineTemp);
    tft->print("C");

    lastEngineTempCritical = engineTemp;
  }
}

void TelemetryView::updateDamageStatus() {
  uint8_t fl = model->getFrontLeftWingDamage();
  uint8_t fr = model->getFrontRightWingDamage();
  uint8_t floor = model->getFloorDamage();
  uint8_t diffuser = model->getDiffuserDamage();

  uint8_t totalDamage = (fl + fr + floor + diffuser) / 4;

  if (totalDamage != lastOverallDamage) {
    tft->fillRect(55, 226, 255, 10, COLOR_BLACK);

    tft->drawRect(55, 227, 150, 8, COLOR_DARKGREY);

    int barWidth = (int)((totalDamage / 100.0f) * 150);
    if (barWidth > 150) barWidth = 150;

    uint16_t color = totalDamage > 50 ? COLOR_RED : (totalDamage > 25 ? COLOR_YELLOW : COLOR_GREEN);
    if (barWidth > 0) {
      tft->fillRect(56, 228, barWidth, 6, color);
    }

    tft->setTextSize(1);
    tft->setTextColor(color);
    tft->setCursor(210, 226);
    tft->print(totalDamage);
    tft->print("%");

    lastOverallDamage = totalDamage;
  }
}

// ============================================
// BOOT SCREEN
// ============================================

void TelemetryView::drawBootAnimation(uint32_t elapsedMS) {
  static bool initialized = false;
  static uint8_t lastBrightness = 0;

  if (!initialized) {
    tft->fillScreen(COLOR_BLACK);
    initialized = true;
  }

  uint8_t brightness = map(elapsedMS, 0, 2000, 0, 255);
  if (brightness > 255) brightness = 255;

  if (abs(brightness - lastBrightness) < 8) return;
  lastBrightness = brightness;

  uint16_t fadedRed = tft->color565(brightness, 0, 0);

  tft->drawBitmap(10, 80, F1_LOGO, 300, 75, fadedRed);
}

void TelemetryView::resetBootInfo() {
  tft->fillScreen(COLOR_BLACK);
  bootInfoDrawn = false;

  pixels->clear();
  pixels->show();
  lastLedsOn = 255;
}

void TelemetryView::drawBootInfo(IPAddress ip) {
  static IPAddress lastIP = IPAddress(0, 0, 0, 0);

  if (ip != lastIP || !bootInfoDrawn) {
    tft->fillScreen(COLOR_BLACK);
    tft->drawBitmap(10, 40, F1_LOGO, 300, 75, COLOR_RED);

    tft->setTextSize(2);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(60, 140);
    tft->print("IP: ");
    tft->print(ip);

    bootInfoDrawn = true;
    lastIP = ip;
  }

  static uint32_t lastDotUpdate = 0;
  static uint8_t lastDotCount = 0;
  uint32_t now = millis();

  if (now - lastDotUpdate > 500) {
    uint8_t dotCount = ((now / 500) % 4);

    if (dotCount != lastDotCount) {
      tft->fillRect(30, 170, 260, 20, COLOR_BLACK);

      tft->setTextSize(2);
      tft->setTextColor(COLOR_YELLOW);
      tft->setCursor(30, 170);
      tft->print("Waiting for data");

      for (uint8_t i = 0; i < dotCount; i++) {
        tft->print(".");
      }

      lastDotCount = dotCount;
    }

    lastDotUpdate = now;
  }
}