#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <Preferences.h>

// ==========================================
// 1. HARDWARE PINS
// ==========================================
// Display (ILI9341 - SPI)
const uint8_t PIN_TFT_CS = 15;
const uint8_t PIN_TFT_DC = 2;
const uint8_t PIN_TFT_RST = 4;
// SPI pins (hardware SPI): MOSI=23, MISO=19, SCK=18

// LEDs (WS2812B)
const uint8_t PIN_LED_STRIP = 13;
const uint8_t NUM_LEDS = 8;

// Audio
const uint8_t PIN_BUZZER = 25;

// Input Controls
const uint8_t PIN_BUTTON = 26;
const uint8_t PIN_JOY_X = 34;
const uint8_t PIN_JOY_Y = 35;
const uint8_t PIN_JOY_BTN = 27;

// ==========================================
// 2. DISPLAY SETTINGS
// ==========================================
const uint16_t SCREEN_WIDTH = 320;
const uint16_t SCREEN_HEIGHT = 240;

const uint8_t DISPLAY_ROTATION = 3;  // 0=Portrait, 1=Landscape, 2=Portrait180, 3=Landscape180
const uint8_t DEFAULT_BRIGHTNESS = 80;

// Color Definitions (RGB565)
const uint16_t COLOR_BLACK = 0x0000;
const uint16_t COLOR_WHITE = 0xFFFF;
const uint16_t COLOR_RED = 0xF800;
const uint16_t COLOR_GREEN = 0x07E0;
const uint16_t COLOR_BLUE = 0x001F;
const uint16_t COLOR_CYAN = 0x07FF;
const uint16_t COLOR_MAGENTA = 0xF81F;
const uint16_t COLOR_YELLOW = 0xFFE0;
const uint16_t COLOR_ORANGE = 0xFD20;
const uint16_t COLOR_DARKGREY = 0x7BEF;
const uint16_t COLOR_LIGHTGREY = 0xC618;

// Temperature Colors
const uint16_t COLOR_TEMP_COLD = 0x001F;      // Blue
const uint16_t COLOR_TEMP_OPTIMAL = 0x07E0;   // Green
const uint16_t COLOR_TEMP_WARM = 0xFFE0;      // Yellow
const uint16_t COLOR_TEMP_HOT = 0xFD20;       // Orange
const uint16_t COLOR_TEMP_CRITICAL = 0xF800;  // Red

// ==========================================
// 3. NETWORK SETTINGS
// ==========================================
const uint16_t UDP_PORT = 20777;
const char DEFAULT_AP_SSID[] = "Telemetry_Dashboard";
const char DEFAULT_AP_PASSWORD[] = "44168104";

const uint16_t PACKET_BUFFER_SIZE = 2048;
const uint32_t WIFI_TIMEOUT_MS = 10000;

// ==========================================
// 4. TIMING & UPDATE RATES
// ==========================================
const uint32_t SERIAL_BAUD_RATE = 115200;
const uint32_t DISPLAY_UPDATE_INTERVAL = 50;
const uint32_t INPUT_UPDATE_INTERVAL = 20;
const uint32_t LED_UPDATE_INTERVAL = 30;
const uint32_t AUDIO_UPDATE_INTERVAL = 50;

const uint32_t BOOT_SPLASH_DURATION = 3000;
const uint32_t CONFIG_ENTRY_HOLD_TIME = 3000;

const uint32_t BUTTON_DEBOUNCE_MS = 50;
const uint32_t BUTTON_LONG_PRESS_MS = 2000;

// ==========================================
// 5. JOYSTICK THRESHOLDS
// ==========================================
const uint16_t JOY_CENTER = 2048;
const uint16_t JOY_DEADZONE = 512;
const uint16_t JOY_THRESHOLD_LOW = JOY_CENTER - JOY_DEADZONE;
const uint16_t JOY_THRESHOLD_HIGH = JOY_CENTER + JOY_DEADZONE;

const uint32_t JOY_REPEAT_DELAY = 200;
const uint32_t JOY_REPEAT_RATE = 100;

// ==========================================
// 6. LED CONFIGURATION
// ==========================================
const uint8_t LED_BRIGHTNESS_DEFAULT = 40;
const uint8_t LED_BRIGHTNESS_MIN = 10;
const uint8_t LED_BRIGHTNESS_MAX = 150;

