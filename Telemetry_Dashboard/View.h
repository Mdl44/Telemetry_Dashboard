#ifndef VIEW_H
#define VIEW_H

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include "Config.h"
#include "Model.h"

class TelemetryView {
  Adafruit_ILI9341* tft;
  Adafruit_NeoPixel* pixels;
  TelemetryModel* model;

  // ============================================
  // Screen Management
  // ============================================
  enum Screen {
    SCREEN_GENERAL = 0,
    SCREEN_TYRE_INFO = 1,
    SCREEN_CAR_INFO = 2,
    SCREEN_SESSION_INFO = 3
  };

  Screen currentScreen;
  bool screenChanged;

  // ============================================
  // Boot State
  // ============================================
  bool bootInfoDrawn;

  // ============================================
  // Dirty Tracking - General Screen
  // ============================================
  uint8_t lastPosition;
  uint16_t lastDeltaFront;
  uint16_t lastDeltaLeader;
  float lastSafetyCarDelta;
  uint32_t lastLapTime;
  uint32_t lastCurrentLapTime;
  uint16_t lastSector1;
  uint16_t lastSector2;
  uint8_t lastCurrentLapNum;
  uint8_t lastCornerCuttingWarnings;

  uint16_t lastSpeed;
  float lastThrottle;
  float lastBrake;
  int8_t lastGear;
  uint16_t lastRPM;
  uint8_t lastDRS;
  uint8_t lastRevLightsPercent;
  int8_t lastSuggestedGear;

  uint8_t lastFrontBrakeBias;
  uint8_t lastDiffOnThrottle;
  float lastFuelInTank;
  float lastFuelRemainingLaps;
  float lastERSEnergy;
  uint8_t lastERSMode;

  uint8_t lastLedsOn;

  // ============================================
  // Dirty Tracking - Car Info Screen
  // ============================================
  uint16_t lastBrakeTempFL;
  uint16_t lastBrakeTempFR;
  uint16_t lastBrakeTempRL;
  uint16_t lastBrakeTempRR;
  uint8_t lastTyreSurfTempFL;
  uint8_t lastTyreSurfTempFR;
  uint8_t lastTyreSurfTempRL;
  uint8_t lastTyreSurfTempRR;
  uint8_t lastTyreInnerTempFL;
  uint8_t lastTyreInnerTempFR;
  uint8_t lastTyreInnerTempRL;
  uint8_t lastTyreInnerTempRR;
  uint16_t lastEngineTemp;
  float lastTyrePressureFL;
  float lastTyrePressureFR;
  float lastTyrePressureRL;
  float lastTyrePressureRR;
  uint8_t lastTyresAgeLaps;

  float lastTyreWearFL;
  float lastTyreWearFR;
  float lastTyreWearRL;
  float lastTyreWearRR;
  uint8_t lastTyreDamageFL;
  uint8_t lastTyreDamageFR;
  uint8_t lastTyreDamageRL;
  uint8_t lastTyreDamageRR;
  uint8_t lastBrakeDamageFL;
  uint8_t lastBrakeDamageFR;
  uint8_t lastBrakeDamageRL;
  uint8_t lastBrakeDamageRR;
  uint8_t lastFrontLeftWingDamage;
  uint8_t lastFrontRightWingDamage;
  uint8_t lastRearWingDamage;
  uint8_t lastFloorDamage;
  uint8_t lastDiffuserDamage;
  uint8_t lastSidepodDamage;
  uint8_t lastDRSFault;
  uint8_t lastERSFault;
  uint8_t lastGearBoxDamage;
  uint8_t lastEngineDamage;
  uint8_t lastEngineMGUHWear;
  uint8_t lastEngineESWear;
  uint8_t lastEngineCEWear;
  uint8_t lastEngineICEWear;
  uint8_t lastEngineMGUKWear;
  uint8_t lastEngineTCWear;
  uint8_t lastEngineBlown;
  uint8_t lastEngineSeized;

  // ============================================
  // Dirty Tracking - Session Info Screen
  // ============================================
  uint8_t lastWeather;
  int8_t lastTrackTemp;
  int8_t lastAirTemp;
  uint8_t lastSessionType;
  uint16_t lastSessionTimeLeft;
  uint8_t lastSafetyCarStatus;
  uint8_t lastPitStopWindowIdealLap;
  uint8_t lastTotalLaps;

  // ============================================
  // Screen Drawing Methods
  // ============================================
  void drawGeneralScreen();
  void drawTyreInfoScreen();
  void drawCarInfoScreen();
  void drawSessionInfoScreen();

  // ============================================
  // Dirty Tracking Reset Methods
  // ============================================
  void resetGeneralDirtyTracking();
  void resetTyreInfoDirtyTracking();
  void resetCarInfoDirtyTracking();
  void resetSessionInfoDirtyTracking();

  // ============================================
  // Update Methods - General Screen
  // ============================================
  void updatePosition();
  void updateDeltaFront();
  void updateDeltaLeader();
  void updateSafetyCarDelta();
  void updateLastLapTime();
  void updateCurrentLapTime();
  void updateSector1();
  void updateSector2();
  void updateCurrentLapNum();
  void updateCornerCuttingWarnings();

  void updateSpeed();
  void updateThrottle();
  void updateBrake();
  void updateGear();
  void updateRPM();
  void updateDRS();
  void updateRevLights();
  void updateSuggestedGear();

  void updateFrontBrakeBias();
  void updateDiffOnThrottle();
  void updateFuelInTank();
  void updateFuelRemainingLaps();
  void updateERSEnergy();
  void updateERSMode();

  void updateLEDs();

  // ============================================
  // Update Methods - Tyre Info Screen
  // ============================================
  void updateBrakeTemps();
  void updateTyreSurfaceTemps();
  void updateTyreInnerTemps();
  void updateTyrePressures();
  void updateTyresAgeLaps();
  void updateTyreWear();
  void updateTyreDamage();
  void updateBrakeDamage();

  // ============================================
  // Update Methods - Car Info Screen
  // ============================================
  void updateEngineTemp();
  void updateWingDamage();
  void updateAeroDamage();
  void updatePowertrainDamage();
  void updateCriticalDamage();

  // ============================================
  // Update Methods - Session Info Screen
  // ============================================
  void updateWeather();
  void updateTrackTemp();
  void updateAirTemp();
  void updateSessionType();
  void updateSessionTimeLeft();
  void updateSafetyCarStatus();
  void updatePitStopWindow();
  void updateTotalLaps();

public:
  TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m);

  void init();
  void render();
  void drawLayout();

  void drawBootAnimation(uint32_t elapsedMS);
  void drawBootInfo(IPAddress ip);
  void resetBootInfo();

  void nextScreen();
  void previousScreen();
  Screen getCurrentScreen() const {
    return currentScreen;
  }
};

#endif