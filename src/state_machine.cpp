#include "state_machine.h"
#include "configuration.h"
#include "wifi_handler.h"

namespace cooboc {
// The reason why I didn't put initializing code in the constructor is I want
// to keep the Arduino style, a.k.a the setup() and loop() functions. But I
// don't know what the underneath framework will do when finish a loop()
// operation. To make the variable visible to both two function. I have to make
// a global variable. And the constructor of global variable will called before
// the setup() function is called, and even before the hardware is initialized.
// It may cause undefined behaviour. To avoid this situation, I have to make
// this compromise. Move all initialization code to begin() function.
StateMachine::StateMachine(const Configuration &configuration,
                           const WifiHandler &wifi)
    : configuration_(configuration), wifi_(wifi) {}

void StateMachine::begin() { transitTo(State::CONNECTING); }

void StateMachine::tick() {
  const WifiHandler::WifiStatus wifiStatus = wifi_.getWifiStatus();
  switch (state_) {
  case (State::FAILSAFE): {
    break;
  }
  case (State::CONNECTING): {
    switch (wifiStatus) {
    case (WifiHandler::WifiStatus::CONNECTING): {
      // Do nothing
      break;
    }
    case (WifiHandler::WifiStatus::CONNECTED): {
      transitOut();
      transitTo(State::WORKING_WITH_WIFI);
      break;
    }
    default: {
      Serial.println("TODO, handle error status");
      // TOOD: error status code
      // delay 2s to avoid connecting flood while AP is off
      break;
    }
    }

    break;
  }
  case (State::WORKING_WITH_WIFI): {
    if (wifiStatus != WifiHandler::WifiStatus::CONNECTED) {
      transitOut();
      transitTo(State::CONNECTING);
    }
    break;
  }
  default: {
    transitTo(State::FAILSAFE);
  }
  }
}

void StateMachine::transitOut() {}
void StateMachine::transitTo(State newState) {
  state_ = newState;
  switch (state_) {
  case (State::FAILSAFE): {
    while (true)
      ;
    break;
  }
  case (State::CONNECTING): {
    wifi_.startConnectAp(configuration_.getWifiSsid(),
                         configuration_.getWifiPassword());
    Serial.print("Connecting");
    break;
  }
  case (State::WORKING_WITH_WIFI): {
    Serial.println("TODO switch to WORKING_STANDALONE, connect server");
    break;
  }
  default: {
    // TODO: LOG an error
    transitTo(State::FAILSAFE);
    break;
  }
  }
}
} // namespace cooboc