const uint8_t LED_REV_START_PERCENT = 70;
const uint8_t LED_REV_YELLOW_PERCENT = 85;
const uint8_t LED_REV_RED_PERCENT = 95;

const uint8_t LED_GREEN_COUNT = 3;
const uint8_t LED_YELLOW_COUNT = 3;
const uint8_t LED_RED_COUNT = 2;

// ==========================================
// 7. AUDIO CONFIGURATION
// ==========================================
const uint16_t FREQ_SHIFT_ALERT = 800;
const uint16_t FREQ_DRS_AVAILABLE = 1000;
const uint16_t FREQ_WARNING = 1500;
const uint16_t FREQ_CRITICAL = 2000;
const uint16_t FREQ_MENU_BEEP = 600;

const uint16_t DURATION_BEEP_SHORT = 100;
const uint16_t DURATION_BEEP_LONG = 300;
const uint16_t DURATION_BEEP_DOUBLE_GAP = 50;

const uint8_t BUZZER_VOLUME_OFF = 0;
const uint8_t BUZZER_VOLUME_LOW = 64;
const uint8_t BUZZER_VOLUME_MED = 128;
const uint8_t BUZZER_VOLUME_HIGH = 255;

// ==========================================
// 8. GAME TELEMETRY THRESHOLDS
// ==========================================
// Temperature Ranges (Celsius)
const uint16_t TEMP_ENGINE_OPTIMAL_MIN = 90;
const uint16_t TEMP_ENGINE_OPTIMAL_MAX = 110;
const uint16_t TEMP_ENGINE_CRITICAL = 120;

const uint16_t TEMP_BRAKE_OPTIMAL_MIN = 300;
const uint16_t TEMP_BRAKE_OPTIMAL_MAX = 700;
const uint16_t TEMP_BRAKE_CRITICAL = 900;

const uint8_t TEMP_TYRE_OPTIMAL_MIN = 80;
const uint8_t TEMP_TYRE_OPTIMAL_MAX = 100;
const uint8_t TEMP_TYRE_CRITICAL = 120;

// Fuel & Strategy
const float FUEL_LOW_LAPS_WARNING = 3.0;
const float FUEL_CRITICAL_LAPS = 1.0;

const uint8_t TYRE_WEAR_WARNING = 70;
const uint8_t TYRE_WEAR_CRITICAL = 90;

const uint8_t DAMAGE_WARNING = 30;
const uint8_t DAMAGE_CRITICAL = 60;

// ==========================================
// 9. F1 GAME PACKET STRUCTURES
// ==========================================
// Packet IDs (F1 2023 format)
const uint8_t PACKET_ID_MOTION = 0;
const uint8_t PACKET_ID_SESSION = 1;
const uint8_t PACKET_ID_LAP_DATA = 2;
const uint8_t PACKET_ID_EVENT = 3;
const uint8_t PACKET_ID_PARTICIPANTS = 4;
const uint8_t PACKET_ID_CAR_SETUPS = 5;
const uint8_t PACKET_ID_CAR_TELEMETRY = 6;
const uint8_t PACKET_ID_CAR_STATUS = 7;
const uint8_t PACKET_ID_FINAL_CLASSIFICATION = 8;
const uint8_t PACKET_ID_LOBBY_INFO = 9;
const uint8_t PACKET_ID_CAR_DAMAGE = 10;
const uint8_t PACKET_ID_SESSION_HISTORY = 11;
const uint8_t PACKET_ID_TYRE_SETS = 12;
const uint8_t PACKET_ID_MOTION_EX = 13;

const uint8_t MAX_CARS = 22;

// ==========================================
// 10. ENUMS & STATES
// ==========================================
enum ScreenMode {
  SCREEN_RACING_HUD = 0,
  SCREEN_CAR_STATUS = 1,
  SCREEN_PIT_STRATEGY = 2,
  SCREEN_SETTINGS_DISPLAY = 3,
  SCREEN_CONFIG_MENU = 4,
  SCREEN_COUNT = 5
};

enum JoystickDirection {
  JOY_NONE = 0,
  JOY_UP = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_PRESS = 5
};

