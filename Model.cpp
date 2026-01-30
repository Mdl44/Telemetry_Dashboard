#include "Model.h"
#include <Preferences.h>

TelemetryModel::TelemetryModel() {
  speed = 0;
  gear = 0;
  engineRPM = 0;
  revLightsPercent = 0;

  currentLapTimeMS = 0;
  lastLapTimeMS = 0;
  deltaTime = 0.0f;
  currentLapNum = 0;
  totalLaps = 0;

  carPosition = 0;
  totalCars = 22;

  sessionType = SESSION_UNKNOWN;
  trackStatus = TRACK_GREEN;

  engineTemp = 0;
  for (uint8_t i = 0; i < 4; i++) {
    brakesTemp[i] = 0;
    tyresSurfaceTemp[i] = 0;
    tyresInnerTemp[i] = 0;
    tyresPressure[i] = 0.0f;
    tyresWear[i] = 0.0f;
  }

  fuelInTank = 0.0f;
  fuelCapacity = 0.0f;
  fuelRemainingLaps = 0.0f;
  ersStoreEnergy = 0.0f;
  ersDeployMode = ERS_OFF;

  drsAvailable = DRS_NOT_AVAILABLE;
  drsActive = DRS_NOT_AVAILABLE;

  frontWingDamage = 0;
  rearWingDamage = 0;
  engineDamage = 0;

  currentScreen = SCREEN_RACING_HUD;
  inConfigMode = false;

  packetsReceived = 0;
  lastPacketTime = 0;
  connected = false;

  loadSettings();
}

void TelemetryModel::updateTelemetry(const PacketCarTelemetryData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarTelemetryData* data = &packet->m_carTelemetryData[playerIndex];

  speed = data->m_speed;
  gear = data->m_gear;
  engineRPM = data->m_engineRPM;
  revLightsPercent = data->m_revLightsPercent;
  drsActive = data->m_drs;

  engineTemp = data->m_engineTemperature;
  for (uint8_t i = 0; i < 4; i++) {
    brakesTemp[i] = data->m_brakesTemperature[i];
    tyresSurfaceTemp[i] = data->m_tyresSurfaceTemperature[i];
    tyresInnerTemp[i] = data->m_tyresInnerTemperature[i];
    tyresPressure[i] = data->m_tyresPressure[i];
  }

  packetsReceived++;
  lastPacketTime = millis();
  connected = true;
}

void TelemetryModel::updateLapData(const PacketLapData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const LapData* data = &packet->m_lapData[playerIndex];

  currentLapTimeMS = data->m_currentLapTimeInMS;
  lastLapTimeMS = data->m_lastLapTimeInMS;
  deltaTime = data->m_safetyCarDelta;
  currentLapNum = data->m_currentLapNum;
  carPosition = data->m_carPosition;
}

void TelemetryModel::updateCarStatus(const PacketCarStatusData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarStatusData* data = &packet->m_carStatusData[playerIndex];

  fuelInTank = data->m_fuelInTank;
  fuelCapacity = data->m_fuelCapacity;
  fuelRemainingLaps = data->m_fuelRemainingLaps;
  ersStoreEnergy = data->m_ersStoreEnergy;
  ersDeployMode = data->m_ersDeployMode;
  drsAvailable = data->m_drsAllowed;
}

void TelemetryModel::updateCarDamage(const PacketCarDamageData* packet, uint8_t playerIndex) {
  if (!packet || playerIndex >= MAX_CARS) return;

  const CarDamageData* data = &packet->m_carDamageData[playerIndex];

  for (uint8_t i = 0; i < 4; i++) {
    tyresWear[i] = data->m_tyresWear[i];
  }

  frontWingDamage = (data->m_frontLeftWingDamage + data->m_frontRightWingDamage) / 2;
  rearWingDamage = data->m_rearWingDamage;
  engineDamage = data->m_engineDamage;
}

void TelemetryModel::updateSessionData(const PacketSessionData* packet) {
  if (!packet) return;

  totalLaps = packet->m_totalLaps;
  sessionType = packet->m_sessionType;
  trackStatus = packet->m_safetyCarStatus;
}

void TelemetryModel::formatLapTime(uint32_t timeMS, char* buffer) {
  uint32_t totalSeconds = timeMS / 1000;
  uint32_t minutes = totalSeconds / 60;
  uint32_t seconds = totalSeconds % 60;
  uint32_t millis = (timeMS % 1000) / 10;

  sprintf(buffer, "%d:%02d.%02d", minutes, seconds, millis);
}

void TelemetryModel::nextScreen() {
  uint8_t nextScreen = (uint8_t)currentScreen + 1;
  if (nextScreen >= SCREEN_CONFIG_MENU) nextScreen = 0;
  currentScreen = (ScreenMode)nextScreen;
}

