#include "Controller.h"

TelemetryController::TelemetryController(TelemetryModel* m, TelemetryView* v, WiFiUDP* u) {
  model = m;
  view = v;
  udp = u;

  bootState = BOOT_ANIMATION;
  bootStartTime = 0;
  firstPacketReceived = false;
  lastDisplayUpdate = 0;

  lastGear = -99;
  lastDRSAvailable = 0;
}

void TelemetryController::init() {
  Serial.begin(SERIAL_BAUD_RATE);

  pinMode(PIN_BUTTON, INPUT_PULLUP);
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, LOW);

  view->init();
  setupWiFi();

  udp->begin(UDP_PORT);

  bootStartTime = millis();
  bootState = BOOT_ANIMATION;
}

void TelemetryController::setupWiFi() {
  WiFi.softAP(WIFI_AP_SSID, WIFI_AP_PASSWORD);

  IPAddress ip = WiFi.softAPIP();
}

void TelemetryController::update() {
  uint32_t currentTime = millis();
  uint32_t elapsed = currentTime - bootStartTime;

  if (bootState == BOOT_ANIMATION) {
    view->drawBootAnimation(elapsed);
    if (elapsed >= BOOT_ANIMATION_DURATION) {
      bootState = BOOT_WAITING;
      view->drawBootInfo(WiFi.softAPIP());
    }
    return;
  }

  if (bootState == BOOT_WAITING) {
    view->drawBootInfo(WiFi.softAPIP());

    handleNetworkPackets();

    if (firstPacketReceived) {
      bootState = BOOT_COMPLETE;
      view->drawLayout();
    }
    return;
  }

  handleButtonPress();

  handleNetworkPackets();

  if (WiFi.softAPgetStationNum() == 0 && firstPacketReceived) {
    bootState = BOOT_WAITING;
    firstPacketReceived = false;
    view->resetBootInfo();
    return;
  }

  checkBuzzerTriggers();

  if (currentTime - lastDisplayUpdate >= DISPLAY_UPDATE_INTERVAL) {
    view->render();
    lastDisplayUpdate = currentTime;
  }
}

void TelemetryController::handleButtonPress() {
  static bool lastButtonState = HIGH;

  bool reading = digitalRead(PIN_BUTTON);

  if (reading == LOW && lastButtonState == HIGH) {
    view->nextScreen();
    lastButtonState = LOW;
    return;
  }

  if (reading == HIGH) {
    lastButtonState = HIGH;
  }
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
  if (!firstPacketReceived) {
    firstPacketReceived = true;
  }

  if (size < sizeof(PacketHeader)) {
    return;
  }

  PacketHeader* header = (PacketHeader*)buffer;
  uint8_t playerIndex = header->m_playerCarIndex;

  switch (header->m_packetId) {
    case PACKET_ID_CAR_TELEMETRY:
      if (size >= sizeof(PacketCarTelemetryData)) {
        model->updateTelemetry((PacketCarTelemetryData*)buffer, playerIndex);
      }
      break;

    case PACKET_ID_LAP_DATA:
      if (size >= sizeof(PacketLapData)) {
        model->updateLapData((PacketLapData*)buffer, playerIndex);
      }
      break;

    case PACKET_ID_CAR_STATUS:
      if (size >= sizeof(PacketCarStatusData)) {
        model->updateCarStatus((PacketCarStatusData*)buffer, playerIndex);
      }
      break;

    case PACKET_ID_CAR_DAMAGE:
      if (size >= sizeof(PacketCarDamageData)) {
        model->updateCarDamage((PacketCarDamageData*)buffer, playerIndex);
      }
      break;

    case PACKET_ID_SESSION:
      if (size >= sizeof(PacketSessionData)) {
        model->updateSessionData((PacketSessionData*)buffer);
      }
      break;

    case PACKET_ID_CAR_SETUPS:
      if (size >= sizeof(PacketCarSetupData)) {
        model->updateCarSetup((PacketCarSetupData*)buffer, playerIndex);
      }
      break;
    default:
      break;
  }
}
void TelemetryController::playBuzzerBeep(uint8_t duration) {
  tone(PIN_BUZZER, 2000, duration);
  delay(duration);
  noTone(PIN_BUZZER);
}

void TelemetryController::checkBuzzerTriggers() {
  int8_t currentGear = model->getGear();
  uint8_t currentDRS = model->getDRS();

  if (lastGear != -99 && currentGear != lastGear) {
    if (currentGear > 0 && lastGear > 0) {
      playBuzzerBeep(50);
    }
  }

  if (lastDRSAvailable == 0 && currentDRS == 1) {
    playBuzzerBeep(100);
  }

  lastGear = currentGear;
  lastDRSAvailable = currentDRS;
}