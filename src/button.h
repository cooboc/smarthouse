#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include <cstdio>

namespace cooboc {
class Button {
public:
  Button(std::uint8_t pin);
  Button(const Button &&b);
  Button(const Button &) = delete;
  Button &operator=(const Button &) = delete;
  ~Button() {}

  void setup();
  void tick();
  bool isPushed() const;

private:
  std::uint8_t pin_;
  const int initValue_;
  int lastValue_;
  int debouncingValue_;
  unsigned long debouncingTime_;

  unsigned long clickStartTime_;

  void behaviourDetector(int currentValue);
};
} // namespace cooboc

#endif
