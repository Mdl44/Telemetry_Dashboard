#include "View.h"

TelemetryView::TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m) {
  tft = display;
  pixels = leds;
  model = m;

  // ============================================
  // Screen Management
  // ============================================
  currentScreen = SCREEN_GENERAL;
  screenChanged = true;
  bootInfoDrawn = false;

  // ============================================
  // Initialize Dirty Tracking - General Screen
  // ============================================
  lastPosition = 255;
  lastDeltaFront = 65535;
  lastDeltaLeader = 65535;
  lastSafetyCarDelta = -999.0f;
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
  // Initialize Dirty Tracking - Car Info Screen
  // ============================================
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
  lastEngineTemp = 65535;
  lastTyrePressureFL = -999.0f;
  lastTyrePressureFR = -999.0f;
  lastTyrePressureRL = -999.0f;
  lastTyrePressureRR = -999.0f;
  lastTyresAgeLaps = 255;

  lastTyreWearFL = -999.0f;
  lastTyreWearFR = -999.0f;
  lastTyreWearRL = -999.0f;
  lastTyreWearRR = -999.0f;
  lastTyreDamageFL = 255;
  lastTyreDamageFR = 255;
  lastTyreDamageRL = 255;
  lastTyreDamageRR = 255;
  lastBrakeDamageFL = 255;
  lastBrakeDamageFR = 255;
  lastBrakeDamageRL = 255;
  lastBrakeDamageRR = 255;
  lastFrontLeftWingDamage = 255;
  lastFrontRightWingDamage = 255;
  lastRearWingDamage = 255;
  lastFloorDamage = 255;
  lastDiffuserDamage = 255;
  lastSidepodDamage = 255;
  lastDRSFault = 255;
  lastERSFault = 255;
  lastGearBoxDamage = 255;
  lastEngineDamage = 255;
  lastEngineMGUHWear = 255;
  lastEngineESWear = 255;
  lastEngineCEWear = 255;
  lastEngineICEWear = 255;
  lastEngineMGUKWear = 255;
  lastEngineTCWear = 255;
  lastEngineBlown = 255;
  lastEngineSeized = 255;

  // ============================================
  // Initialize Dirty Tracking - Session Info Screen
  // ============================================
  lastWeather = 255;
  lastTrackTemp = -99;
  lastAirTemp = -99;
  lastSessionType = 255;
  lastSessionTimeLeft = 65535;
  lastSafetyCarStatus = 255;
  lastPitStopWindowIdealLap = 255;
  lastTotalLaps = 255;
}

void TelemetryView::init() {
  tft->begin();
  tft->setRotation(DISPLAY_ROTATION);
  tft->fillScreen(COLOR_BLACK);

  pixels->begin();
  pixels->setBrightness(LED_BRIGHTNESS_DEFAULT);
  pixels->clear();
  pixels->show();
}

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
      updateSafetyCarDelta();
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
      updateBrakeTemps();
      updateTyreSurfaceTemps();
      updateTyreInnerTemps();
      updateTyrePressures();
      updateTyresAgeLaps();
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
      updateEngineTemp();
      updateWingDamage();
      updateAeroDamage();
      updatePowertrainDamage();
      updateCriticalDamage();
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
      updateSessionTimeLeft();
      updateSafetyCarStatus();
      updatePitStopWindow();
      updateTotalLaps();
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

void TelemetryView::previousScreen() {
  currentScreen = (Screen)((currentScreen + 3) % 4);
  screenChanged = true;
}

// ============================================
// SCREEN DRAWING METHODS
// ============================================

void TelemetryView::drawGeneralScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->fillRect(0, 0, 320, 20, COLOR_RED);
  tft->setTextSize(2);
  tft->setTextColor(COLOR_WHITE);
  tft->setCursor(60, 3);
  tft->print("GENERAL (1/4)");

  tft->setTextSize(1);
  tft->setTextColor(COLOR_DARKGREY);

  int y = 25;
  tft->setCursor(5, y);
  tft->print("Position:");
  tft->setCursor(5, y += 15);
  tft->print("Gap Front:");
  tft->setCursor(5, y += 15);
  tft->print("Gap Leader:");
  tft->setCursor(5, y += 15);
  tft->print("Safety Delta:");
  tft->setCursor(5, y += 15);
  tft->print("Last Lap:");
  tft->setCursor(5, y += 15);
  tft->print("Curr Lap:");
  tft->setCursor(5, y += 15);
  tft->print("Sector 1:");
  tft->setCursor(5, y += 15);
  tft->print("Sector 2:");
  tft->setCursor(5, y += 15);
  tft->print("Lap Num:");
  tft->setCursor(5, y += 15);
  tft->print("Warnings:");

  y = 25;
  tft->setCursor(165, y);
  tft->print("Speed:");
  tft->setCursor(165, y += 15);
  tft->print("Throttle:");
  tft->setCursor(165, y += 15);
  tft->print("Brake:");
  tft->setCursor(165, y += 15);
  tft->print("Gear:");
  tft->setCursor(165, y += 15);
  tft->print("RPM:");
  tft->setCursor(165, y += 15);
  tft->print("DRS:");
  tft->setCursor(165, y += 15);
  tft->print("Rev:");
  tft->setCursor(165, y += 15);
  tft->print("Sug Gear:");
  tft->setCursor(165, y += 15);
  tft->print("Brake Bias:");
  tft->setCursor(165, y += 15);
  tft->print("Diff:");
  tft->setCursor(165, y += 15);
  tft->print("Fuel Tank:");
  tft->setCursor(165, y += 15);
  tft->print("Fuel Laps:");
  tft->setCursor(165, y += 15);
  tft->print("ERS:");
  tft->setCursor(165, y += 15);
  tft->print("ERS Mode:");
}

