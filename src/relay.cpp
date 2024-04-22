#include "relay.h"
#include <Arduino.h>
#include <cstdint>

namespace cooboc {

Relay::Relay(std::uint8_t pin) : pin_{pin} {}

void Relay::setup() {
  pinMode(pin_, OUTPUT);
  Serial.print("setup output pin#");
  Serial.println(pin_);
  digitalWrite(pin_, 0);
}

void Relay::set(bool status) {
  if (status) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  digitalWrite(pin_, status ? 1 : 0);
}

} // namespace cooboc