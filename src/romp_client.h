#ifndef __ROMP_CLIENT_H__
#define __ROMP_CLIENT_H__

#include "configuration.h"
#include <ESPAsyncTCP.h>
#include <cstdint>

namespace cooboc {
/**
 * @brief ROMP is a protocol for communication between smart gear with the
 * server.
 *
 */
class RompClient {
public:
  RompClient(const Configuration &configuration);
  void begin();
  void tick();
  void end();

private:
  enum class Status : std::uint8_t {
    IDLE,
    CONNECTING,
    CONNECTED,
    BAD,
  };

  const Configuration &configuration_;
  AsyncClient *socketClient_{nullptr};
  Status status_{Status::IDLE};
  std::uint32_t lastHeartbeatTime_{0UL};

  void onSocketConnected();
};
} // namespace cooboc

#endif