void TelemetryView::drawTyreInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->fillRect(0, 0, 320, 20, COLOR_CYAN);
  tft->setTextSize(2);
  tft->setTextColor(COLOR_BLACK);
  tft->setCursor(30, 3);
  tft->print("TYRE & BRAKE (2/4)");

  tft->setTextSize(1);
  tft->setTextColor(COLOR_DARKGREY);

  tft->setCursor(5, 25);
  tft->print("--- FRONT LEFT ---");
  tft->setCursor(5, 37);
  tft->print("Brake:");
  tft->setCursor(5, 49);
  tft->print("Surf:");
  tft->setCursor(5, 61);
  tft->print("Inner:");
  tft->setCursor(5, 73);
  tft->print("Press:");
  tft->setCursor(5, 85);
  tft->print("Wear:");
  tft->setCursor(5, 97);
  tft->print("Dmg T:");
  tft->setCursor(5, 109);
  tft->print("Dmg B:");

  tft->setCursor(165, 25);
  tft->print("--- FRONT RIGHT ---");
  tft->setCursor(165, 37);
  tft->print("Brake:");
  tft->setCursor(165, 49);
  tft->print("Surf:");
  tft->setCursor(165, 61);
  tft->print("Inner:");
  tft->setCursor(165, 73);
  tft->print("Press:");
  tft->setCursor(165, 85);
  tft->print("Wear:");
  tft->setCursor(165, 97);
  tft->print("Dmg T:");
  tft->setCursor(165, 109);
  tft->print("Dmg B:");

  tft->setCursor(5, 130);
  tft->print("--- REAR LEFT ---");
  tft->setCursor(5, 142);
  tft->print("Brake:");
  tft->setCursor(5, 154);
  tft->print("Surf:");
  tft->setCursor(5, 166);
  tft->print("Inner:");
  tft->setCursor(5, 178);
  tft->print("Press:");
  tft->setCursor(5, 190);
  tft->print("Wear:");
  tft->setCursor(5, 202);
  tft->print("Dmg T:");
  tft->setCursor(5, 214);
  tft->print("Dmg B:");

  tft->setCursor(165, 130);
  tft->print("--- REAR RIGHT ---");
  tft->setCursor(165, 142);
  tft->print("Brake:");
  tft->setCursor(165, 154);
  tft->print("Surf:");
  tft->setCursor(165, 166);
  tft->print("Inner:");
  tft->setCursor(165, 178);
  tft->print("Press:");
  tft->setCursor(165, 190);
  tft->print("Wear:");
  tft->setCursor(165, 202);
  tft->print("Dmg T:");
  tft->setCursor(165, 214);
  tft->print("Dmg B:");

  tft->setCursor(100, 227);
  tft->print("Age:");
}

void TelemetryView::drawCarInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->fillRect(0, 0, 320, 20, COLOR_ORANGE);
  tft->setTextSize(2);
  tft->setTextColor(COLOR_BLACK);
  tft->setCursor(30, 3);
  tft->print("CAR DAMAGE (3/4)");

  tft->setTextSize(1);
  tft->setTextColor(COLOR_DARKGREY);

  int y = 25;
  tft->setCursor(5, y);
  tft->print("--- ENGINE ---");
  tft->setCursor(5, y += 12);
  tft->print("Temp:");
  tft->setCursor(5, y += 12);
  tft->print("Damage:");
  tft->setCursor(5, y += 12);
  tft->print("Blown:");
  tft->setCursor(5, y += 12);
  tft->print("Seized:");

  tft->setCursor(5, y += 15);
  tft->print("--- AERO ---");
  tft->setCursor(5, y += 12);
  tft->print("Wing FL:");
  tft->setCursor(5, y += 12);
  tft->print("Wing FR:");
  tft->setCursor(5, y += 12);
  tft->print("Wing Rear:");
  tft->setCursor(5, y += 12);
  tft->print("Floor:");
  tft->setCursor(5, y += 12);
  tft->print("Diffuser:");
  tft->setCursor(5, y += 12);
  tft->print("Sidepod:");

  y = 25;
  tft->setCursor(165, y);
  tft->print("--- POWERTRAIN ---");
  tft->setCursor(165, y += 12);
  tft->print("Gearbox:");
  tft->setCursor(165, y += 12);
  tft->print("ICE:");
  tft->setCursor(165, y += 12);
  tft->print("MGUH:");
  tft->setCursor(165, y += 12);
  tft->print("MGUK:");
  tft->setCursor(165, y += 12);
  tft->print("ES:");
  tft->setCursor(165, y += 12);
  tft->print("CE:");
  tft->setCursor(165, y += 12);
  tft->print("TC:");

  tft->setCursor(165, y += 15);
  tft->print("--- SYSTEMS ---");
  tft->setCursor(165, y += 12);
  tft->print("DRS:");
  tft->setCursor(165, y += 12);
  tft->print("ERS:");
}

