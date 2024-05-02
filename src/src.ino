#include "configuration.h"

#include "i_gear.h"
#include "romp_client.h"
#include "state_machine.h"
#include "wifi_handler.h"
#include <ESP8266WiFi.h>

cooboc::Configuration configuration{};
// cooboc::Hal hal{configuration};
cooboc::WifiHandler wifiHandler{configuration};
cooboc::RompClient romp{configuration};
cooboc::StateMachine sm{configuration, wifiHandler, romp};

void setup() {
  Serial.begin(115200);
  Serial.println("\r\n\r\n");
  configuration.begin();

  cooboc::IGearInstance *gear = configuration.getGearInstance();
  if (gear != nullptr) {
    gear->setup();
    gear->onResetPushed([]() {
      Serial.println("user push reset");
      configuration.resetSettings();
      ESP.restart();
    });
  } else {
    Serial.println("[WARN] gear instance null");
  }

  // hal.setup();
  sm.begin();
}

void loop() {
  cooboc::IGearInstance *gear = configuration.getGearInstance();
  if (gear != nullptr) {
    gear->tick();
  }
  // hal.tick();
  sm.tick();
}