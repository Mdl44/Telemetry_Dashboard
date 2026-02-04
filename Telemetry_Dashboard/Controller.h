#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "Config.h"
#include "Model.h"
#include "View.h"

class TelemetryController {
  TelemetryModel* model;
  TelemetryView* view;
  WiFiUDP* udp;

  enum BootState {
    BOOT_ANIMATION,
    BOOT_WAITING,
    BOOT_COMPLETE
  };

  BootState bootState;
  uint32_t bootStartTime;
  bool firstPacketReceived;

  uint32_t lastDisplayUpdate;
  uint8_t packetBuffer[PACKET_BUFFER_SIZE];

  int8_t lastGear;
  uint8_t lastDRSAvailable;

  void handleNetworkPackets();
  void processPacket(uint8_t* buffer, int size);
  void setupWiFi();
  void handleButtonPress();
  void checkBuzzerTriggers();
  void playBuzzerBeep(uint8_t duration = 50);

public:
  TelemetryController(TelemetryModel* m, TelemetryView* v, WiFiUDP* u);

  void init();
  void update();
};

#endif