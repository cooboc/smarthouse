#ifndef __I_GEAR_H__
#define __I_GEAR_H__

#include "data_def.h"
#include <cstdint>
#include <functional>
#include <vector>

namespace cooboc {

using UserActionCallback = std::function<void(void *)>;

class IGearInstance {
public:
  virtual void setup() = 0;
  virtual void tick() = 0;
  virtual void fillStatus(std::uint8_t *buffer) const = 0;
  virtual void onUserAction(UserActionCallback callback) {
    userActionCallback_ = callback;
  }
  virtual void onServerRequest(ServerRequest req) {}

protected:
  UserActionCallback userActionCallback_ = [](void *) {};
};

class IGear {
public:
  virtual ~IGear() {}
  virtual const char *getName() const = 0;

  virtual IGearInstance *getInstance(const std::uint8_t *gearConfig) const {
    return nullptr;
  };
};
} // namespace cooboc

#endif
