#ifndef __STATE_MACHINE__
#define __STATE_MACHINE__

#include "configuration.h"
#include "romp_client.h"
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

  StateMachine(const Configuration &configruation, WifiHandler &wifi,
               RompClient &rompClient);
  void begin();
  void tick();

private:
  State state_{State::CONNECTING};
  const Configuration &configuration_;
  WifiHandler &wifi_;
  RompClient &rompClient_;

  void transitOut();
  void transitTo(State newState);

  void handleConnectingTick(const WifiHandler::WifiStatus wifiStatus);
};

} // namespace cooboc

#endif
