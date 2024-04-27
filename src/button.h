#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <Arduino.h>
#include <cstdio>
#include <functional>

namespace cooboc {
class Button {
public:
  using EventCallback = std::function<void(void)>;

  Button(std::uint8_t pin);
  Button(const Button &&b);
  Button(const Button &) = delete;
  Button &operator=(const Button &) = delete;
  ~Button() {}

  void setup();
  void tick();
  bool isPushed() const;
  void onPushDown(EventCallback callback) { pushDownCallback_ = callback; }
  void onPushUp(EventCallback callback) { pushUpCallback_ = callback; }

private:
  std::uint8_t pin_;
  const int initValue_;
  int lastValue_;
  int debouncingValue_;
  unsigned long debouncingTime_;

  unsigned long clickStartTime_;

  EventCallback pushDownCallback_ = []() {};
  EventCallback pushUpCallback_ = []() {};

  void behaviourDetector(int currentValue);
};
} // namespace cooboc

#endif
