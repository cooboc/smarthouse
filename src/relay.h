#ifndef __RELAY_H__
#define __RELAY_H__

#include <cstdint>

namespace cooboc {
struct RelayConfig {
  std::uint8_t pin;
};
class Relay {
public:
  Relay(const RelayConfig &cfg);
  Relay(std::uint8_t pin);
  Relay(const Relay &&r);

  void setup();

  void set(bool status);
  void toggle();
  bool isClosed() const { return status_; }

private:
  std::uint8_t pin_;
  bool status_;
};
} // namespace cooboc

#endif
