#include "state_machine.h"
#include "configuration.h"
#include "wifi_handler.h"

#include <ESP8266WiFi.h>

cooboc::Configuration configuration{};
cooboc::WifiHandler wifiHandler{};
cooboc::StateMachine sm{configuration};

void setup()
{
    Serial.begin(115200);
    configuration.begin();
    sm.begin();
}

void loop()
{
    sm.tick();
}