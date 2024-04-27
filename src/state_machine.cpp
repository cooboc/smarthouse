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
                           WifiHandler &wifi, RompClient &rompClient)
    : configuration_(configuration), wifi_(wifi), rompClient_{rompClient} {}

void StateMachine::begin() {
  Serial.println("begin state machine");

  if (configuration_.isWifiCredentialExists()) {
    transitTo(State::CONNECTING);
  } else {
    Serial.println("No wifi credential exists, go to web portal.");
    transitTo(State::CONFIG_PORTAL);
  }
}

void StateMachine::tick() {
  wifi_.tick();
  const WifiHandler::WifiStatus wifiStatus = wifi_.getWifiStatus();
  switch (state_) {
  case (State::FAILSAFE): {
    break;
  }
  case (State::CONFIG_PORTAL): {
    break;
  }
  case (State::CONNECTING): {
    handleConnectingTick(wifiStatus);
    break;
  }
  case (State::WORKING_WITH_WIFI): {
    if (wifiStatus != WifiHandler::WifiStatus::CONNECTED) {
      transitOut();
      transitTo(State::CONNECTING);
    } else {
      rompClient_.tick();
    }
    break;
  }
  default: {
    transitTo(State::FAILSAFE);
  }
  }
}

void StateMachine::transitOut() {
  switch (state_) {
  case (State::WORKING_WITH_WIFI): {
    rompClient_.end();
    break;
  }
  default: {
    break;
  }
  }
}
void StateMachine::transitTo(State newState) {
  state_ = newState;
  switch (state_) {
  case (State::FAILSAFE): {
    Serial.println("FAILSAFE");
    while (true)
      ;
    break;
  }
  case (State::CONFIG_PORTAL): {
    wifi_.startWebPortal();
    break;
  }
  case (State::CONNECTING): {
    wifi_.startConnectAp(configuration_.getWifiSsid(),
                         configuration_.getWifiPassword());
    Serial.print("Connecting: ");
    Serial.println(configuration_.getWifiSsid());
    break;
  }
  case (State::WORKING_WITH_WIFI): {
    Serial.print("IP address: ");
    Serial.println(wifi_.getLocalIP());
    rompClient_.begin();
    wifi_.startOtaServer();
    break;
  }
  default: {
    // TODO: LOG an error
    transitTo(State::FAILSAFE);
    break;
  }
  }
}

void StateMachine::handleConnectingTick(
    const WifiHandler::WifiStatus wifiStatus) {
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
}

} // namespace cooboc
