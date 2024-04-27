#include "led.h"
#include <Arduino.h>
#include <cstdint>
namespace cooboc {

Led::Led(std::uint8_t pin, bool reversed)
    : pin_{pin}, status_{false}, reversed_{reversed} {}
Led::Led(const Led &&another)
    : pin_{another.pin_}, status_{another.status_},
      reversed_{another.reversed_} {}

void Led::setup() {
  pinMode(pin_, OUTPUT);
  Serial.print("setup LED pin#");
  Serial.println(pin_);
  digitalWrite(pin_, status_ ^ reversed_);
}

void Led::set(bool status) {
  status_ = status;
  digitalWrite(pin_, status_ ^ reversed_);
}

void Led::tick() {}

} // namespace cooboc