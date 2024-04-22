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
  virtual const char *getName() const;
  virtual IGearInstance *getInstance() const;

private:
  class Button3GearInstance : public IGearInstance {
  public:
    static Button3GearInstance *getInstance(void);

    virtual void setup() override;
    virtual void tick() override;
    virtual void fillStatus(std::uint8_t *buffer) const override;
    virtual void onServerRequest(ServerRequest req) override;

  private:
    std::array<Button, 3U> buttons_;
    std::array<Relay, 3U> relays_;
    std::uint8_t userActionPayload_[2];

    static Button3GearInstance *instance_;
    Button3GearInstance();
  };
};
} // namespace cooboc

#endif
