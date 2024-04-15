#include "button.h"
#include <Arduino.h>

namespace cooboc {

constexpr unsigned long CLICK_TIME_THRESHOLD{400U};

Button::Button(std::uint8_t pin)
    : pin_{pin}, initValue_{1}, lastValue_{1}, debouncingValue_{1},
      debouncingTime_{0U}, clickStartTime_{0U} {}

Button::Button(const Button &&b)
    : pin_{b.pin_}, initValue_{b.initValue_}, lastValue_{b.lastValue_},
      debouncingTime_{b.debouncingTime_}, debouncingValue_{b.debouncingValue_},
      clickStartTime_{0U} {}

void Button::setup() {
  pinMode(pin_, INPUT_PULLUP);
  Serial.print("setup pin #");
  Serial.println(pin_);
}
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
        lastValue_ = debouncingValue_;
        debouncingTime_ = 0U;
        behaviourDetector(currentValue);
      }
    } else {
      // User relase the button, reset the value
      debouncingValue_ = lastValue_;
      debouncingTime_ = 0;
    }
  }
}

bool Button::isPushed() const { return lastValue_ != initValue_; }

void Button::behaviourDetector(int currentValue) {
  if (currentValue == initValue_) {
    Serial.println("realsed");
    // relased
    if ((millis() - clickStartTime_) < CLICK_TIME_THRESHOLD) {
      Serial.println("clicked");
    } else {
      Serial.println("long push");
    }
  } else {
    // pushed
    Serial.println("push down");
    clickStartTime_ = millis();
  }
}

} // namespace cooboc
