#ifndef __I_GEAR_H__
#define __I_GEAR_H__

#include <cstdint>
#include <vector>

namespace cooboc {

class IGearInstance {
public:
  virtual void setup() = 0;
  virtual void tick() = 0;
};

class IGear {
public:
  virtual ~IGear() {}
  virtual const char *getName() const = 0;

  virtual IGearInstance *buildInstance() const = 0;
};
} // namespace cooboc

#endif
