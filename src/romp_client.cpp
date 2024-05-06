#include "romp_client.h"
#include "utils.h"
#include <Arduino.h>
#include <cstdint>

namespace cooboc {

constexpr static std::uint32_t RECONNECT_SERVER_TIME{2000UL};

RompClient::RompClient(const Configuration &configuration)
    : configuration_{configuration}, socketClient_{nullptr},
      status_{Status::IDLE}, lastHeartbeatTime_{0UL},
      lastConnectFailedTime_{0UL}, packetSeq_{0U}, parser_{} {
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
  socketClient_->connect(configuration_.getServerAddr(), 8114);
  status_ = Status::CONNECTING;

  Serial.print("ready to connect server: ");
  Serial.print(configuration_.getServerAddr());
  Serial.println(":8114");

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
      socketClient_->connect(configuration_.getServerAddr(), 8114);
    }
  }
  default: {
    break;
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

} // namespace cooboc