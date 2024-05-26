#ifndef __BUTTON_3_GEAR_H__
#define __BUTTON_3_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include "led.h"
#include "n_button_relay_gear.h"
#include "relay.h"
#include <array>
#include <functional>

namespace cooboc {
class Button3Gear : public NButtonRelayGear {
public:
  Button3Gear() : NButtonRelayGear() {}
  virtual ~Button3Gear() {}
  virtual const char *getName() const override { return "3-Button"; }
  virtual IGearInstance *
  setupInstance(const std::uint8_t *gearConfig) const override;

private:
  class Button3GearInstance : public NButtonRelayGearInstance {
  public:
    Button3GearInstance(const std::uint8_t *gearConfig)
        : NButtonRelayGearInstance(gearConfig) {}
    virtual ~Button3GearInstance() {}
  };
};
} // namespace cooboc

#endif
