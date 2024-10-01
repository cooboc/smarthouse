#include "led.h"
#include <Arduino.h>
#include <cstdint>
namespace cooboc {

constexpr unsigned long FAST_BLINK_CYCLE{200U};

Led::Led(std::uint8_t pin, bool reversed)
    : pin_{pin}, status_{false}, reversed_{reversed}, animationStartTime_{0U},
      animationStatus_{AnimationStatus::SOLID} {}
Led::Led(const Led &&another)
    : pin_{another.pin_}, status_{another.status_},
      reversed_{another.reversed_},
      animationStartTime_{another.animationStartTime_},
      animationStatus_{another.animationStatus_} {}

void Led::setup() {
  pinMode(pin_, OUTPUT);
  Serial.print("setup LED pin#");
  Serial.println(pin_);
  digitalWrite(pin_, status_ ^ reversed_);
}

void Led::set(bool status) {
  status_ = status;
  digitalWrite(pin_, status_ ^ reversed_);
  animationStatus_ = AnimationStatus::SOLID;
}

void Led::startFastBlink() {
  animationStatus_ = AnimationStatus::FAST_BLINKING;
  animationStartTime_ = millis();
}

void Led::tick() {
  switch (animationStatus_) {
  case (AnimationStatus::SOLID): {
    break;
  }
  case (AnimationStatus::FAST_BLINKING): {
    const unsigned long diffTime = millis() - animationStartTime_;
    const unsigned long offsetTime = diffTime % FAST_BLINK_CYCLE;
    bool isOn = (offsetTime > (FAST_BLINK_CYCLE >> 1));
    digitalWrite(pin_, isOn ^ reversed_);
    break;
  }
  default: {
    break;
  }
  }
}

} // namespace cooboc