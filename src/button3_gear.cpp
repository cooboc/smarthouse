#include "button3_gear.h"
#include "data_def.h"
#include <vector>

namespace cooboc {
IGearInstance *
Button3Gear::setupInstance(const std::uint8_t *gearConfig) const {
  return new Button3GearInstance(gearConfig);
}

} // namespace cooboc
