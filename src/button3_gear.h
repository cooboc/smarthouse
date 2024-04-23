#ifndef __BUTTON_3_GEAR_H__
#define __BUTTON_3_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include "relay.h"
#include <array>
#include <functional>

namespace cooboc {
class Button3Gear : public IGear {
public:
  Button3Gear();
  virtual ~Button3Gear() {}
  virtual const char *getName() const override;
  virtual IGearInstance *
  getInstance(const std::uint8_t *gearConfig) const override;

private:
  class Button3GearInstance : public IGearInstance {
  public:
    static Button3GearInstance *getInstance(const std::uint8_t *gearConfig);

    virtual void setup() override;
    virtual void tick() override;
    virtual void fillStatus(std::uint8_t *buffer) const override;
    virtual void onServerRequest(ServerRequest req) override;

  private:
    struct Button3GearConfig {
      std::uint8_t buttonRelayconnectivity;
    };

    std::array<Button, 3U> buttons_;
    std::array<Relay, 3U> relays_;
    std::uint8_t userActionPayload_[2];
    Button3GearConfig config_;

    static Button3GearInstance *instance_;
    Button3GearInstance(const std::uint8_t *gearConfig);
  };
};
} // namespace cooboc

#endif
