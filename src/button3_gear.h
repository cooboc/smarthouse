#ifndef __BUTTON_3_GEAR_H__
#define __BUTTON_3_GEAR_H__

#include "button.h"
#include "i_gear.h"
#include <array>

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

    virtual void setup();
    virtual void tick();
    virtual void fillStatus(std::uint8_t *buffer) const;

  private:
    std::array<Button, 3U> buttons_;

    static Button3GearInstance *instance_;
    Button3GearInstance();
  };
};
} // namespace cooboc

#endif