void TelemetryView::drawSessionInfoScreen() {
  tft->fillScreen(COLOR_BLACK);

  tft->fillRect(0, 0, 320, 20, COLOR_RED);
  tft->setTextSize(2);
  tft->setTextColor(COLOR_WHITE);
  tft->setCursor(35, 3);
  tft->print("SESSION INFO (4/4)");

  tft->setTextSize(1);
  tft->setTextColor(COLOR_DARKGREY);

  int y = 40;
  tft->setCursor(20, y);
  tft->print("Weather:");
  tft->setCursor(20, y += 20);
  tft->print("Track Temp:");
  tft->setCursor(20, y += 20);
  tft->print("Air Temp:");
  tft->setCursor(20, y += 20);
  tft->print("Session Type:");
  tft->setCursor(20, y += 20);
  tft->print("Time Left:");
  tft->setCursor(20, y += 20);
  tft->print("Safety Car:");
  tft->setCursor(20, y += 20);
  tft->print("Pit Window:");
  tft->setCursor(20, y += 20);
  tft->print("Total Laps:");
}

// ============================================
// DIRTY TRACKING RESET METHODS
// ============================================

void TelemetryView::resetGeneralDirtyTracking() {
  lastPosition = 255;
  lastDeltaFront = 65535;
  lastDeltaLeader = 65535;
  lastSafetyCarDelta = -999.0f;
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
  lastTyrePressureFL = -999.0f;
  lastTyrePressureFR = -999.0f;
  lastTyrePressureRL = -999.0f;
  lastTyrePressureRR = -999.0f;
  lastTyresAgeLaps = 255;

  lastTyreWearFL = -999.0f;
  lastTyreWearFR = -999.0f;
  lastTyreWearRL = -999.0f;
  lastTyreWearRR = -999.0f;
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
  lastEngineTemp = 65535;
  lastFrontLeftWingDamage = 255;
  lastFrontRightWingDamage = 255;
  lastRearWingDamage = 255;
  lastFloorDamage = 255;
  lastDiffuserDamage = 255;
  lastSidepodDamage = 255;
  lastDRSFault = 255;
  lastERSFault = 255;
  lastGearBoxDamage = 255;
  lastEngineDamage = 255;
  lastEngineMGUHWear = 255;
  lastEngineESWear = 255;
  lastEngineCEWear = 255;
  lastEngineICEWear = 255;
  lastEngineMGUKWear = 255;
  lastEngineTCWear = 255;
  lastEngineBlown = 255;
  lastEngineSeized = 255;
}

void TelemetryView::resetSessionInfoDirtyTracking() {
  lastWeather = 255;
  lastTrackTemp = -99;
  lastAirTemp = -99;
  lastSessionType = 255;
  lastSessionTimeLeft = 65535;
  lastSafetyCarStatus = 255;
  lastPitStopWindowIdealLap = 255;
  lastTotalLaps = 255;
}

// ============================================
// UPDATE METHODS - GENERAL SCREEN
// ============================================

