#include "Model.h"

TelemetryModel::TelemetryModel() {
  // ============================================
  // PacketSessionData
  // ============================================
  weather = 0;
  trackTemperature = 0;
  airTemperature = 0;
  sessionType = SESSION_UNKNOWN;
  sessionTimeLeft = 0;
  safetyCarStatus = 0;
  pitStopWindowIdealLap = 0;
  totalLaps = 0;

  // ============================================
  // LapData
  // ============================================
  lastLapTimeMS = 0;
  currentLapTimeMS = 0;
  sector1TimeMS = 0;
  sector2TimeMS = 0;
  deltaToCarInFrontMS = 0;
  deltaToRaceLeaderMS = 0;
  safetyCarDelta = 0.0f;
  carPosition = 0;
  currentLapNum = 0;
  cornerCuttingWarnings = 0;

  // ============================================
  // CarSetupData
  // ============================================
  diffOnThrottle = 50;
  diffOffThrottle = 50;

  // ============================================
  // CarTelemetryData
  // ============================================
  speed = 0;
  throttle = 0.0f;
  brake = 0.0f;
  gear = 0;
  engineRPM = 0;
  drs = 0;
  revLightsPercent = 0;
  revLightsBitValue = 0;
  engineTemp = 0;
  suggestedGear = 0;

  for (uint8_t i = 0; i < 4; i++) {
    brakesTemp[i] = 0;
    tyresSurfaceTemp[i] = 0;
    tyresInnerTemp[i] = 0;
    tyresPressure[i] = 0.0f;
  }

  // ============================================
  // CarStatusData
  // ============================================
  frontBrakeBias = 50;
  fuelInTank = 0.0f;
  fuelRemainingLaps = 0.0f;
  tyresAgeLaps = 0;
  enginePowerICE = 0.0f;
  enginePowerMGUK = 0.0f;
  ersStoreEnergy = 0.0f;
  ersDeployMode = ERS_OFF;

  // ============================================
  // CarDamageData
  // ============================================
  for (uint8_t i = 0; i < 4; i++) {
    tyresWear[i] = 0.0f;
    tyresDamage[i] = 0;
    brakesDamage[i] = 0;
  }

  frontLeftWingDamage = 0;
  frontRightWingDamage = 0;
  rearWingDamage = 0;
  floorDamage = 0;
  diffuserDamage = 0;
  sidepodDamage = 0;
  drsFault = 0;
  ersFault = 0;
  gearBoxDamage = 0;
  engineDamage = 0;
  engineMGUHWear = 0;
  engineESWear = 0;
  engineCEWear = 0;
  engineICEWear = 0;
  engineMGUKWear = 0;
  engineTCWear = 0;
  engineBlown = 0;
  engineSeized = 0;

  // ============================================
  // Utility
  // ============================================
  packetsReceived = 0;
}

// ============================================
// UPDATE METHODS
// ============================================

void TelemetryModel::updateSessionData(const PacketSessionData* packet) {
  if (!packet) return;

  weather = packet->m_weather;
  trackTemperature = packet->m_trackTemperature;
  airTemperature = packet->m_airTemperature;
  sessionType = packet->m_sessionType;
  sessionTimeLeft = packet->m_sessionTimeLeft;
  safetyCarStatus = packet->m_safetyCarStatus;
  pitStopWindowIdealLap = packet->m_pitStopWindowIdealLap;
  totalLaps = packet->m_totalLaps;
}

void TelemetryModel::updateLapData(const PacketLapData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const LapData* data = &packet->m_lapData[playerIndex];

  lastLapTimeMS = data->m_lastLapTimeInMS;
  currentLapTimeMS = data->m_currentLapTimeInMS;
  sector1TimeMS = data->m_sector1TimeInMS;
  sector2TimeMS = data->m_sector2TimeInMS;
  deltaToCarInFrontMS = data->m_deltaToCarInFrontInMS;
  deltaToRaceLeaderMS = data->m_deltaToRaceLeaderInMS;
  safetyCarDelta = data->m_safetyCarDelta;
  carPosition = data->m_carPosition;
  currentLapNum = data->m_currentLapNum;
  cornerCuttingWarnings = data->m_cornerCuttingWarnings;
}

