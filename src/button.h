#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include <cstdio>

namespace cooboc {
class Button {
public:
  Button(std::uint8_t pin);
  Button(const Button &) = delete;
  Button(const Button &&b)
      : pin_{b.pin_}, initValue_{b.initValue_}, lastValue_{b.lastValue_},
        debouncingTime_{b.debouncingTime_},
        debouncingValue_{b.debouncingValue_} {}
  Button &operator=(const Button &) = delete;
  ~Button() {}

  void setup();
  void tick();

private:
  std::uint8_t pin_;
  const int initValue_;
  int lastValue_;
  int debouncingValue_;
  unsigned long debouncingTime_;
};
} // namespace cooboc

#endif
