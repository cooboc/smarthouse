#ifndef __ROMP_CLIENT_H__
#define __ROMP_CLIENT_H__

#include "configuration.h"
#include "romp_parser.h"
#include <ESPAsyncTCP.h>
#include <cstdint>
#include <functional>

namespace cooboc {

namespace detail {
constexpr std::size_t PACKET_HEAD_LENGTH{8U};
constexpr std::size_t PACKET_BODY_LENGTH{3U};

} // namespace detail

/**
 * @brief ROMP is a protocol for communication between smart gear with the
 * server.
 *
 */
class RompClient {
public:
  using ServerRequestCallback = std::function<void(ServerRequest)>;

  RompClient(const Configuration &configuration);
  void begin();
  void tick();
  void end();

private:
  enum class Status : std::uint8_t {
    IDLE,
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
  };

  enum class PakcetType : std::uint8_t {
    HEARTBEAT = 1U,
  };

  const Configuration &configuration_;
  AsyncClient *socketClient_{nullptr};
  Status status_{Status::IDLE};
  std::uint32_t lastHeartbeatTime_{0UL};
  std::uint32_t lastConnectFailedTime_{0UL};

  RompParser parser_;

  // Packet related data
  // HEAD: 'A5', PROTOCOL_VERSION, ESP_ID,type
  // BODY: heartbeat,ext
  std::uint8_t packetBuffer_[detail::PACKET_HEAD_LENGTH +
                             detail::PACKET_BODY_LENGTH]{0U};
  std::uint8_t packetSeq_{0U};

  void onSocketConnected();
  void onSocketData(void *data, size_t len);
  void onConnectFailed(void *arg, AsyncClient *ac, err_t error);
  void onDisconnect(void *arg, AsyncClient *ac);
  void sendHeartbeat();
};

} // namespace cooboc

#endif
