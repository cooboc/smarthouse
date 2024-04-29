#include "button3_gear.h"
#include "data_def.h"
#include <vector>

namespace cooboc {

static constexpr char GEAR_NAME[]{"3-Button"};
Button3Gear::Button3Gear() {}

const char *Button3Gear::getName() const { return GEAR_NAME; }

Button3Gear::Button3GearInstance::Button3GearInstance(
    const std::uint8_t *gearConfig)
    : buttons_{Button{12U}, Button{14U}, Button{5U}},
      relays_{Relay{4U}, Relay{13U}, Relay{15U}}, leds_{Led{2U, true}},
      isReadyRestart_{false} {

  static_assert(sizeof(Button3GearConfig) <= sizeof(Persistent::gearConfig));
  memcpy(&config_, gearConfig, sizeof(Button3GearConfig));
  Serial.print("config = ");
  Serial.println(config_.buttonRelayconnectivity);
}

Button3Gear::Button3GearInstance *Button3Gear::Button3GearInstance::instance_ =
    nullptr;

IGearInstance *Button3Gear::getInstance(const std::uint8_t *gearConfig) const {
  return Button3GearInstance::getInstance(gearConfig);
}

Button3Gear::Button3GearInstance *
Button3Gear::Button3GearInstance::getInstance(const std::uint8_t *gearConfig) {
  if (instance_ == nullptr) {
    instance_ = new Button3Gear::Button3GearInstance(gearConfig);
  }
  return instance_;
}

void Button3Gear::Button3GearInstance::setup() {
  Serial.print("Button3Gear connectivity: ");
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

  buttons_[0].onPushUp([this]() {
    this->leds_[0].set(false);
    if (this->isReadyRestart_) {
      Serial.println("Bye.");
      ESP.restart();
    }
  });
}

void Button3Gear::Button3GearInstance::tick() {
  std::for_each(leds_.begin(), leds_.end(), [](Led &l) { l.tick(); });
  std::for_each(buttons_.begin(), buttons_.end(), [](Button &b) { b.tick(); });
}

void Button3Gear::Button3GearInstance::fillStatus(std::uint8_t *buffer) const {
  std::uint8_t statusByte{0U};
  for (std::size_t i{0U}; i < buttons_.size(); ++i) {
    statusByte <<= 1;
    if (buttons_[i].isPushed()) {
      statusByte |= 1U;
    }
  }
  buffer[0] = statusByte;
}

void Button3Gear::Button3GearInstance::onServerRequest(ServerRequest req) {
  Serial.println("handle server here");
  const std::uint8_t portId = req.portId;
  if (portId >= relays_.size()) {
    return;
  }
  const bool status = req.request == ServerRequest::RequestType::RELAY_ON;

  relays_[portId].set(status);
}

} // namespace cooboc
