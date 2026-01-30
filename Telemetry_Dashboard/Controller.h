#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Config.h"
#include "Model.h"
#include "View.h"

class TelemetryController {
private:
  TelemetryModel* model;
  TelemetryView* view;
  WiFiUDP* udp;

  uint32_t lastDisplayUpdate;
  uint32_t lastInputCheck;

  bool lastButtonState;
  uint32_t buttonPressTime;

  uint8_t packetBuffer[PACKET_BUFFER_SIZE];

  void handleNetworkPackets();
  void handleInput();
  bool checkButton();
  void processPacket(uint8_t* buffer, int size);

public:
  TelemetryController(TelemetryModel* m, TelemetryView* v, WiFiUDP* u);

  void init();
  void update();
  void setupWiFi();
};

#endif