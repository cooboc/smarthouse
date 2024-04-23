#ifndef __RELAY_H__
#define __RELAY_H__

#include <cstdint>

namespace cooboc {
class Relay {
public:
  Relay(std::uint8_t pin);
  void setup();

  void set(bool status);
  void toggle();

private:
  std::uint8_t pin_;
  bool status_;
};
} // namespace cooboc

#endif
