#ifndef MODEL_H
#define MODEL_H

#include <Arduino.h>
#include "Config.h"

class TelemetryModel {
  // ============================================
  // PacketSessionData
  // ============================================
  uint8_t weather;
  int8_t trackTemperature;
  int8_t airTemperature;
  uint8_t sessionType;
  uint16_t sessionTimeLeft;
  uint8_t safetyCarStatus;
  uint8_t totalLaps;

  // ============================================
  // LapData
  // ============================================
  uint32_t lastLapTimeMS;
  uint32_t currentLapTimeMS;
  uint16_t sector1TimeMS;
  uint16_t sector2TimeMS;
  uint16_t deltaToCarInFrontMS;
  uint16_t deltaToRaceLeaderMS;
  uint8_t carPosition;
  uint8_t currentLapNum;
  uint8_t cornerCuttingWarnings;
  float lapDistance;
  uint32_t bestLapTimeMS;

#define MAX_REFERENCE_POINTS 300
  struct ReferencePoint {
    float distance;
    uint32_t timeMS;
  };
  ReferencePoint referenceLap[MAX_REFERENCE_POINTS];
  ReferencePoint currentRecording[MAX_REFERENCE_POINTS];
  uint16_t referencePointCount;
  uint16_t currentRecordingCount;
  float trackLength;
  bool hasReferenceLap;

  // ============================================
  // CarSetupData
  // ============================================
  uint8_t diffOnThrottle;

  // ============================================
  // CarTelemetryData
  // ============================================
  uint16_t speed;
  float throttle;
  float brake;
  int8_t gear;
  uint16_t engineRPM;
  uint8_t drs;
  uint8_t revLightsPercent;
  uint16_t brakesTemp[4];
  uint8_t tyresSurfaceTemp[4];
  uint8_t tyresInnerTemp[4];
  uint16_t engineTemp;
  float tyresPressure[4];
  int8_t suggestedGear;

  // ============================================
  // CarStatusData
  // ============================================
  uint8_t frontBrakeBias;
  float fuelInTank;
  float fuelRemainingLaps;
  uint8_t tyresAgeLaps;
  float enginePowerICE;
  float enginePowerMGUK;
  float ersStoreEnergy;
  uint8_t ersDeployMode;

  // ============================================
  // CarDamageData
  // ============================================
  float tyresWear[4];
  uint8_t tyresDamage[4];
  uint8_t brakesDamage[4];
  uint8_t frontLeftWingDamage;
  uint8_t frontRightWingDamage;
  uint8_t rearWingDamage;
  uint8_t floorDamage;
  uint8_t diffuserDamage;
  uint8_t sidepodDamage;
  uint8_t drsFault;
  uint8_t ersFault;
  uint8_t gearBoxDamage;
  uint8_t engineDamage;
  uint8_t engineMGUHWear;
  uint8_t engineESWear;
  uint8_t engineCEWear;
  uint8_t engineICEWear;
  uint8_t engineMGUKWear;
  uint8_t engineTCWear;

  // ============================================
  // Utility
  // ============================================
  uint32_t packetsReceived;

public:
  TelemetryModel();

  void updateSessionData(const PacketSessionData* packet);
  void updateLapData(const PacketLapData* packet, uint8_t playerIndex);
  void updateCarSetup(const PacketCarSetupData* packet, uint8_t playerIndex);
  void updateTelemetry(const PacketCarTelemetryData* packet, uint8_t playerIndex);
  void updateCarStatus(const PacketCarStatusData* packet, uint8_t playerIndex);
  void updateCarDamage(const PacketCarDamageData* packet, uint8_t playerIndex);

  // ============================================
  // Getters - PacketSessionData
  // ============================================
  uint8_t getWeather() const {
    return weather;
  }
  int8_t getTrackTemperature() const {
    return trackTemperature;
  }
  int8_t getAirTemperature() const {
    return airTemperature;
  }
  uint8_t getSessionType() const {
    return sessionType;
  }
  uint16_t getSessionTimeLeft() const {
    return sessionTimeLeft;
  }
  uint8_t getSafetyCarStatus() const {
    return safetyCarStatus;
  }
  uint8_t getTotalLaps() const {
    return totalLaps;
  }

  // ============================================
  // Getters - LapData
  // ============================================
  uint32_t getLastLapTimeMS() const {
    return lastLapTimeMS;
  }
  uint32_t getCurrentLapTimeMS() const {
    return currentLapTimeMS;
  }
  uint16_t getSector1TimeMS() const {
    return sector1TimeMS;
  }
  uint16_t getSector2TimeMS() const {
    return sector2TimeMS;
  }
  uint16_t getDeltaToCarInFrontMS() const {
    return deltaToCarInFrontMS;
  }
  uint16_t getDeltaToRaceLeaderMS() const {
    return deltaToRaceLeaderMS;
  }
  uint8_t getCarPosition() const {
    return carPosition;
  }
  uint8_t getCurrentLapNum() const {
    return currentLapNum;
  }
  uint8_t getCornerCuttingWarnings() const {
    return cornerCuttingWarnings;
  }
  uint32_t getBestLapTimeMS() const {
    return bestLapTimeMS;
  }

