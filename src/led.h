#ifndef __LED_H__
#define __LED_H__

#include <cstdint>

namespace cooboc {
class Led {
public:
  Led(std::uint8_t pin, bool reversed_ = false);
  Led(const Led &&another);
  Led(const Led &) = delete;
  Led &operator=(const Led &) = delete;
  ~Led() {}

  void setup();
  void tick();

  void set(bool status);
  void startFastBlink();

private:
  std::uint8_t pin_;
  bool status_;
  bool reversed_;
  unsigned long animationStartTime_;

  enum class AnimationStatus : std::uint8_t { SOLID, FAST_BLINKING };
  AnimationStatus animationStatus_;
};
} // namespace cooboc

#endif
