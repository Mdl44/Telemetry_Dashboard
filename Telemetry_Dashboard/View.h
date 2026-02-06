#ifndef VIEW_H
#define VIEW_H

#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_NeoPixel.h>
#include "Model.h"
#include "Config.h"

class TelemetryView {
public:
  // ============================================
  // Screen Management
  // ============================================
  enum Screen {
    SCREEN_GENERAL = 0,
    SCREEN_TYRE_INFO = 1,
    SCREEN_CAR_INFO = 2,
    SCREEN_SESSION_INFO = 3
  };

private:
  // ============================================
  // Hardware References
  // ============================================
  Adafruit_ILI9341* tft;
  Adafruit_NeoPixel* pixels;
  TelemetryModel* model;

  // ============================================
  // Screen State
  // ============================================
  Screen currentScreen;
  bool screenChanged;
  bool bootInfoDrawn;

  // ============================================
  // Dirty Tracking - SCREEN 1: GENERAL
  // ============================================
  uint8_t lastPosition;
  uint16_t lastDeltaFront;
  uint16_t lastDeltaLeader;
  float lastDeltaLive;

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

  int lastLedsOn;

  // ============================================
  // Dirty Tracking - SCREEN 2: TYRE INFO
  // ============================================
  uint8_t lastTyresAgeLaps;

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

  float lastTyrePressureFL;
  float lastTyrePressureFR;
  float lastTyrePressureRL;
  float lastTyrePressureRR;

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

  // ============================================
  // Dirty Tracking - SCREEN 3: CAR INFO
  // ============================================
  float lastICEPower;
  float lastMGUKPower;
  float lastERSPercent;
  uint16_t lastEngineTemp;
  uint8_t lastDRSFault;
  uint8_t lastERSFault;

  uint8_t lastWingDamageFL;
  uint8_t lastWingDamageFR;
  uint8_t lastWingDamageRear;
  uint8_t lastFloorDamage;
  uint8_t lastDiffuserDamage;
  uint8_t lastSidepodDamage;

  uint8_t lastGearboxDamage;
  uint8_t lastICEDamage;
  uint8_t lastMGUHDamage;
  uint8_t lastMGUKDamage;
  uint8_t lastTCDamage;
  uint8_t lastERSDamage;
  uint8_t lastCEDamage;


  // ============================================
  // Dirty Tracking - SCREEN 4: RACE OVERVIEW
  // ============================================
  uint8_t lastWeather;
  int8_t lastTrackTemp;
  int8_t lastAirTemp;

  uint8_t lastSessionType;
  uint8_t lastCurrentLapInfo;
  uint8_t lastTotalLaps;
  uint16_t lastSessionTimeLeft;
  uint8_t lastSafetyCarStatus;

  uint32_t lastBestLapTime;
  uint32_t lastLastLapTime;
  uint16_t lastSector1Time;
  uint16_t lastSector2Time;

  float lastFuelRemaining;
  uint8_t lastTyresAge;
  uint16_t lastEngineTempCritical;
  uint8_t lastOverallDamage;

public:
  // ============================================
  // Constructor
  // ============================================
  TelemetryView(Adafruit_ILI9341* display, Adafruit_NeoPixel* leds, TelemetryModel* m);

  // ============================================
  // Core Methods
  // ============================================
  void init();
  void render();
  void drawLayout();
  void nextScreen();

  // ============================================
  // Screen Drawing Methods
  // ============================================
  void drawGeneralScreen();
  void drawTyreInfoScreen();
  void drawCarInfoScreen();
  void drawSessionInfoScreen();

  // ============================================
  // Reset Dirty Tracking
  // ============================================
  void resetGeneralDirtyTracking();
  void resetTyreInfoDirtyTracking();
  void resetCarInfoDirtyTracking();
  void resetSessionInfoDirtyTracking();

  // ============================================
  // Update Methods - SCREEN 1: GENERAL
  // ============================================
  void updatePosition();
  void updateDeltaFront();
  void updateDeltaLeader();
  void updateDeltaLive();
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
  // Update Methods - SCREEN 2: TYRE INFO
  // ============================================
  void updateTyresAgeLaps();
  void updateBrakeTemps();
  void updateTyreSurfaceTemps();
  void updateTyreInnerTemps();
  void updateTyrePressures();
  void updateTyreWear();
  void updateTyreDamage();
  void updateBrakeDamage();

  // ============================================
  // Update Methods - SCREEN 3: CAR INFO
  // ============================================
  void updatePowerUnit();
  void updateAeroDamage();
  void updateComponentWear();

  // ============================================
  // Update Methods - SCREEN 4: RACE OVERVIEW
  // ============================================
  void updateWeather();
  void updateTrackTemp();
  void updateAirTemp();

  void updateSessionType();
  void updateLapInfo();
  void updateSessionTimeLeft();
  void updateSafetyCarStatus();

  void updateBestLap();
  void updateLastLap();
  void updateSectorTimes();

  void updateFuelStatus();
  void updateTyreStatus();
  void updateEngineStatus();
  void updateDamageStatus();

  // ============================================
  // Boot Screen
  // ============================================
  void drawBootAnimation(uint32_t elapsedMS);
  void drawBootInfo(IPAddress ip);
  void resetBootInfo();
};

#endif