void TelemetryView::updatePosition() {
  uint8_t pos = model->getCarPosition();
  if (pos != lastPosition) {
    tft->fillRect(75, 25, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(75, 25);
    if (pos > 0) {
      tft->print("P");
      tft->print(pos);
    } else {
      tft->print("--");
    }
    lastPosition = pos;
  }
}

void TelemetryView::updateDeltaFront() {
  uint16_t delta = model->getDeltaToCarInFrontMS();
  if (delta != lastDeltaFront) {
    tft->fillRect(75, 40, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(75, 40);
    if (delta > 0 && delta < 30000) {
      tft->print("+");
      tft->print(delta / 1000.0f, 3);
      tft->print("s");
    } else {
      tft->print("--");
    }
    lastDeltaFront = delta;
  }
}

void TelemetryView::updateDeltaLeader() {
  uint16_t delta = model->getDeltaToRaceLeaderMS();
  if (delta != lastDeltaLeader) {
    tft->fillRect(75, 55, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setCursor(75, 55);
    if (delta == 0) {
      tft->setTextColor(COLOR_GREEN);
      tft->print("LEADER");
    } else if (delta < 30000) {
      tft->setTextColor(COLOR_ORANGE);
      tft->print("+");
      tft->print(delta / 1000.0f, 2);
      tft->print("s");
    } else {
      tft->setTextColor(COLOR_DARKGREY);
      tft->print("--");
    }
    lastDeltaLeader = delta;
  }
}

void TelemetryView::updateSafetyCarDelta() {
  float delta = model->getSafetyCarDelta();
  if (abs(delta - lastSafetyCarDelta) > 0.01f) {
    tft->fillRect(75, 70, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(75, 70);
    tft->print(delta, 2);
    tft->print("s");
    lastSafetyCarDelta = delta;
  }
}

void TelemetryView::updateLastLapTime() {
  uint32_t time = model->getLastLapTimeMS();
  if (time != lastLapTime) {
    tft->fillRect(75, 85, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(75, 85);
    if (time > 0) {
      char buffer[16];
      model->formatLapTime(time, buffer);
      tft->print(buffer);
    } else {
      tft->print("--:--.--");
    }
    lastLapTime = time;
  }
}

void TelemetryView::updateCurrentLapTime() {
  uint32_t time = model->getCurrentLapTimeMS();
  if (time != lastCurrentLapTime) {
    tft->fillRect(75, 100, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(75, 100);
    if (time > 0) {
      char buffer[16];
      model->formatLapTime(time, buffer);
      tft->print(buffer);
    } else {
      tft->print("--:--.--");
    }
    lastCurrentLapTime = time;
  }
}

void TelemetryView::updateSector1() {
  uint16_t time = model->getSector1TimeMS();
  if (time != lastSector1) {
    tft->fillRect(75, 115, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(75, 115);
    if (time > 0) {
      tft->print(time / 1000.0f, 3);
      tft->print("s");
    } else {
      tft->print("--");
    }
    lastSector1 = time;
  }
}

void TelemetryView::updateSector2() {
  uint16_t time = model->getSector2TimeMS();
  if (time != lastSector2) {
    tft->fillRect(75, 130, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(75, 130);
    if (time > 0) {
      tft->print(time / 1000.0f, 3);
      tft->print("s");
    } else {
      tft->print("--");
    }
    lastSector2 = time;
  }
}

void TelemetryView::updateCurrentLapNum() {
  uint8_t lap = model->getCurrentLapNum();
  if (lap != lastCurrentLapNum) {
    tft->fillRect(75, 145, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(75, 145);
    tft->print(lap);
    lastCurrentLapNum = lap;
  }
}

void TelemetryView::updateCornerCuttingWarnings() {
  uint8_t warnings = model->getCornerCuttingWarnings();
  if (warnings != lastCornerCuttingWarnings) {
    tft->fillRect(75, 160, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(warnings > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 160);
    tft->print(warnings);
    lastCornerCuttingWarnings = warnings;
  }
}

void TelemetryView::updateSpeed() {
  uint16_t speed = model->getSpeed();
  if (speed != lastSpeed) {
    tft->fillRect(235, 25, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(235, 25);
    tft->print(speed);
    tft->print(" km/h");
    lastSpeed = speed;
  }
}

void TelemetryView::updateThrottle() {
  float throttle = model->getThrottle();
  if (abs(throttle - lastThrottle) > 0.01f) {
    tft->fillRect(235, 40, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_GREEN);
    tft->setCursor(235, 40);
    tft->print((int)(throttle * 100));
    tft->print("%");
    lastThrottle = throttle;
  }
}

void TelemetryView::updateBrake() {
  float brake = model->getBrake();
  if (abs(brake - lastBrake) > 0.01f) {
    tft->fillRect(235, 55, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_RED);
    tft->setCursor(235, 55);
    tft->print((int)(brake * 100));
    tft->print("%");
    lastBrake = brake;
  }
}

void TelemetryView::updateGear() {
  int8_t gear = model->getGear();
  if (gear != lastGear) {
    tft->fillRect(235, 70, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(235, 70);
    if (gear == 0) tft->print("N");
    else if (gear == -1) tft->print("R");
    else tft->print(gear);
    lastGear = gear;
  }
}

void TelemetryView::updateRPM() {
  uint16_t rpm = model->getEngineRPM();
  if (rpm != lastRPM) {
    tft->fillRect(235, 85, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(235, 85);
    tft->print(rpm);
    lastRPM = rpm;
  }
}

void TelemetryView::updateDRS() {
  uint8_t drs = model->getDRS();
  if (drs != lastDRS) {
    tft->fillRect(235, 100, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(drs == 1 ? COLOR_GREEN : COLOR_DARKGREY);
    tft->setCursor(235, 100);
    tft->print(drs == 1 ? "ACTIVE" : "OFF");
    lastDRS = drs;
  }
}

void TelemetryView::updateRevLights() {
  uint8_t rev = model->getRevLightsPercent();
  if (rev != lastRevLightsPercent) {
    tft->fillRect(235, 115, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(235, 115);
    tft->print(rev);
    tft->print("%");
    lastRevLightsPercent = rev;
  }
}

void TelemetryView::updateSuggestedGear() {
  int8_t gear = model->getSuggestedGear();
  if (gear != lastSuggestedGear) {
    tft->fillRect(235, 130, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(235, 130);
    if (gear > 0) tft->print(gear);
    else tft->print("--");
    lastSuggestedGear = gear;
  }
}

void TelemetryView::updateFrontBrakeBias() {
  uint8_t bias = model->getFrontBrakeBias();
  if (bias != lastFrontBrakeBias) {
    tft->fillRect(235, 145, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(235, 145);
    tft->print(bias);
    tft->print("%");
    lastFrontBrakeBias = bias;
  }
}

void TelemetryView::updateDiffOnThrottle() {
  uint8_t diff = model->getDiffOnThrottle();
  if (diff != lastDiffOnThrottle) {
    tft->fillRect(235, 160, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(235, 160);
    tft->print(diff);
    tft->print("%");
    lastDiffOnThrottle = diff;
  }
}

void TelemetryView::updateFuelInTank() {
  float fuel = model->getFuelInTank();
  if (abs(fuel - lastFuelInTank) > 0.1f) {
    tft->fillRect(235, 175, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(235, 175);
    tft->print(fuel, 1);
    tft->print(" kg");
    lastFuelInTank = fuel;
  }
}

void TelemetryView::updateFuelRemainingLaps() {
  float laps = model->getFuelRemainingLaps();
  if (abs(laps - lastFuelRemainingLaps) > 0.1f) {
    tft->fillRect(235, 190, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setCursor(235, 190);

    if (laps > 0.1f) {
      uint16_t color = laps < 1.0f ? COLOR_RED : (laps < 3.0f ? COLOR_YELLOW : COLOR_GREEN);
      tft->setTextColor(color);
      tft->print(laps, 1);
      tft->print(" laps");
    } else {
      tft->setTextColor(COLOR_DARKGREY);
      tft->print("--");
    }

    lastFuelRemainingLaps = laps;
  }
}

void TelemetryView::updateERSEnergy() {
  float ers = model->getERSPercent();
  if (abs(ers - lastERSEnergy) > 0.1f) {
    tft->fillRect(235, 205, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(235, 205);
    tft->print(ers, 1);
    tft->print(" %");
    lastERSEnergy = ers;
  }
}

void TelemetryView::updateERSMode() {
  uint8_t mode = model->getERSDeployMode();
  if (mode != lastERSMode) {
    tft->fillRect(235, 220, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(235, 220);
    switch (mode) {
      case 0: tft->print("NONE"); break;
      case 1: tft->print("MEDIUM"); break;
      case 2: tft->print("HOTLAP"); break;
      case 3: tft->print("OVERTAKE"); break;
      default: tft->print("--"); break;
    }
    lastERSMode = mode;
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

// ============================================
// UPDATE METHODS - TYRE INFO SCREEN
// ============================================

void TelemetryView::updateBrakeTemps() {
  uint16_t fl = model->getBrakeTemp(2);
  uint16_t fr = model->getBrakeTemp(3);
  uint16_t rl = model->getBrakeTemp(0);
  uint16_t rr = model->getBrakeTemp(1);

  if (fl != lastBrakeTempFL) {
    tft->fillRect(50, 37, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(50, 37);
    tft->print(fl);
    tft->print("C");
    lastBrakeTempFL = fl;
  }

  if (fr != lastBrakeTempFR) {
    tft->fillRect(220, 37, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(220, 37);
    tft->print(fr);
    tft->print("C");
    lastBrakeTempFR = fr;
  }

  if (rl != lastBrakeTempRL) {
    tft->fillRect(50, 142, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(50, 142);
    tft->print(rl);
    tft->print("C");
    lastBrakeTempRL = rl;
  }

  if (rr != lastBrakeTempRR) {
    tft->fillRect(220, 142, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(220, 142);
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
    tft->fillRect(50, 49, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_RED);
    tft->setCursor(50, 49);
    tft->print(fl);
    tft->print("C");
    lastTyreSurfTempFL = fl;
  }

  if (fr != lastTyreSurfTempFR) {
    tft->fillRect(220, 49, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_RED);
    tft->setCursor(220, 49);
    tft->print(fr);
    tft->print("C");
    lastTyreSurfTempFR = fr;
  }

  if (rl != lastTyreSurfTempRL) {
    tft->fillRect(50, 154, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_RED);
    tft->setCursor(50, 154);
    tft->print(rl);
    tft->print("C");
    lastTyreSurfTempRL = rl;
  }

  if (rr != lastTyreSurfTempRR) {
    tft->fillRect(220, 154, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_RED);
    tft->setCursor(220, 154);
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
    tft->fillRect(50, 61, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(50, 61);
    tft->print(fl);
    tft->print("C");
    lastTyreInnerTempFL = fl;
  }

  if (fr != lastTyreInnerTempFR) {
    tft->fillRect(220, 61, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(220, 61);
    tft->print(fr);
    tft->print("C");
    lastTyreInnerTempFR = fr;
  }

  if (rl != lastTyreInnerTempRL) {
    tft->fillRect(50, 166, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(50, 166);
    tft->print(rl);
    tft->print("C");
    lastTyreInnerTempRL = rl;
  }

  if (rr != lastTyreInnerTempRR) {
    tft->fillRect(220, 166, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(220, 166);
    tft->print(rr);
    tft->print("C");
    lastTyreInnerTempRR = rr;
  }
}

void TelemetryView::updateEngineTemp() {
  uint16_t temp = model->getEngineTemp();
  if (temp != lastEngineTemp) {
    tft->fillRect(60, 37, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(60, 37);
    tft->print(temp);
    tft->print("C");
    lastEngineTemp = temp;
  }
}

void TelemetryView::updateTyrePressures() {
  float fl = model->getTyrePressure(2);
  float fr = model->getTyrePressure(3);
  float rl = model->getTyrePressure(0);
  float rr = model->getTyrePressure(1);

  if (abs(fl - lastTyrePressureFL) > 0.01f) {
    tft->fillRect(50, 73, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(50, 73);
    tft->print(fl, 1);
    tft->print(" PSI");
    lastTyrePressureFL = fl;
  }

  if (abs(fr - lastTyrePressureFR) > 0.01f) {
    tft->fillRect(220, 73, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(220, 73);
    tft->print(fr, 1);
    tft->print(" PSI");
    lastTyrePressureFR = fr;
  }

  if (abs(rl - lastTyrePressureRL) > 0.01f) {
    tft->fillRect(50, 178, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(50, 178);
    tft->print(rl, 1);
    tft->print(" PSI");
    lastTyrePressureRL = rl;
  }

  if (abs(rr - lastTyrePressureRR) > 0.01f) {
    tft->fillRect(220, 178, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(220, 178);
    tft->print(rr, 1);
    tft->print(" PSI");
    lastTyrePressureRR = rr;
  }
}

void TelemetryView::updateTyresAgeLaps() {
  uint8_t age = model->getTyresAgeLaps();
  if (age != lastTyresAgeLaps) {
    tft->fillRect(135, 227, 70, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(135, 227);
    tft->print(age);
    tft->print(" laps");
    lastTyresAgeLaps = age;
  }
}

void TelemetryView::updateTyreWear() {
  float fl = model->getTyreWear(2);
  float fr = model->getTyreWear(3);
  float rl = model->getTyreWear(0);
  float rr = model->getTyreWear(1);

  if (abs(fl - lastTyreWearFL) > 0.1f) {
    tft->fillRect(50, 85, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(50, 85);
    tft->print(fl, 1);
    tft->print("%");
    lastTyreWearFL = fl;
  }

  if (abs(fr - lastTyreWearFR) > 0.1f) {
    tft->fillRect(220, 85, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(220, 85);
    tft->print(fr, 1);
    tft->print("%");
    lastTyreWearFR = fr;
  }

  if (abs(rl - lastTyreWearRL) > 0.1f) {
    tft->fillRect(50, 190, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(50, 190);
    tft->print(rl, 1);
    tft->print("%");
    lastTyreWearRL = rl;
  }

  if (abs(rr - lastTyreWearRR) > 0.1f) {
    tft->fillRect(220, 190, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(220, 190);
    tft->print(rr, 1);
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
    tft->fillRect(50, 97, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(50, 97);
    tft->print(fl);
    tft->print("%");
    lastTyreDamageFL = fl;
  }

  if (fr != lastTyreDamageFR) {
    tft->fillRect(220, 97, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(220, 97);
    tft->print(fr);
    tft->print("%");
    lastTyreDamageFR = fr;
  }

  if (rl != lastTyreDamageRL) {
    tft->fillRect(50, 202, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(50, 202);
    tft->print(rl);
    tft->print("%");
    lastTyreDamageRL = rl;
  }

  if (rr != lastTyreDamageRR) {
    tft->fillRect(220, 202, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(220, 202);
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
    tft->fillRect(50, 109, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(50, 109);
    tft->print(fl);
    tft->print("%");
    lastBrakeDamageFL = fl;
  }

  if (fr != lastBrakeDamageFR) {
    tft->fillRect(220, 109, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(220, 109);
    tft->print(fr);
    tft->print("%");
    lastBrakeDamageFR = fr;
  }

  if (rl != lastBrakeDamageRL) {
    tft->fillRect(50, 214, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(50, 214);
    tft->print(rl);
    tft->print("%");
    lastBrakeDamageRL = rl;
  }

  if (rr != lastBrakeDamageRR) {
    tft->fillRect(220, 214, 100, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(220, 214);
    tft->print(rr);
    tft->print("%");
    lastBrakeDamageRR = rr;
  }
}

// ============================================
// UPDATE METHODS - CAR DAMAGE SCREEN
// ============================================

void TelemetryView::updateWingDamage() {
  uint8_t fl = model->getFrontLeftWingDamage();
  uint8_t fr = model->getFrontRightWingDamage();
  uint8_t rear = model->getRearWingDamage();

  if (fl != lastFrontLeftWingDamage) {
    tft->fillRect(75, 97, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fl > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 97);
    tft->print(fl);
    tft->print("%");
    lastFrontLeftWingDamage = fl;
  }

  if (fr != lastFrontRightWingDamage) {
    tft->fillRect(75, 109, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(fr > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 109);
    tft->print(fr);
    tft->print("%");
    lastFrontRightWingDamage = fr;
  }

  if (rear != lastRearWingDamage) {
    tft->fillRect(75, 121, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(rear > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 121);
    tft->print(rear);
    tft->print("%");
    lastRearWingDamage = rear;
  }
}

void TelemetryView::updateAeroDamage() {
  uint8_t floor = model->getFloorDamage();
  uint8_t diffuser = model->getDiffuserDamage();
  uint8_t sidepod = model->getSidepodDamage();

  if (floor != lastFloorDamage) {
    tft->fillRect(75, 133, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(floor > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 133);
    tft->print(floor);
    tft->print("%");
    lastFloorDamage = floor;
  }

  if (diffuser != lastDiffuserDamage) {
    tft->fillRect(75, 145, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(diffuser > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 145);
    tft->print(diffuser);
    tft->print("%");
    lastDiffuserDamage = diffuser;
  }

  if (sidepod != lastSidepodDamage) {
    tft->fillRect(75, 157, 80, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(sidepod > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(75, 157);
    tft->print(sidepod);
    tft->print("%");
    lastSidepodDamage = sidepod;
  }
}

void TelemetryView::updatePowertrainDamage() {
  uint8_t gearbox = model->getGearBoxDamage();
  uint8_t ice = model->getEngineICEWear();
  uint8_t mguh = model->getEngineMGUHWear();
  uint8_t mguk = model->getEngineMGUKWear();
  uint8_t es = model->getEngineESWear();
  uint8_t ce = model->getEngineCEWear();
  uint8_t tc = model->getEngineTCWear();

  if (gearbox != lastGearBoxDamage) {
    tft->fillRect(240, 37, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(gearbox > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 37);
    tft->print(gearbox);
    tft->print("%");
    lastGearBoxDamage = gearbox;
  }

  if (ice != lastEngineICEWear) {
    tft->fillRect(240, 49, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(ice > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 49);
    tft->print(ice);
    tft->print("%");
    lastEngineICEWear = ice;
  }

  if (mguh != lastEngineMGUHWear) {
    tft->fillRect(240, 61, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(mguh > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 61);
    tft->print(mguh);
    tft->print("%");
    lastEngineMGUHWear = mguh;
  }

  if (mguk != lastEngineMGUKWear) {
    tft->fillRect(240, 73, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(mguk > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 73);
    tft->print(mguk);
    tft->print("%");
    lastEngineMGUKWear = mguk;
  }

  if (es != lastEngineESWear) {
    tft->fillRect(240, 85, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(es > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 85);
    tft->print(es);
    tft->print("%");
    lastEngineESWear = es;
  }

  if (ce != lastEngineCEWear) {
    tft->fillRect(240, 97, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(ce > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 97);
    tft->print(ce);
    tft->print("%");
    lastEngineCEWear = ce;
  }

  if (tc != lastEngineTCWear) {
    tft->fillRect(240, 109, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(tc > 80 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 109);
    tft->print(tc);
    tft->print("%");
    lastEngineTCWear = tc;
  }
}

void TelemetryView::updateCriticalDamage() {
  uint8_t engineDmg = model->getEngineDamage();
  uint8_t blown = model->getEngineBlown();
  uint8_t seized = model->getEngineSeized();
  uint8_t drsFault = model->getDRSFault();
  uint8_t ersFault = model->getERSFault();

  if (engineDmg != lastEngineDamage) {
    tft->fillRect(60, 49, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(engineDmg > 50 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(60, 49);
    tft->print(engineDmg);
    tft->print("%");
    lastEngineDamage = engineDmg;
  }

  if (blown != lastEngineBlown) {
    tft->fillRect(60, 61, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(blown > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(60, 61);
    tft->print(blown > 0 ? "YES" : "NO");
    lastEngineBlown = blown;
  }

  if (seized != lastEngineSeized) {
    tft->fillRect(60, 73, 90, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(seized > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(60, 73);
    tft->print(seized > 0 ? "YES" : "NO");
    lastEngineSeized = seized;
  }

  if (drsFault != lastDRSFault) {
    tft->fillRect(240, 134, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(drsFault > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 134);
    tft->print(drsFault > 0 ? "FAULT" : "OK");
    lastDRSFault = drsFault;
  }

  if (ersFault != lastERSFault) {
    tft->fillRect(240, 146, 75, 10, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(ersFault > 0 ? COLOR_RED : COLOR_GREEN);
    tft->setCursor(240, 146);
    tft->print(ersFault > 0 ? "FAULT" : "OK");
    lastERSFault = ersFault;
  }
}

// ============================================
// UPDATE METHODS - SESSION INFO SCREEN
// ============================================

void TelemetryView::updateWeather() {
  uint8_t weather = model->getWeather();
  if (weather != lastWeather) {
    tft->fillRect(140, 40, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(140, 40);
    switch (weather) {
      case 0: tft->print("Clear"); break;
      case 1: tft->print("Light Cloud"); break;
      case 2: tft->print("Overcast"); break;
      case 3: tft->print("Light Rain"); break;
      case 4: tft->print("Heavy Rain"); break;
      case 5: tft->print("Storm"); break;
      default: tft->print("Unknown"); break;
    }
    lastWeather = weather;
  }
}

void TelemetryView::updateTrackTemp() {
  int8_t temp = model->getTrackTemperature();
  if (temp != lastTrackTemp) {
    tft->fillRect(140, 60, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_ORANGE);
    tft->setCursor(140, 60);
    tft->print(temp);
    tft->print(" C");
    lastTrackTemp = temp;
  }
}

void TelemetryView::updateAirTemp() {
  int8_t temp = model->getAirTemperature();
  if (temp != lastAirTemp) {
    tft->fillRect(140, 80, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_CYAN);
    tft->setCursor(140, 80);
    tft->print(temp);
    tft->print(" C");
    lastAirTemp = temp;
  }
}

void TelemetryView::updateSessionType() {
  uint8_t type = model->getSessionType();
  if (type != lastSessionType) {
    tft->fillRect(140, 100, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_YELLOW);
    tft->setCursor(140, 100);
    switch (type) {
      case 1: tft->print("Practice 1"); break;
      case 2: tft->print("Practice 2"); break;
      case 3: tft->print("Practice 3"); break;
      case 5: tft->print("Qualifying 1"); break;
      case 6: tft->print("Qualifying 2"); break;
      case 7: tft->print("Qualifying 3"); break;
      case 10: tft->print("Race"); break;
      default: tft->print("Unknown"); break;
    }
    lastSessionType = type;
  }
}

void TelemetryView::updateSessionTimeLeft() {
  uint16_t time = model->getSessionTimeLeft();
  uint8_t sessionType = model->getSessionType();

  if (time != lastSessionTimeLeft) {
    tft->fillRect(140, 120, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(140, 120);

    if (sessionType == SESSION_RACE || sessionType == SESSION_RACE2 || sessionType == SESSION_RACE3) {
      tft->print("--");
    } else if (time > 0) {
      uint16_t mins = time / 60;
      uint16_t secs = time % 60;
      tft->print(mins);
      tft->print(":");
      if (secs < 10) tft->print("0");
      tft->print(secs);
    } else {
      tft->print("--");
    }

    lastSessionTimeLeft = time;
  }
}

void TelemetryView::updateSafetyCarStatus() {
  uint8_t status = model->getSafetyCarStatus();
  if (status != lastSafetyCarStatus) {
    tft->fillRect(140, 140, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setCursor(140, 140);
    switch (status) {
      case 0:
        tft->setTextColor(COLOR_GREEN);
        tft->print("No Safety Car");
        break;
      case 1:
        tft->setTextColor(COLOR_YELLOW);
        tft->print("Full Safety Car");
        break;
      case 2:
        tft->setTextColor(COLOR_YELLOW);
        tft->print("Virtual SC");
        break;
      case 3:
        tft->setTextColor(COLOR_CYAN);
        tft->print("Formation Lap");
        break;
      default:
        tft->setTextColor(COLOR_DARKGREY);
        tft->print("Unknown");
        break;
    }
    lastSafetyCarStatus = status;
  }
}

void TelemetryView::updatePitStopWindow() {
  uint8_t lap = model->getPitStopWindowIdealLap();
  uint8_t sessionType = model->getSessionType();

  if (lap != lastPitStopWindowIdealLap) {
    tft->fillRect(140, 160, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_MAGENTA);
    tft->setCursor(140, 160);

    if (sessionType != SESSION_RACE && sessionType != SESSION_RACE2 && sessionType != SESSION_RACE3) {
      tft->print("--");
    } else if (lap > 0) {
      tft->print("Lap ");
      tft->print(lap);
    } else {
      tft->print("N/A");
    }

    lastPitStopWindowIdealLap = lap;
  }
}

void TelemetryView::updateTotalLaps() {
  uint8_t laps = model->getTotalLaps();
  if (laps != lastTotalLaps) {
    tft->fillRect(140, 180, 160, 15, COLOR_BLACK);
    tft->setTextSize(1);
    tft->setTextColor(COLOR_WHITE);
    tft->setCursor(140, 180);
    tft->print(laps);
    lastTotalLaps = laps;
  }
}

// ============================================
// BOOT SCREEN (NEATINS!)
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