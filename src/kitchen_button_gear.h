#ifndef __KITCHEN_BUTTON_GEAR_H__
#define __KITCHEN_BUTTON_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include "led.h"
#include "n_button_relay_gear.h"
#include "relay.h"
#include <array>
#include <functional>

namespace cooboc {
class KitchenButtonGear : public NButtonRelayGear {
public:
  KitchenButtonGear() : NButtonRelayGear() {}
  virtual ~KitchenButtonGear() {}
  virtual const char *getName() const override { return "Kitchen"; }
  virtual IGearInstance *
  setupInstance(const std::uint8_t *gearConfig) const override;

private:
  class KitchenButtonGearInstance : public NButtonRelayGearInstance {
  public:
    KitchenButtonGearInstance(const std::uint8_t *gearConfig);
    virtual ~KitchenButtonGearInstance() {}
    virtual void setup() override;
  };
};
} // namespace cooboc

#endif
