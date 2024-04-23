#include "relay.h"
#include <Arduino.h>
#include <cstdint>

namespace cooboc {

Relay::Relay(std::uint8_t pin) : pin_{pin}, status_{false} {}

void Relay::setup() {
  pinMode(pin_, OUTPUT);
  Serial.print("setup output pin#");
  Serial.println(pin_);
  digitalWrite(pin_, status_);
}

void Relay::set(bool status) {
  status_ = status;

  if (status_) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
  digitalWrite(pin_, status_);
}

void Relay::toggle() {
  status_ = !status_;
  digitalWrite(pin_, status_);

  if (status_) {
    Serial.println("ON");
  } else {
    Serial.println("OFF");
  }
}

} // namespace cooboc