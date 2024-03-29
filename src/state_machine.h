#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include "configuration.h"
#include "wifi_handler.h"
#include <cstdint>

namespace cooboc {
class StateMachine {
public:
  enum class State : std::uint8_t {
    FAILSAFE,
    CONFIG_PORTAL,
    CONNECTING,
    WORKING_WITH_WIFI,

  };

  StateMachine(const Configuration &configruation, WifiHandler &wifi);
  void begin();
  void tick();

private:
  State state_{State::CONNECTING};
  const Configuration &configuration_;
  WifiHandler &wifi_;

  void transitOut();
  void transitTo(State newState);
};

} // namespace cooboc

#endif
