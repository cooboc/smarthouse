#include "button.h"
#include <Arduino.h>

namespace cooboc {
Button::Button(std::uint8_t pin)
    : pin_{pin}, initValue_{1}, lastValue_{1}, debouncingValue_{1},
      debouncingTime_{0U} {}
void Button::setup() { pinMode(pin_, INPUT_PULLUP); }
void Button::tick() {
  const int currentValue = digitalRead(pin_);
  if (lastValue_ == debouncingValue_) {
    if (debouncingValue_ == currentValue) {
      // Nothing to do, just keep the current status
    } else {
      // User start to pushed button
      debouncingTime_ = millis();
      debouncingValue_ = currentValue;
    }
  } else {
    // User already pushed button
    if (currentValue == debouncingValue_) {
      // keep push, check the time
      const unsigned long currentTime = millis();
      if ((currentTime - debouncingTime_) > 20U) {
        Serial.println("button fliped");
        lastValue_ = debouncingValue_;
        debouncingTime_ = 0U;
      }
    } else {
      // User relase the button, reset the value
      debouncingValue_ = lastValue_;
      debouncingTime_ = 0;
    }
  }
}

} // namespace cooboc
