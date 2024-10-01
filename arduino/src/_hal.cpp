// #include "hal.h"
// #include "i_gear.h"
// #include <Arduino.h>

// namespace cooboc {

// Hal::Hal(const Configuration &configuration)
//     : configuration_(configuration), gear_(nullptr) {}

// void Hal::setup() {
//   gear_ = configuration_.getGear();
//   Serial.print("HAL ready to setup ");
//   Serial.println(gear_->getName());
//   Serial.print("size of pins: ");
//   Serial.println(gear_->getInputPins().size());
//   const PinList &pinList = gear_->getInputPins();
//   Serial.println("input pin: ");
//   for (auto pin : pinList) {
//     Button b(pin);
//     b.setup();
//     buttons_.emplace_back(std::move(b));
//     Serial.println(pin);
//   }
// }

// void Hal::tick() {
//   for (Button &b : buttons_) {
//     b.tick();
//   }
// }

// } // namespace cooboc