void TelemetryModel::updateCarSetup(const PacketCarSetupData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarSetupData* data = &packet->m_carSetups[playerIndex];

  diffOnThrottle = data->m_onThrottle;
  diffOffThrottle = data->m_offThrottle;
}

void TelemetryModel::updateTelemetry(const PacketCarTelemetryData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarTelemetryData* data = &packet->m_carTelemetryData[playerIndex];

  speed = data->m_speed;
  throttle = data->m_throttle;
  brake = data->m_brake;
  gear = data->m_gear;
  engineRPM = data->m_engineRPM;
  drs = data->m_drs;
  revLightsPercent = data->m_revLightsPercent;
  revLightsBitValue = data->m_revLightsBitValue;
  engineTemp = data->m_engineTemperature;
  suggestedGear = packet->m_suggestedGear;

  for (uint8_t i = 0; i < 4; i++) {
    brakesTemp[i] = data->m_brakesTemperature[i];
    tyresSurfaceTemp[i] = data->m_tyresSurfaceTemperature[i];
    tyresInnerTemp[i] = data->m_tyresInnerTemperature[i];
    tyresPressure[i] = data->m_tyresPressure[i];
  }

  packetsReceived++;
}

void TelemetryModel::updateCarStatus(const PacketCarStatusData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarStatusData* data = &packet->m_carStatusData[playerIndex];

  frontBrakeBias = data->m_frontBrakeBias;
  fuelInTank = data->m_fuelInTank;
  fuelRemainingLaps = data->m_fuelRemainingLaps;
  tyresAgeLaps = data->m_tyresAgeLaps;
  enginePowerICE = data->m_enginePowerICE;
  enginePowerMGUK = data->m_enginePowerMGUK;
  ersStoreEnergy = data->m_ersStoreEnergy;
  ersDeployMode = data->m_ersDeployMode;
}

void TelemetryModel::updateCarDamage(const PacketCarDamageData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarDamageData* data = &packet->m_carDamageData[playerIndex];

  for (uint8_t i = 0; i < 4; i++) {
    tyresWear[i] = data->m_tyresWear[i];
    tyresDamage[i] = data->m_tyresDamage[i];
    brakesDamage[i] = data->m_brakesDamage[i];
  }

  frontLeftWingDamage = data->m_frontLeftWingDamage;
  frontRightWingDamage = data->m_frontRightWingDamage;
  rearWingDamage = data->m_rearWingDamage;
  floorDamage = data->m_floorDamage;
  diffuserDamage = data->m_diffuserDamage;
  sidepodDamage = data->m_sidepodDamage;
  drsFault = data->m_drsFault;
  ersFault = data->m_ersFault;
  gearBoxDamage = data->m_gearBoxDamage;
  engineDamage = data->m_engineDamage;
  engineMGUHWear = data->m_engineMGUHWear;
  engineESWear = data->m_engineESWear;
  engineCEWear = data->m_engineCEWear;
  engineICEWear = data->m_engineICEWear;
  engineMGUKWear = data->m_engineMGUKWear;
  engineTCWear = data->m_engineTCWear;
  engineBlown = data->m_engineBlown;
  engineSeized = data->m_engineSeized;
}

// ============================================
// UTILITY
// ============================================

void TelemetryModel::formatLapTime(uint32_t timeMS, char* buffer) {
  uint32_t totalSeconds = timeMS / 1000;
  uint32_t minutes = totalSeconds / 60;
  uint32_t seconds = totalSeconds % 60;
  uint32_t millis = (timeMS % 1000) / 10;

  sprintf(buffer, "%d:%02d.%02d", minutes, seconds, millis);
}