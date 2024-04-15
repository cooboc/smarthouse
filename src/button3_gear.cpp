#include "button3_gear.h"

#include <vector>

namespace cooboc {

static constexpr char GEAR_NAME[]{"3-Button"};
Button3Gear::Button3Gear() {}

const char *Button3Gear::getName() const { return GEAR_NAME; }

Button3Gear::Button3GearInstance::Button3GearInstance()
    : buttons_{Button{12U}, Button{14U}, Button{5U}} {}

Button3Gear::Button3GearInstance *Button3Gear::Button3GearInstance::instance_ =
    nullptr;

IGearInstance *Button3Gear::getInstance() const {
  return Button3GearInstance::getInstance();
}

Button3Gear::Button3GearInstance *
Button3Gear::Button3GearInstance::getInstance(void) {
  if (instance_ == nullptr) {
    instance_ = new Button3Gear::Button3GearInstance();
  }
  return instance_;
}

void Button3Gear::Button3GearInstance::setup() {
  for (Button &b : buttons_) {
    b.setup();
  }
}

void Button3Gear::Button3GearInstance::tick() {
  for (Button &b : buttons_) {
    b.tick();
  }
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

} // namespace cooboc
