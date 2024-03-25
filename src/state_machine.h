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
    WORKING_WITH_WIFI,
    CONNECTING,
  };

  StateMachine(const Configuration &configruation, const WifiHandler &wifi);
  void begin();
  void tick();

private:
  State state_{State::CONNECTING};
  const Configuration &configuration_;
  const WifiHandler &wifi_;

  void transitOut();
  void transitTo(State newState);
};

} // namespace cooboc

#endif
