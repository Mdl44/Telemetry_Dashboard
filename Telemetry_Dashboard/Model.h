#ifndef MODEL_H
#define MODEL_H

#include <Arduino.h>
#include "Config.h"

class TelemetryModel {
  uint16_t speed;
  int8_t gear;
  uint16_t engineRPM;
  uint8_t revLightsPercent;

  uint32_t currentLapTimeMS;
  uint32_t lastLapTimeMS;
  float deltaTime;
  uint8_t currentLapNum;
  uint8_t totalLaps;

  uint8_t carPosition;
  uint8_t totalCars;
  uint8_t sessionType;
  int8_t trackStatus;

  uint16_t engineTemp;
  uint16_t brakesTemp[4];
  uint8_t tyresSurfaceTemp[4];
  uint8_t tyresInnerTemp[4];
  float tyresPressure[4];
  float tyresWear[4];

  float fuelInTank;
  float fuelCapacity;
  float fuelRemainingLaps;
  float ersStoreEnergy;
  uint8_t ersDeployMode;

  uint8_t drsAvailable;
  uint8_t drsActive;

  uint8_t frontWingDamage;
  uint8_t rearWingDamage;
  uint8_t engineDamage;

  UserSettings settings;

  ScreenMode currentScreen;
  bool inConfigMode;

  uint32_t packetsReceived;
  uint32_t lastPacketTime;
  bool connected;

public:
  TelemetryModel();

  void updateTelemetry(const PacketCarTelemetryData* packet, uint8_t playerIndex);
  void updateLapData(const PacketLapData* packet, uint8_t playerIndex);
  void updateCarStatus(const PacketCarStatusData* packet, uint8_t playerIndex);
  void updateCarDamage(const PacketCarDamageData* packet, uint8_t playerIndex);
  void updateSessionData(const PacketSessionData* packet);

  uint16_t getSpeed() const {
    return speed;
  }
  int8_t getGear() const {
    return gear;
  }
  uint16_t getEngineRPM() const {
    return engineRPM;
  }
  uint8_t getRevLightsPercent() const {
    return revLightsPercent;
  }

  uint32_t getCurrentLapTimeMS() const {
    return currentLapTimeMS;
  }
  uint32_t getLastLapTimeMS() const {
    return lastLapTimeMS;
  }
  float getDeltaTime() const {
    return deltaTime;
  }
  uint8_t getCurrentLapNum() const {
    return currentLapNum;
  }
  uint8_t getTotalLaps() const {
    return totalLaps;
  }
  void formatLapTime(uint32_t timeMS, char* buffer);

  uint8_t getCarPosition() const {
    return carPosition;
  }
  uint8_t getTotalCars() const {
    return totalCars;
  }

  uint8_t getSessionType() const {
    return sessionType;
  }
  int8_t getTrackStatus() const {
    return trackStatus;
  }

  uint16_t getEngineTemp() const {
    return engineTemp;
  }
  uint16_t getBrakeTemp(uint8_t corner) const {
    return brakesTemp[corner];
  }
  uint8_t getTyreSurfaceTemp(uint8_t corner) const {
    return tyresSurfaceTemp[corner];
  }
  uint8_t getTyreInnerTemp(uint8_t corner) const {
    return tyresInnerTemp[corner];
  }

  float getTyrePressure(uint8_t corner) const {
    return tyresPressure[corner];
  }
  float getTyreWear(uint8_t corner) const {
    return tyresWear[corner];
  }

  float getFuelInTank() const {
    return fuelInTank;
  }
  float getFuelCapacity() const {
    return fuelCapacity;
  }
  float getFuelRemainingLaps() const {
    return fuelRemainingLaps;
  }

  float getERSStoreEnergy() const {
    return ersStoreEnergy;
  }
  uint8_t getERSDeployMode() const {
    return ersDeployMode;
  }
  float getERSPercent() const {
    return (ersStoreEnergy / 4.0f) * 100.0f;
  }

  uint8_t getDRSAvailable() const {
    return drsAvailable;
  }
  uint8_t getDRSActive() const {
    return drsActive;
  }

  uint8_t getFrontWingDamage() const {
    return frontWingDamage;
  }
  uint8_t getRearWingDamage() const {
    return rearWingDamage;
  }
  uint8_t getEngineDamage() const {
    return engineDamage;
  }

  ScreenMode getCurrentScreen() const {
    return currentScreen;
  }
  void setCurrentScreen(ScreenMode screen) {
    currentScreen = screen;
  }
  void nextScreen();
  void previousScreen();

  bool isInConfigMode() const {
    return inConfigMode;
  }
  void enterConfigMode() {
    inConfigMode = true;
  }
  void exitConfigMode() {
    inConfigMode = false;
  }

  UserSettings* getSettings() {
    return &settings;
  }
  void loadSettings();
  void saveSettings();

  bool isConnected() const {
    return connected;
  }
  uint32_t getPacketsReceived() const {
    return packetsReceived;
  }
  void updateConnectionStatus();
};

#endif