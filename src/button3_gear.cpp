#include "button3_gear.h"

#include <vector>

namespace cooboc {

static constexpr char GEAR_NAME[]{"3-Button"};
// static const PinList PIN_LIST{12U, 14U, 5U};

Button3Gear::Button3Gear() {}

const char *Button3Gear::getName() const { return GEAR_NAME; }

Button3Gear::Button3GearInstance::Button3GearInstance()
    : buttons_{Button{12U}, Button{14U}, Button{5U}} {}

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

IGearInstance *Button3Gear::buildInstance() const {
  return new Button3GearInstance();
}

} // namespace cooboc
