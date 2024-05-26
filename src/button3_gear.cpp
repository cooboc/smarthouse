#include "button3_gear.h"
#include "data_def.h"
#include <vector>

namespace cooboc {
IGearInstance *
Button3Gear::setupInstance(const std::uint8_t *gearConfig) const {
  return new NButtonRelayGearInstance(
      gearConfig, {ButtonConfig{12U}, ButtonConfig{14U}, ButtonConfig{5U}},
      {RelayConfig{4U}, RelayConfig{13U}, RelayConfig{15U}}, Led{2U, true});
}

} // namespace cooboc
