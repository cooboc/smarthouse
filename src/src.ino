#include "configuration.h"
#include "hal.h"
#include "romp_client.h"
#include "state_machine.h"
#include "wifi_handler.h"
#include <ESP8266WiFi.h>

cooboc::Configuration configuration{};
cooboc::Hal hal{configuration};
cooboc::WifiHandler wifiHandler{configuration};
cooboc::RompClient romp{configuration};
cooboc::StateMachine sm{configuration, wifiHandler, romp};

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\r\n");
  configuration.begin();
  hal.setup();
  sm.begin();
}

void loop() {
  hal.tick();
  sm.tick();
}