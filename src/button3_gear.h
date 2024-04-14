#ifndef __BUTTON_3_GEAR_H__
#define __BUTTON_3_GEAR_H__

#include "i_gear.h"

namespace cooboc {
class Button3Gear : public IGear {
public:
  Button3Gear(){};
  virtual ~Button3Gear() {}
  virtual const char *getName() const;
  virtual const PinList &getInputPins() const;

private:
};
} // namespace cooboc

#endif