  float getLapDistance() const {
    return lapDistance;
  }

  float getDeltaLive() const {
    if (!hasReferenceLap || bestLapTimeMS == 0 || referencePointCount < 2) {
      return 0.0f;
    }

    if (lapDistance < 10.0f || currentLapTimeMS == 0) {
      return 0.0f;
    }

    uint32_t referenceTimeMS = interpolateReferenceTime(lapDistance);
    if (referenceTimeMS == 0) {
      return 0.0f;
    }

    return ((int32_t)currentLapTimeMS - (int32_t)referenceTimeMS) / 1000.0f;
  }

private:
  uint32_t interpolateReferenceTime(float distance) const {
    if (referencePointCount < 2) return 0;

    for (uint16_t i = 0; i < referencePointCount - 1; i++) {
      if (distance >= referenceLap[i].distance && distance <= referenceLap[i + 1].distance) {
        float ratio = (distance - referenceLap[i].distance) / (referenceLap[i + 1].distance - referenceLap[i].distance);
        return referenceLap[i].timeMS + (uint32_t)(ratio * (referenceLap[i + 1].timeMS - referenceLap[i].timeMS));
      }
    }

    return 0;
  }
public:

  // ============================================
  // Getters - CarSetupData
  // ============================================
  uint8_t getDiffOnThrottle() const {
    return diffOnThrottle;
  }

  // ============================================
  // Getters - CarTelemetryData
  // ============================================
  uint16_t getSpeed() const {
    return speed;
  }
  float getThrottle() const {
    return throttle;
  }
  float getBrake() const {
    return brake;
  }
  int8_t getGear() const {
    return gear;
  }
  uint16_t getEngineRPM() const {
    return engineRPM;
  }
  uint8_t getDRS() const {
    return drs;
  }
  uint8_t getRevLightsPercent() const {
    return revLightsPercent;
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
  uint16_t getEngineTemp() const {
    return engineTemp;
  }
  float getTyrePressure(uint8_t corner) const {
    return tyresPressure[corner];
  }
  int8_t getSuggestedGear() const {
    return suggestedGear;
  }

  // ============================================
  // Getters - CarStatusData
  // ============================================
  uint8_t getFrontBrakeBias() const {
    return frontBrakeBias;
  }
  float getFuelInTank() const {
    return fuelInTank;
  }
  float getFuelRemainingLaps() const {
    return fuelRemainingLaps;
  }
  uint8_t getTyresAgeLaps() const {
    return tyresAgeLaps;
  }
  float getEnginePowerICE() const {
    return enginePowerICE;
  }
  float getEnginePowerMGUK() const {
    return enginePowerMGUK;
  }
  uint8_t getERSDeployMode() const {
    return ersDeployMode;
  }
  float getERSPercent() const {
    return (ersStoreEnergy / 4000000.0f) * 100.0f;
  }

  // ============================================
  // Getters - CarDamageData
  // ============================================
  float getTyreWear(uint8_t corner) const {
    return tyresWear[corner];
  }
  uint8_t getTyreDamage(uint8_t corner) const {
    return tyresDamage[corner];
  }
  uint8_t getBrakeDamage(uint8_t corner) const {
    return brakesDamage[corner];
  }
  uint8_t getFrontLeftWingDamage() const {
    return frontLeftWingDamage;
  }
  uint8_t getFrontRightWingDamage() const {
    return frontRightWingDamage;
  }
  uint8_t getRearWingDamage() const {
    return rearWingDamage;
  }
  uint8_t getFloorDamage() const {
    return floorDamage;
  }
  uint8_t getDiffuserDamage() const {
    return diffuserDamage;
  }
  uint8_t getSidepodDamage() const {
    return sidepodDamage;
  }
  uint8_t getDRSFault() const {
    return drsFault;
  }
  uint8_t getERSFault() const {
    return ersFault;
  }
  uint8_t getGearBoxDamage() const {
    return gearBoxDamage;
  }
  uint8_t getEngineDamage() const {
    return engineDamage;
  }
  uint8_t getEngineMGUHWear() const {
    return engineMGUHWear;
  }
  uint8_t getEngineESWear() const {
    return engineESWear;
  }
  uint8_t getEngineCEWear() const {
    return engineCEWear;
  }
  uint8_t getEngineICEWear() const {
    return engineICEWear;
  }
  uint8_t getEngineMGUKWear() const {
    return engineMGUKWear;
  }
  uint8_t getEngineTCWear() const {
    return engineTCWear;
  }

  // ============================================
  // Utility
  // ============================================
  uint32_t getPacketsReceived() const {
    return packetsReceived;
  }
  void formatLapTime(uint32_t timeMS, char* buffer);
};

#endif