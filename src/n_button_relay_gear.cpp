#include "n_button_relay_gear.h"
#include "data_def.h"
#include <vector>

namespace cooboc {

NButtonRelayGearInstance::NButtonRelayGearInstance(
    const std::uint8_t *gearConfig)
    : buttons_{Button{12U}, Button{14U}, Button{5U}},
      relays_{Relay{4U}, Relay{13U}, Relay{15U}}, leds_{Led{2U, true}},
      isReadyRestart_{false}, isReadyReset_{false} {
  static_assert(sizeof(NButtonRelayGearConfig) <=
                sizeof(Persistent::gearConfig));
  memcpy(&config_, gearConfig, sizeof(NButtonRelayGearConfig));
  Serial.print("config = ");
  Serial.println(config_.buttonRelayconnectivity);
}

void NButtonRelayGearInstance::setup() {
  Serial.print("NButtonRelayGear connectivity: ");
  Serial.println(static_cast<uint32_t>(config_.buttonRelayconnectivity));

  std::for_each(leds_.begin(), leds_.end(), [](Led &l) { l.setup(); });
  std::for_each(relays_.begin(), relays_.end(), [](Relay &r) { r.setup(); });

  for (std::size_t i{0U}; i < buttons_.size(); ++i) {
    buttons_[i].setup();
    buttons_[i].onPushDown([this, i]() {
      Serial.print(i);
      Serial.println(" button pushed down");
      this->leds_[0].set(true);
      // check connectivity
      if (this->config_.buttonRelayconnectivity & (1 << i)) {
        this->relays_[i].toggle();
      }
      userActionPayload_[0] = 0;
      userActionPayload_[1] = i;
      userActionCallback_(userActionPayload_);
    });
    buttons_[i].onPushUp([this, i]() { this->leds_[0].set(false); });
  }

  // first button as the restart button

  buttons_[0].onRestartSatisfied([this]() {
    Serial.println("Restart Ready.");
    this->leds_[0].startFastBlink();
    this->isReadyRestart_ = true;
  });

  buttons_[0].onResetSatified([this]() {
    Serial.println("Reset Ready.");
    this->leds_[0].set(true);
    this->isReadyRestart_ = false;
    this->isReadyReset_ = true;
  });

  buttons_[0].onPushUp([this]() {
    this->leds_[0].set(false);
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
  std::for_each(leds_.begin(), leds_.end(), [](Led &l) { l.tick(); });
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

  statusByte = 0U;
  for (std::size_t i{0U}; i < relays_.size(); ++i) {
    if (relays_[i].isClosed()) {
      statusByte |= (1U << i);
    }
  }
  buffer[1] = statusByte;
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

IGearInstance *
NButtonRelayGear::setupInstance(const std::uint8_t *gearConfig) const {
  return new NButtonRelayGearInstance(gearConfig);
}

} // namespace cooboc