void TelemetryModel::previousScreen() {
  if (currentScreen == 0) {
    currentScreen = (ScreenMode)(SCREEN_CONFIG_MENU - 1);
  } else {
    currentScreen = (ScreenMode)((uint8_t)currentScreen - 1);
  }
}

void TelemetryModel::loadSettings() {
  Preferences prefs;
  prefs.begin(PREFS_NAMESPACE, true);

  settings.displayBrightness = prefs.getUChar(PREFS_KEY_BRIGHTNESS, DEFAULT_SETTINGS.displayBrightness);
  settings.displayRotation = prefs.getUChar(PREFS_KEY_ROTATION, DEFAULT_SETTINGS.displayRotation);
  settings.useMetricUnits = prefs.getBool(PREFS_KEY_UNITS, DEFAULT_SETTINGS.useMetricUnits);
  settings.defaultScreen = prefs.getUChar(PREFS_KEY_DEFAULT_SCREEN, DEFAULT_SETTINGS.defaultScreen);

  settings.buzzerVolume = prefs.getUChar(PREFS_KEY_BUZZER_VOL, DEFAULT_SETTINGS.buzzerVolume);
  settings.shiftAlertEnabled = prefs.getBool(PREFS_KEY_SHIFT_ALERT, DEFAULT_SETTINGS.shiftAlertEnabled);
  settings.drsAlertEnabled = prefs.getBool(PREFS_KEY_DRS_ALERT, DEFAULT_SETTINGS.drsAlertEnabled);
  settings.warningAlertsEnabled = prefs.getBool(PREFS_KEY_WARN_ALERT, DEFAULT_SETTINGS.warningAlertsEnabled);

  settings.ledBrightness = prefs.getUChar(PREFS_KEY_LED_BRIGHT, DEFAULT_SETTINGS.ledBrightness);
  settings.ledRevStartPercent = prefs.getUChar(PREFS_KEY_LED_START, DEFAULT_SETTINGS.ledRevStartPercent);
  settings.ledRevEndPercent = prefs.getUChar(PREFS_KEY_LED_END, DEFAULT_SETTINGS.ledRevEndPercent);

  settings.wifiMode = prefs.getUChar(PREFS_KEY_WIFI_MODE, DEFAULT_SETTINGS.wifiMode);
  prefs.getString(PREFS_KEY_WIFI_SSID, settings.wifiSSID, 32);
  prefs.getString(PREFS_KEY_WIFI_PASS, settings.wifiPassword, 64);

  if (settings.wifiSSID[0] == '\0') {
    strcpy(settings.wifiSSID, DEFAULT_SETTINGS.wifiSSID);
    strcpy(settings.wifiPassword, DEFAULT_SETTINGS.wifiPassword);
  }

  settings.gameVersion = prefs.getUChar(PREFS_KEY_GAME_VER, DEFAULT_SETTINGS.gameVersion);
  settings.playerIndexOverride = prefs.getChar(PREFS_KEY_PLAYER_IDX, DEFAULT_SETTINGS.playerIndexOverride);

  prefs.end();
}

void TelemetryModel::saveSettings() {
  Preferences prefs;
  prefs.begin(PREFS_NAMESPACE, false);

  prefs.putUChar(PREFS_KEY_BRIGHTNESS, settings.displayBrightness);
  prefs.putUChar(PREFS_KEY_ROTATION, settings.displayRotation);
  prefs.putBool(PREFS_KEY_UNITS, settings.useMetricUnits);
  prefs.putUChar(PREFS_KEY_DEFAULT_SCREEN, settings.defaultScreen);

  prefs.putUChar(PREFS_KEY_BUZZER_VOL, settings.buzzerVolume);
  prefs.putBool(PREFS_KEY_SHIFT_ALERT, settings.shiftAlertEnabled);
  prefs.putBool(PREFS_KEY_DRS_ALERT, settings.drsAlertEnabled);
  prefs.putBool(PREFS_KEY_WARN_ALERT, settings.warningAlertsEnabled);

  prefs.putUChar(PREFS_KEY_LED_BRIGHT, settings.ledBrightness);
  prefs.putUChar(PREFS_KEY_LED_START, settings.ledRevStartPercent);
  prefs.putUChar(PREFS_KEY_LED_END, settings.ledRevEndPercent);

  prefs.putUChar(PREFS_KEY_WIFI_MODE, settings.wifiMode);
  prefs.putString(PREFS_KEY_WIFI_SSID, settings.wifiSSID);
  prefs.putString(PREFS_KEY_WIFI_PASS, settings.wifiPassword);

  prefs.putUChar(PREFS_KEY_GAME_VER, settings.gameVersion);
  prefs.putChar(PREFS_KEY_PLAYER_IDX, settings.playerIndexOverride);

  prefs.end();
}

void TelemetryModel::updateConnectionStatus() {
  if (millis() - lastPacketTime > 2000) {
    connected = false;
  }
}