enum DashboardWiFiMode {
  DASHBOARD_WIFI_AP = 0,
  DASHBOARD_WIFI_CLIENT = 1
};

enum BuzzerVolume {
  VOLUME_OFF = 0,
  VOLUME_LOW = 1,
  VOLUME_MED = 2,
  VOLUME_HIGH = 3
};

enum F1GameVersion {
  F1_AUTO_DETECT = 0,
  F1_2022 = 22,
  F1_2023 = 23,
  F1_2024 = 24,
  F1_2025 = 25
};

enum SessionType {
  SESSION_UNKNOWN = 0,
  SESSION_P1 = 1,
  SESSION_P2 = 2,
  SESSION_P3 = 3,
  SESSION_SHORT_P = 4,
  SESSION_Q1 = 5,
  SESSION_Q2 = 6,
  SESSION_Q3 = 7,
  SESSION_SHORT_Q = 8,
  SESSION_OSQ = 9,
  SESSION_RACE = 10,
  SESSION_RACE2 = 11,
  SESSION_RACE3 = 12,
  SESSION_TIME_TRIAL = 13
};

enum TrackStatus {
  TRACK_GREEN = 0,
  TRACK_YELLOW = 1,
  TRACK_SAFETY_CAR = 2,
  TRACK_VIRTUAL_SAFETY_CAR = 3,
  TRACK_RED = 4
};

enum DRSStatus {
  DRS_NOT_AVAILABLE = 0,
  DRS_AVAILABLE = 1,
  DRS_ACTIVE = 2
};

enum ERSMode {
  ERS_OFF = 0,
  ERS_MEDIUM = 1,
  ERS_HOTLAP = 2,
  ERS_OVERTAKE = 3
};

enum FuelMix {
  FUEL_LEAN = 0,
  FUEL_STANDARD = 1,
  FUEL_RICH = 2,
  FUEL_MAX = 3
};

enum TyreCompound {
  TYRE_C5 = 16,  // Soft
  TYRE_C4 = 17,  // Medium
  TYRE_C3 = 18,  // Hard
  TYRE_INTER = 7,
  TYRE_WET = 8
};

// ==========================================
// 11. DATA STRUCTURES
// ==========================================
struct __attribute__((packed)) PacketHeader {
  uint16_t m_packetFormat;
  uint8_t m_gameYear;
  uint8_t m_gameMajorVersion;
  uint8_t m_gameMinorVersion;
  uint8_t m_packetVersion;
  uint8_t m_packetId;
  uint64_t m_sessionUID;
  float m_sessionTime;
  uint32_t m_frameIdentifier;
  uint32_t m_overallFrameIdentifier;
  uint8_t m_playerCarIndex;
  uint8_t m_secondaryPlayerCarIndex;
};

struct __attribute__((packed)) CarTelemetryData {
  uint16_t m_speed;
  float m_throttle;
  float m_steer;
  float m_brake;
  uint8_t m_clutch;
  int8_t m_gear;
  uint16_t m_engineRPM;
  uint8_t m_drs;
  uint8_t m_revLightsPercent;
  uint16_t m_revLightsBitValue;
  uint16_t m_brakesTemperature[4];
  uint8_t m_tyresSurfaceTemperature[4];
  uint8_t m_tyresInnerTemperature[4];
  uint16_t m_engineTemperature;
  float m_tyresPressure[4];
  uint8_t m_surfaceType[4];
};

struct __attribute__((packed)) PacketCarTelemetryData {
  PacketHeader m_header;
  CarTelemetryData m_carTelemetryData[22];
  uint8_t m_mfdPanelIndex;
  uint8_t m_mfdPanelIndexSecondaryPlayer;
  int8_t m_suggestedGear;
};

struct __attribute__((packed)) LapData {
  uint32_t m_lastLapTimeInMS;
  uint32_t m_currentLapTimeInMS;
  uint16_t m_sector1TimeInMS;
  uint16_t m_sector2TimeInMS;
  uint16_t m_lapDistance;
  float m_totalDistance;
  float m_safetyCarDelta;
  uint8_t m_carPosition;
  uint8_t m_currentLapNum;
  uint8_t m_pitStatus;
  uint8_t m_numPitStops;
  uint8_t m_sector;
  uint8_t m_currentLapInvalid;
  uint8_t m_penalties;
  uint8_t m_warnings;
  uint8_t m_numUnservedDriveThroughPens;
  uint8_t m_numUnservedStopGoPens;
  uint8_t m_gridPosition;
  uint8_t m_driverStatus;
  uint8_t m_resultStatus;
  uint8_t m_pitLaneTimerActive;
  uint16_t m_pitLaneTimeInLaneInMS;
  uint16_t m_pitStopTimerInMS;
  uint8_t m_pitStopShouldServePen;
};

