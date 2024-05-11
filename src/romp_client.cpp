#include "romp_client.h"
#include "utils.h"
#include <Arduino.h>
#include <cstdint>

namespace cooboc {

namespace {
constexpr std::size_t PACKET_OFFSET_SEQUENCE{8U};
constexpr std::size_t PACKET_OFFSET_TYPE{9U};
} // namespace

constexpr static std::uint32_t RECONNECT_SERVER_TIME{2000UL};

RompClient::RompClient(const Configuration &configuration)
    : configuration_{configuration}, socketClient_{nullptr},
      status_{Status::IDLE}, lastHeartbeatTime_{0UL},
      lastConnectFailedTime_{0UL}, packetSeq_{0U}, parser_{configuration} {}

void RompClient::init() {
  // inflate packet buffer
  packetBuffer_[0] = 'A';
  packetBuffer_[1] = '5';
  packetBuffer_[2] = 1; // current protocol version
  const uint32_t deviceId = configuration_.getDeviceId();
  utils::writeUint32(deviceId, packetBuffer_ + 3);
  const uint8_t typeId = configuration_.getGearTypeId();
  utils::writeUint8(typeId, packetBuffer_ + 7);
  Serial.print("chipId: ");
  Serial.println(deviceId);

  socketClient_ = new AsyncClient();
  socketClient_->setAckTimeout(400U);
  socketClient_->setRxTimeout(5U);
  socketClient_->onConnect(
      [this](void *arg, AsyncClient *client) -> void {
        this->onSocketConnected();
      },
      nullptr);
  socketClient_->onDisconnect(
      [this](void *arg, AsyncClient *ac) { this->onDisconnect(arg, ac); },
      this);
  //   socketClient_->onDisconnect(onDisconnectedCallback, this);
  //   socketClient_->onError(onErrorCallback, this);
  socketClient_->onData([this](void *, AsyncClient *, void *data,
                               size_t len) { this->onSocketData(data, len); },
                        nullptr);
  socketClient_->onError(
      [this](void *arg, AsyncClient *ac, err_t error) {
        this->onConnectFailed(arg, ac, error);
      },
      this);
  //   socketClient_->onTimeout(onTimeoutCallback, this);
  //   socketClient_->onPoll(onPollCallback, this);
}

void RompClient::tick() {
  switch (status_) {
  case (Status::IDLE): {
    // do nothing
    break;
  }
  case (Status::CONNECTING): {
    // do nothing, waiting for connecting
    break;
  }
  case (Status::CONNECTED): {
    if (millis() - lastHeartbeatTime_ > 2000UL) {
      lastHeartbeatTime_ = millis();
      sendHeartbeat();
    }
    break;
  }
  case (Status::DISCONNECTED): {
    // wait for cooling down and reconnect
    if ((millis() - lastConnectFailedTime_) > RECONNECT_SERVER_TIME) {
      Serial.println("reconnect server");
      status_ = Status::CONNECTING;
      socketClient_->connect(configuration_.getServerAddr(), 8113);
    }
  }
  default: {
    break;
  }
  }
}

void RompClient::start() {
  socketClient_->connect(configuration_.getServerAddr(), 8113);
  status_ = Status::CONNECTING;
  Serial.print("ready to connect server: ");
  Serial.print(configuration_.getServerAddr());
  Serial.println(":8113");

  auto gearPtr = configuration_.getGearInstance();
  if (gearPtr != nullptr) {
    gearPtr->onUserAction(
        [this](void *payload) { this->onUserAction(payload); });
  }
}
void RompClient::stop() {}

void RompClient::onSocketConnected(void) {
  status_ = Status::CONNECTED;
  Serial.println("connected to server, hahahahah");
}

void RompClient::onConnectFailed(void *arg, AsyncClient *ac, err_t error) {
  Serial.println("on connect error");
  status_ = Status::DISCONNECTED;
  lastConnectFailedTime_ = millis();
}

void RompClient::onDisconnect(void *arg, AsyncClient *ac) {
  Serial.println("disconnected to server");
  status_ = Status::DISCONNECTED;
  lastConnectFailedTime_ = millis();
}

void RompClient::onSocketData(void *data, size_t len) {
  Serial.print("got data length: ");
  Serial.println(len);
  ServerRequestOpt opt = parser_.prase(static_cast<std::uint8_t *>(data), len);
  if (opt.hasData) {
    auto gearPtr = configuration_.getGearInstance();
    if (gearPtr != nullptr) {
      gearPtr->onServerRequest(opt.data);
    }
  }
}

void RompClient::sendHeartbeat() {
  // Set the packet type
  packetBuffer_[PACKET_OFFSET_TYPE] = utils::valueOf(PacketType::HEARTBEAT);

  // get the payload of heartbeat
  auto gearPtr = configuration_.getGearInstance();
  if (gearPtr == nullptr) {
    // fill zero
    memset(packetBuffer_ + detail::PACKET_HEAD_LENGTH, 0U,
           detail::PACKET_PAYLOAD_LENGTH);
  } else {
    gearPtr->fillStatus(packetBuffer_ + detail::PACKET_HEAD_LENGTH,
                        detail::PACKET_PAYLOAD_LENGTH);
  }

  sendPacket();
}

void RompClient::onUserAction(void *payload) {
  // Set packet type
  packetBuffer_[PACKET_OFFSET_TYPE] = utils::valueOf(PacketType::USER_EVENT);

  // Payload
  std::memcpy(packetBuffer_ + detail::PACKET_HEAD_LENGTH, payload,
              detail::PACKET_PAYLOAD_LENGTH);
  sendPacket();
}

void RompClient::sendPacket() {
  packetSeq_++;
  packetBuffer_[PACKET_OFFSET_SEQUENCE] = packetSeq_;

  socketClient_->write((const char *)packetBuffer_,
                       detail::PACKET_HEAD_LENGTH +
                           detail::PACKET_PAYLOAD_LENGTH);
}

} // namespace cooboc