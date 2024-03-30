#include "romp_client.h"
#include <Arduino.h>

namespace cooboc {

RompClient::RompClient(const Configuration &configuration)
    : configuration_{configuration}, socketClient_{nullptr},
      status_{Status::IDLE}, lastHeartbeatTime_{0UL} {}

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
  //   socketClient_->onData(onDataCallback, this);
  //   socketClient_->onTimeout(onTimeoutCallback, this);
  //   socketClient_->onPoll(onPollCallback, this);
  socketClient_->connect(configuration_.getServerAddr(), 8113);
  status_ = Status::CONNECTING;

  Serial.print("ready to connect server: ");
  Serial.print(configuration_.getServerAddr());
  Serial.println(":8113");
}

void RompClient::tick() {
  if (status_ == Status::CONNECTED) {
    if (millis() - lastHeartbeatTime_ > 1000UL) {
      lastHeartbeatTime_ = millis();
      socketClient_->write("Hello world!", std::strlen("Hello world!"));
    }
  }
}

void RompClient::end() {}

void RompClient::onSocketConnected(void) {
  status_ = Status::CONNECTED;
  Serial.println("connected to server");
}

} // namespace cooboc