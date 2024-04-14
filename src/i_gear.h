#ifndef __I_GEAR_H__
#define __I_GEAR_H__

#include <cstdint>
#include <vector>

namespace cooboc {

using PinList = std::vector<std::uint8_t>;

class IGear {
public:
  virtual ~IGear() {}
  virtual const char *getName() const = 0;
  virtual const PinList &getInputPins() const = 0;
};
} // namespace cooboc

#endif
