#ifndef __N_BUTTON_RELAY_GEAR_H__
#define __N_BUTTON_RELAY_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include "led.h"
#include "relay.h"
#include <array>
#include <functional>

namespace cooboc {

class NButtonRelayGear : public IGear {
public:
  NButtonRelayGear() : IGear() {}
  virtual ~NButtonRelayGear() {}
  virtual const char *getName() const override { return "NB"; }
  virtual IGearInstance *
  setupInstance(const std::uint8_t *gearConfig) const override;
};

class NButtonRelayGearInstance : public IGearInstance {
public:
  NButtonRelayGearInstance(const std::uint8_t *gearConfig);
  virtual void setup() override;
  virtual void tick() override;
  virtual void fillStatus(std::uint8_t *buffer,
                          std::size_t length) const override;
  virtual void onServerRequest(const ServerRequest &req) override;

private:
  struct NButtonRelayGearConfig {
    std::uint8_t buttonRelayconnectivity;
  };

  std::array<Button, 3U> buttons_;
  std::array<Relay, 3U> relays_;
  std::array<Led, 1U> leds_;
  std::uint8_t userActionPayload_[2];
  NButtonRelayGearConfig config_;
  bool isReadyRestart_;
  bool isReadyReset_;
};
} // namespace cooboc

#endif
