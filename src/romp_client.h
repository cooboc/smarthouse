#ifndef __ROMP_CLIENT_H__
#define __ROMP_CLIENT_H__

#include "configuration.h"
#include "romp_parser.h"
#include <ESPAsyncTCP.h>
#include <cstdint>
#include <functional>

namespace cooboc {

namespace detail {
constexpr std::size_t PACKET_HEAD_LENGTH{10U};
constexpr std::size_t PACKET_PAYLOAD_LENGTH{11U};

/**********************************************
0       'A'
1       '5'
2       Protocol_Version
3456    CHIP_ID
7       GEAR_TYPE
8       Packet_Seq
9       Packet_Type e.g. heartbeat, useraction...
10-19   Payload


**********************************************/

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
  void init();
  void tick();
  void start();
  void stop();

private:
  enum class Status : std::uint8_t {
    IDLE,
    CONNECTING,
    CONNECTED,
    DISCONNECTED,
  };

  enum class PacketType : std::uint8_t {
    HEARTBEAT = 1U,
    USER_EVENT = 2U,
  };

  const Configuration &configuration_;
  AsyncClient *socketClient_{nullptr};
  Status status_{Status::IDLE};
  std::uint32_t lastHeartbeatTime_{0UL};
  std::uint32_t lastConnectFailedTime_{0UL};
  std::uint8_t packetSeq_{0U};
  RompParser parser_;

  // Packet related data
  // HEAD: 'A5', PROTOCOL_VERSION, ESP_ID,type
  // BODY: heartbeat,ext
  std::uint8_t packetBuffer_[detail::PACKET_HEAD_LENGTH +
                             detail::PACKET_PAYLOAD_LENGTH]{0U};

  void onSocketConnected();
  void onSocketData(void *data, size_t len);
  void onConnectFailed(void *arg, AsyncClient *ac, err_t error);
  void onDisconnect(void *arg, AsyncClient *ac);
  void sendHeartbeat();
  void onUserAction(void *payload);
  // This is a underlying function that readlly send the packet out
  void sendPacket();
};

} // namespace cooboc

#endif
