#ifndef __KITCHEN_GEAR_H__
#define __KITCHEN_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include "led.h"
#include "relay.h"
#include <array>
#include <functional>

namespace cooboc {
class KitchenGear : public IGear {
public:
  KitchenGear();
  virtual ~KitchenGear() {}
  virtual const char *getName() const override;
  virtual IGearInstance *
  getInstance(const std::uint8_t *gearConfig) const override;

private:
  class KitchenGearInstance : public IGearInstance {
  public:
    static KitchenGearInstance *getInstance(const std::uint8_t *gearConfig);

    virtual void setup() override;
    virtual void tick() override;
    virtual void fillStatus(std::uint8_t *buffer,
                            std::size_t length) const override;
    virtual void onServerRequest(const ServerRequest &req) override;

  private:
    struct KitchenGearConfig {
      std::uint8_t buttonRelayconnectivity;
    };

    std::array<Button, 3U> buttons_;
    std::array<Relay, 3U> relays_;
    std::array<Led, 1U> leds_;
    std::uint8_t userActionPayload_[2];
    KitchenGearConfig config_;
    bool isReadyRestart_;
    bool isReadyReset_;

    static KitchenGearInstance *instance_;
    KitchenGearInstance(const std::uint8_t *gearConfig);
  };
};
} // namespace cooboc

#endif