struct __attribute__((packed)) PacketLapData {
  PacketHeader m_header;
  LapData m_lapData[22];
  uint8_t m_timeTrialPBCarIdx;
  uint8_t m_timeTrialRivalCarIdx;
};

struct __attribute__((packed)) CarStatusData {
  uint8_t m_tractionControl;
  uint8_t m_antiLockBrakes;
  uint8_t m_fuelMix;
  uint8_t m_frontBrakeBias;
  uint8_t m_pitLimiterStatus;
  float m_fuelInTank;
  float m_fuelCapacity;
  float m_fuelRemainingLaps;
  uint16_t m_maxRPM;
  uint16_t m_idleRPM;
  uint8_t m_maxGears;
  uint8_t m_drsAllowed;
  uint16_t m_drsActivationDistance;
  uint8_t m_actualTyreCompound;
  uint8_t m_visualTyreCompound;
  uint8_t m_tyresAgeLaps;
  int8_t m_vehicleFiaFlags;
  float m_enginePowerICE;
  float m_enginePowerMGUK;
  float m_ersStoreEnergy;
  uint8_t m_ersDeployMode;
  float m_ersHarvestedThisLapMGUK;
  float m_ersHarvestedThisLapMGUH;
  float m_ersDeployedThisLap;
  uint8_t m_networkPaused;
};

struct __attribute__((packed)) PacketCarStatusData {
  PacketHeader m_header;
  CarStatusData m_carStatusData[22];
};

struct __attribute__((packed)) CarDamageData {
  float m_tyresWear[4];
  uint8_t m_tyresDamage[4];
  uint8_t m_brakesDamage[4];
  uint8_t m_frontLeftWingDamage;
  uint8_t m_frontRightWingDamage;
  uint8_t m_rearWingDamage;
  uint8_t m_floorDamage;
  uint8_t m_diffuserDamage;
  uint8_t m_sidepodDamage;
  uint8_t m_drsFault;
  uint8_t m_ersFault;
  uint8_t m_gearBoxDamage;
  uint8_t m_engineDamage;
  uint8_t m_engineMGUHWear;
  uint8_t m_engineESWear;
  uint8_t m_engineCEWear;
  uint8_t m_engineICEWear;
  uint8_t m_engineMGUKWear;
  uint8_t m_engineTCWear;
  uint8_t m_engineBlown;
  uint8_t m_engineSeized;
};

struct __attribute__((packed)) PacketCarDamageData {
  PacketHeader m_header;
  CarDamageData m_carDamageData[22];
};

struct __attribute__((packed)) MarshalZone {
  float m_zoneStart;
  int8_t m_zoneFlag;
};

struct __attribute__((packed)) WeatherForecastSample {
  uint8_t m_sessionType;
  uint8_t m_timeOffset;
  uint8_t m_weather;
  int8_t m_trackTemperature;
  int8_t m_trackTemperatureChange;
  int8_t m_airTemperature;
  int8_t m_airTemperatureChange;
  uint8_t m_rainPercentage;
};

