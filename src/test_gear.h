#ifndef __TEST_GEAR_H__
#define __TEST_GEAR_H__

#include "i_gear.h"

namespace cooboc {

class TestGear : public IGear {
public:
  TestGear();
  virtual ~TestGear() {}
  virtual const char *getName() const;

private:
};
} // namespace cooboc
#endif
