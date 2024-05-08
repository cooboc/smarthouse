#ifndef __I_GEAR_H__
#define __I_GEAR_H__

#include "data_def.h"
#include "utils.h"
#include <cstdint>
#include <functional>
#include <vector>

namespace cooboc {

using UserActionCallback = std::function<void(void *)>;
using EventCallback = std::function<void(void)>;

class IGearInstance {
public:
  virtual void setup() = 0;
  virtual void tick() = 0;
  virtual void fillStatus(std::uint8_t *buffer, std::size_t length) const = 0;
  virtual void onUserAction(UserActionCallback callback) {
    userActionCallback_ = callback;
  }
  virtual void onResetPushed(EventCallback callback) {
    resetPushedCallback_ = callback;
  }
  virtual void onServerRequest(ServerRequest req) {}

protected:
  UserActionCallback userActionCallback_ = [](void *) {};
  EventCallback resetPushedCallback_ = utils::EMPTY_FUNCTION;
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
