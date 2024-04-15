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
  virtual IGearInstance *buildInstance() const;

private:
  class Button3GearInstance : public IGearInstance {
  public:
    Button3GearInstance();
    virtual void setup();
    virtual void tick();

  private:
    std::array<Button, 3U> buttons_;
  };
};
} // namespace cooboc

#endif