struct __attribute__((packed)) PacketSessionData {
  PacketHeader m_header;
  uint8_t m_weather;
  int8_t m_trackTemperature;
  int8_t m_airTemperature;
  uint8_t m_totalLaps;
  uint16_t m_trackLength;
  uint8_t m_sessionType;
  int8_t m_trackId;
  uint8_t m_formula;
  uint16_t m_sessionTimeLeft;
  uint16_t m_sessionDuration;
  uint8_t m_pitSpeedLimit;
  uint8_t m_gamePaused;
  uint8_t m_isSpectating;
  uint8_t m_spectatorCarIndex;
  uint8_t m_sliProNativeSupport;
  uint8_t m_numMarshalZones;
  MarshalZone m_marshalZones[21];
  uint8_t m_safetyCarStatus;
  uint8_t m_networkGame;
  uint8_t m_numWeatherForecastSamples;
  WeatherForecastSample m_weatherForecastSamples[56];
  uint8_t m_forecastAccuracy;
  uint8_t m_aiDifficulty;
  uint32_t m_seasonLinkIdentifier;
  uint32_t m_weekendLinkIdentifier;
  uint32_t m_sessionLinkIdentifier;
  uint8_t m_pitStopWindowIdealLap;
  uint8_t m_pitStopWindowLatestLap;
  uint8_t m_pitStopRejoinPosition;
  uint8_t m_steeringAssist;
  uint8_t m_brakingAssist;
  uint8_t m_gearboxAssist;
  uint8_t m_pitAssist;
  uint8_t m_pitReleaseAssist;
  uint8_t m_ERSAssist;
  uint8_t m_DRSAssist;
  uint8_t m_dynamicRacingLine;
  uint8_t m_dynamicRacingLineType;
  uint8_t m_gameMode;
  uint8_t m_ruleSet;
  uint32_t m_timeOfDay;
  uint8_t m_sessionLength;
};

// ==========================================
// 12. USER SETTINGS (Stored in Flash)
// ==========================================
struct UserSettings {
  // Display
  uint8_t displayBrightness;
  uint8_t displayRotation;
  bool useMetricUnits;
  uint8_t defaultScreen;

  // Sound
  uint8_t buzzerVolume;
  bool shiftAlertEnabled;
  bool drsAlertEnabled;
  bool warningAlertsEnabled;

  // LEDs
  uint8_t ledBrightness;
  uint8_t ledRevStartPercent;
  uint8_t ledRevEndPercent;

  // Network
  uint8_t wifiMode;
  char wifiSSID[32];
  char wifiPassword[64];

  // Game
  uint8_t gameVersion;
  int8_t playerIndexOverride;

  // Internal
  uint32_t settingsVersion;
};

// Default settings
const UserSettings DEFAULT_SETTINGS = {
  .displayBrightness = 80,
  .displayRotation = 3,
  .useMetricUnits = true,
  .defaultScreen = 0,

  .buzzerVolume = VOLUME_MED,
  .shiftAlertEnabled = true,
  .drsAlertEnabled = true,
  .warningAlertsEnabled = true,

  .ledBrightness = LED_BRIGHTNESS_DEFAULT,
  .ledRevStartPercent = LED_REV_START_PERCENT,
  .ledRevEndPercent = LED_REV_RED_PERCENT,

  .wifiMode = DASHBOARD_WIFI_AP,
  .wifiSSID = "Telemetry_Dashboard",
  .wifiPassword = "44168104",

  .gameVersion = F1_AUTO_DETECT,
  .playerIndexOverride = -1,

  .settingsVersion = 1
};

// ==========================================
// 13. PREFERENCES KEY NAMES
// ==========================================
const char PREFS_NAMESPACE[] = "dashboard";
const char PREFS_KEY_BRIGHTNESS[] = "dispBright";
const char PREFS_KEY_ROTATION[] = "dispRot";
const char PREFS_KEY_UNITS[] = "units";
const char PREFS_KEY_DEFAULT_SCREEN[] = "defScreen";
const char PREFS_KEY_BUZZER_VOL[] = "buzzVol";
const char PREFS_KEY_SHIFT_ALERT[] = "shiftAlert";
const char PREFS_KEY_DRS_ALERT[] = "drsAlert";
const char PREFS_KEY_WARN_ALERT[] = "warnAlert";
const char PREFS_KEY_LED_BRIGHT[] = "ledBright";
const char PREFS_KEY_LED_START[] = "ledStart";
const char PREFS_KEY_LED_END[] = "ledEnd";
const char PREFS_KEY_WIFI_MODE[] = "wifiMode";
const char PREFS_KEY_WIFI_SSID[] = "wifiSSID";
const char PREFS_KEY_WIFI_PASS[] = "wifiPass";
const char PREFS_KEY_GAME_VER[] = "gameVer";
const char PREFS_KEY_PLAYER_IDX[] = "playerIdx";


#endif  // CONFIG_H