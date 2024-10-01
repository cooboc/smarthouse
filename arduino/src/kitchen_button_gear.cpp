#include "kitchen_button_gear.h"
#include "data_def.h"
#include <Arduino.h>
#include <vector>

namespace cooboc {

KitchenButtonGear::KitchenButtonGearInstance::KitchenButtonGearInstance(
    const std::uint8_t *gearConfig)
    : NButtonRelayGearInstance(
          gearConfig, {ButtonConfig{12U}, ButtonConfig{14U}, ButtonConfig{5U}},
          {RelayConfig{4U}, RelayConfig{13U}}, Led{2U, true}) {}

void KitchenButtonGear::KitchenButtonGearInstance::setup() {
  NButtonRelayGearInstance::setup();

  buttons_[2].onPushDown([this]() {
    Serial.println("kitchen remote down");
    led_.set(true);
    const bool isLightOn = relays_[0].isClosed() || relays_[1].isClosed();
    relays_[0].set(!isLightOn);
    relays_[1].set(!isLightOn);
    sendUserAction(UserActionType::BUTTON_PUSHED_DOWN, 2);
  });
}

IGearInstance *
KitchenButtonGear::setupInstance(const std::uint8_t *gearConfig) const {
  return new KitchenButtonGearInstance(gearConfig);
}

} // namespace cooboc
