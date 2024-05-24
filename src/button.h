#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "utils.h"
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
  void onRestartSatisfied(EventCallback callback) {
    restartCallback_ = callback;
  }
  void onResetSatified(EventCallback callback) { resetCallback_ = callback; }
  std::uint8_t getPin() const { return pin_; }

private:
  std::uint8_t pin_;
  const int initValue_;
  int lastValue_;
  int debouncingValue_;
  unsigned long debouncingTime_;
  unsigned long clickStartTime_;
  bool isRestartTriggered_;
  bool isResetTriggered_;

  EventCallback pushDownCallback_ = utils::EMPTY_FUNCTION;
  EventCallback pushUpCallback_ = utils::EMPTY_FUNCTION;
  EventCallback restartCallback_ = utils::EMPTY_FUNCTION;
  EventCallback resetCallback_ = utils::EMPTY_FUNCTION;

  void flipFlopBehaviourDetector(int currentValue);
  void holdingBehaviourDetector(int currentValue);
};
} // namespace cooboc

#endif
