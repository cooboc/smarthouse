#include "button.h"
#include "utils.h"
#include <Arduino.h>

namespace cooboc {

constexpr unsigned long CLICK_TIME_THRESHOLD{400U};
constexpr unsigned long RESTART_TIME_THRESHOLD{15000u};

Button::Button(std::uint8_t pin)
    : pin_{pin}, initValue_{1}, lastValue_{1}, debouncingValue_{1},
      debouncingTime_{0U}, clickStartTime_{0U}, isRestartTriggered_{false},
      pushDownCallback_{utils::EMPTY_FUNCTION},
      pushUpCallback_{utils::EMPTY_FUNCTION},
      restartCallback_{utils::EMPTY_FUNCTION} {}

Button::Button(const Button &&b)
    : pin_{b.pin_}, initValue_{b.initValue_}, lastValue_{b.lastValue_},
      debouncingTime_{b.debouncingTime_}, debouncingValue_{b.debouncingValue_},
      clickStartTime_{0U}, isRestartTriggered_{b.isRestartTriggered_},
      pushDownCallback_{b.pushDownCallback_},
      pushUpCallback_{b.pushUpCallback_}, restartCallback_{b.restartCallback_} {
}

void Button::setup() {
  pinMode(pin_, INPUT_PULLUP);
  Serial.print("setup pin #");
  Serial.println(pin_);
}
void Button::tick() {
  const int currentValue = digitalRead(pin_);
  if (lastValue_ == debouncingValue_) {
    if (debouncingValue_ == currentValue) {
      // keep last status
      holdingBehaviourDetector(currentValue);
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
        flipFlopBehaviourDetector(currentValue);
      }
    } else {
      // User relase the button, reset the value
      debouncingValue_ = lastValue_;
      debouncingTime_ = 0;
    }
  }
}

bool Button::isPushed() const { return lastValue_ != initValue_; }

void Button::flipFlopBehaviourDetector(int currentValue) {
  if (currentValue == initValue_) {
    // relased
    Serial.println("realsed");
    isRestartTriggered_ = false;
    pushUpCallback_();
    if ((millis() - clickStartTime_) < CLICK_TIME_THRESHOLD) {
      Serial.println("clicked");
    } else {
      Serial.println("long push");
    }
  } else {
    // pushed
    clickStartTime_ = millis();
    Serial.println("push down");
    pushDownCallback_();
  }
}

void Button::holdingBehaviourDetector(int currentValue) {
  if (currentValue != initValue_) {
    const unsigned long holdingTime{millis() - clickStartTime_};
    const bool isTimeSatisfied{holdingTime > RESTART_TIME_THRESHOLD};
    if (isTimeSatisfied && !isRestartTriggered_) {
      isRestartTriggered_ = true;
      restartCallback_();
    }
  }
}

} // namespace cooboc
