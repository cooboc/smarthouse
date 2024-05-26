#include "n_button_relay_gear.h"
#include "data_def.h"
#include <vector>

namespace cooboc {

NButtonRelayGearInstance::NButtonRelayGearInstance(
    const std::uint8_t *gearConfig,
    const std::vector<ButtonConfig> &buttonsConfig,
    const std::vector<RelayConfig> &relaysConfig, const Led &&led)
    : led_{std::move(led)}, isReadyRestart_{false}, isReadyReset_{false} {
  static_assert(sizeof(NButtonRelayGearConfig) <=
                sizeof(Persistent::gearConfig));
  for (const auto &bc : buttonsConfig) {
    buttons_.emplace_back(Button{bc.pin});
  }
  for (const auto &rc : relaysConfig) {
    relays_.emplace_back(Relay{rc});
  }

  memcpy(&config_, gearConfig, sizeof(NButtonRelayGearConfig));
  Serial.print("config = ");
  Serial.println(config_.buttonRelayconnectivity);
}

void NButtonRelayGearInstance::setup() {
  Serial.print("NButtonRelayGear connectivity: ");
  Serial.println(static_cast<uint32_t>(config_.buttonRelayconnectivity));

  led_.setup();
  std::for_each(relays_.begin(), relays_.end(), [](Relay &r) { r.setup(); });

  for (std::size_t i{0U}; i < buttons_.size(); ++i) {
    buttons_[i].setup();
    buttons_[i].onPushDown([this, i]() {
      Serial.print(i);
      Serial.println(" button pushed down");
      led_.set(true);
      // check connectivity
      if (this->config_.buttonRelayconnectivity & (1 << i)) {
        this->relays_[i].toggle();
      }
      sendUserAction(UserActionType::BUTTON_PUSHED_DOWN, i);
    });
    buttons_[i].onPushUp([this, i]() { led_.set(false); });
  }

  // first button as the restart button

  buttons_[0].onRestartSatisfied([this]() {
    Serial.println("Restart Ready.");
    led_.startFastBlink();
    this->isReadyRestart_ = true;
  });

  buttons_[0].onResetSatified([this]() {
    Serial.println("Reset Ready.");
    led_.set(true);
    this->isReadyRestart_ = false;
    this->isReadyReset_ = true;
  });

  buttons_[0].onPushUp([this]() {
    led_.set(false);
    if (this->isReadyReset_) {
      this->resetPushedCallback_();
      this->isReadyReset_ = false;
    } else if (this->isReadyRestart_) {
      Serial.println("Bye.");
      ESP.restart();
      this->isReadyRestart_ = false;
    }
  });
}

void NButtonRelayGearInstance::tick() {
  led_.tick();
  std::for_each(buttons_.begin(), buttons_.end(), [](Button &b) { b.tick(); });
}

void NButtonRelayGearInstance::fillStatus(std::uint8_t *buffer,
                                          std::size_t length) const {
  std::memset(buffer, 0U, length);
  std::uint8_t statusByte{0U};
  for (std::size_t i{0U}; i < buttons_.size(); ++i) {
    if (buttons_[i].isPushed()) {
      statusByte |= (1U << i);
    }
  }
  buffer[0] = statusByte;
  buffer[1] = makeRelayStatusByte();
}

std::uint8_t NButtonRelayGearInstance::makeRelayStatusByte() const {
  std::uint8_t statusByte{0U};
  for (std::size_t i{0U}; i < relays_.size(); ++i) {
    if (relays_[i].isClosed()) {
      statusByte |= (1U << i);
    }
  }
  return statusByte;
}

void NButtonRelayGearInstance::onServerRequest(const ServerRequest &req) {
  Serial.println("handle server here");
  // type
  const std::uint8_t requestType = req[0];
  if (requestType == 0x01) { // Change status
    const std::uint8_t &portMask = req[1];
    const std::uint8_t &valueList = req[2];
    for (std::size_t i{0U}; i < relays_.size(); ++i) {
      if ((portMask & (1U << i)) != 0U) {
        const bool status = (valueList & (1U << i)) != 0U;
        relays_[i].set(status);
        Serial.print("server set ");
        Serial.print((int)(i));
        Serial.print(" to ");
        Serial.println((int)status);
      }
    }
  }
}

void NButtonRelayGearInstance::sendUserAction(UserActionType type,
                                              std::uint8_t value) {
  userActionPayload_[0] =
      static_cast<std::underlying_type_t<UserActionType>>(type);
  userActionPayload_[1] = value;
  userActionPayload_[2] = makeRelayStatusByte();
  userActionCallback_(userActionPayload_);
}

IGearInstance *
NButtonRelayGear::setupInstance(const std::uint8_t *gearConfig) const {
  return new NButtonRelayGearInstance(
      gearConfig, {ButtonConfig{12U}, ButtonConfig{14U}, ButtonConfig{5U}},
      {RelayConfig{4U}, RelayConfig{13U}, RelayConfig{15U}}, Led{2U, true});
}

} // namespace cooboc
