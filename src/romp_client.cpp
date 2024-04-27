#include "romp_client.h"
#include "utils.h"
#include <Arduino.h>

namespace cooboc {

RompClient::RompClient(const Configuration &configuration)
    : configuration_{configuration}, socketClient_{nullptr},
      status_{Status::IDLE}, lastHeartbeatTime_{0UL}, packetSeq_{0U},
      parser_{} {
  packetBuffer_[0] = 'A';
  packetBuffer_[1] = '5';
  packetBuffer_[2] = 1; // current protocol version
  const uint32_t deviceId = configuration.getDeviceId();
  utils::writeUint32(deviceId, packetBuffer_ + 3);
  const uint8_t typeId = configuration.getGearTypeId();
  utils::writeUint8(typeId, packetBuffer_ + 7);
}

void RompClient::begin() {
  socketClient_ = new AsyncClient();
  socketClient_->setAckTimeout(400U);
  socketClient_->setRxTimeout(5U);
  socketClient_->onConnect(
      [this](void *arg, AsyncClient *client) -> void {
        this->onSocketConnected();
      },
      nullptr);
  //   socketClient_->onDisconnect(onDisconnectedCallback, this);
  //   socketClient_->onError(onErrorCallback, this);
  socketClient_->onData([this](void *, AsyncClient *, void *data,
                               size_t len) { this->onSocketData(data, len); },
                        nullptr);
  //   socketClient_->onTimeout(onTimeoutCallback, this);
  //   socketClient_->onPoll(onPollCallback, this);
  socketClient_->connect(configuration_.getServerAddr(), 8113);
  status_ = Status::CONNECTING;

  Serial.print("ready to connect server: ");
  Serial.print(configuration_.getServerAddr());
  Serial.println(":8113");

  auto gearPtr = configuration_.getGearInstance();
  if (gearPtr != nullptr) {
    gearPtr->onUserAction([this](void *payload) {
      std::memcpy(packetBuffer_ + detail::PACKET_HEAD_LENGTH, payload, 2U);

      socketClient_->write((const char *)packetBuffer_,
                           detail::PACKET_HEAD_LENGTH +
                               detail::PACKET_BODY_LENGTH);
    });
  }
}

void RompClient::tick() {
  if (status_ == Status::CONNECTED) {
    if (millis() - lastHeartbeatTime_ > 2000UL) {
      lastHeartbeatTime_ = millis();
      sendHeartbeat();
    }
  }
}

void RompClient::sendHeartbeat() {
  packetBuffer_[detail::PACKET_HEAD_LENGTH] =
      static_cast<std::underlying_type_t<PakcetType>>(PakcetType::HEARTBEAT);
  auto gearPtr = configuration_.getGearInstance();
  if (gearPtr == nullptr) {
    packetBuffer_[detail::PACKET_HEAD_LENGTH + 1U] = 0U;
  } else {
    gearPtr->fillStatus(packetBuffer_ + detail::PACKET_HEAD_LENGTH + 1U);
  }

  socketClient_->write((const char *)packetBuffer_,
                       detail::PACKET_HEAD_LENGTH + detail::PACKET_BODY_LENGTH);

  // socketClient_->write("Hello world!", std::strlen("Hello world!"));
}

void RompClient::end() {}

void RompClient::onSocketConnected(void) {
  status_ = Status::CONNECTED;
  Serial.println("connected to server, hahahahah");
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

} // namespace cooboc