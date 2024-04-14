#include "hal.h"
#include "i_gear.h"
#include <Arduino.h>

namespace cooboc {

Hal::Hal(const Configuration &configuration)
    : configuration_(configuration), gear_(nullptr) {}

void Hal::setup() {
  const IGear *gear{configuration_.getGear()};
  Serial.print("HAL ready to setup ");
  Serial.println(gear->getName());
  Serial.print("size of pins: ");
  Serial.println(gear->getInputPins().size());
  const PinList &pinList = gear->getInputPins();
  Serial.println("input pin: ");
  for (auto pin : pinList) {
    pinMode(pin, INPUT);
    Serial.println(pin);
  }
}

void Hal::tick() {}

} // namespace cooboc
