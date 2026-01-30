#include "Controller.h"

TelemetryController::TelemetryController(TelemetryModel* m, TelemetryView* v, WiFiUDP* u) {
  model = m;
  view = v;
  udp = u;

  lastDisplayUpdate = 0;
  lastInputCheck = 0;

  lastButtonState = HIGH;
  buttonPressTime = 0;
}

void TelemetryController::init() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial.println("Telemetry Dashboard Starting...");

  view->init();

  setupWiFi();

  udp->begin(UDP_PORT);
  Serial.print("UDP listening on port: ");
  Serial.println(UDP_PORT);

  delay(BOOT_SPLASH_DURATION);

  view->drawStaticLayout();
}

void TelemetryController::setupWiFi() {
  UserSettings* settings = model->getSettings();

  if (settings->wifiMode == DASHBOARD_WIFI_AP) {
    Serial.println("Starting WiFi AP...");
    WiFi.softAP(settings->wifiSSID, settings->wifiPassword);

    IPAddress ip = WiFi.softAPIP();
    Serial.print("AP IP: ");
    Serial.println(ip);
  } else {
    Serial.println("Connecting to WiFi...");
    WiFi.begin(settings->wifiSSID, settings->wifiPassword);

    uint32_t startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < WIFI_TIMEOUT_MS) {
      delay(500);
      Serial.print(".");
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nWiFi Connected!");
      Serial.print("IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nWiFi Connection Failed! Falling back to AP mode...");
      WiFi.softAP(DEFAULT_AP_SSID, DEFAULT_AP_PASSWORD);
      Serial.print("AP IP: ");
      Serial.println(WiFi.softAPIP());
    }
  }
}

void TelemetryController::update() {
  uint32_t currentTime = millis();

  if (currentTime - lastInputCheck >= INPUT_UPDATE_INTERVAL) {
    handleInput();
    lastInputCheck = currentTime;
  }

  handleNetworkPackets();

  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    model->updateConnectionStatus();
    view->render();
    lastDisplayUpdate = currentTime;
  }
}

void TelemetryController::handleInput() {
  if (checkButton()) {
    Serial.println("Button pressed - switching screen");
    model->nextScreen();
  }
}

bool TelemetryController::checkButton() {
  bool currentState = digitalRead(PIN_BUTTON);

  if (currentState != lastButtonState) {
    delay(BUTTON_DEBOUNCE_MS);
    currentState = digitalRead(PIN_BUTTON);

    if (currentState != lastButtonState) {
      lastButtonState = currentState;

      if (currentState == LOW) {
        buttonPressTime = millis();
        return true;
      }
    }
  }

  return false;
}

void TelemetryController::handleNetworkPackets() {
  int packetSize = udp->parsePacket();

  if (packetSize > 0) {
    int len = udp->read(packetBuffer, PACKET_BUFFER_SIZE);

    if (len > 0) {
      processPacket(packetBuffer, len);
    }
  }
}

void TelemetryController::processPacket(uint8_t* buffer, int size) {
  if (size < sizeof(PacketHeader)) {
    return;
  }

  PacketHeader* header = (PacketHeader*)buffer;

  uint8_t playerIndex = header->m_playerCarIndex;
  UserSettings* settings = model->getSettings();

  if (settings->playerIndexOverride >= 0) {
    playerIndex = settings->playerIndexOverride;
  }

  switch (header->m_packetId) {
    case PACKET_ID_CAR_TELEMETRY:
      if (size >= sizeof(PacketHeader) + sizeof(CarTelemetryData)) {
        PacketCarTelemetryData* packet = (PacketCarTelemetryData*)buffer;
        model->updateTelemetry(packet, playerIndex);
      }
      break;

    case PACKET_ID_LAP_DATA:
      if (size >= sizeof(PacketHeader) + sizeof(LapData)) {
        PacketLapData* packet = (PacketLapData*)buffer;
        model->updateLapData(packet, playerIndex);
      }
      break;

    case PACKET_ID_CAR_STATUS:
      if (size >= sizeof(PacketHeader) + sizeof(CarStatusData)) {
        PacketCarStatusData* packet = (PacketCarStatusData*)buffer;
        model->updateCarStatus(packet, playerIndex);
      }
      break;

    case PACKET_ID_CAR_DAMAGE:
      if (size >= sizeof(PacketHeader) + sizeof(CarDamageData)) {
        PacketCarDamageData* packet = (PacketCarDamageData*)buffer;
        model->updateCarDamage(packet, playerIndex);
      }
      break;

    case PACKET_ID_SESSION:
      if (size >= sizeof(PacketHeader) + 100) {
        PacketSessionData* packet = (PacketSessionData*)buffer;
        model->updateSessionData(packet);
      }
      break;

    default:
      break;
  }
}