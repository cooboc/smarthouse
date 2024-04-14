#ifndef __HAL_H__
#define __HAL_H__

#include "configuration.h"
#include "i_gear.h"

namespace cooboc {
class Hal {
public:
  Hal(const Configuration &configuration);
  ~Hal() {}
  void setup();
  void tick();

private:
  const Configuration &configuration_;
  IGear *gear_;
};
} // namespace cooboc

#endif
