#include "romp_parser.h"
#include "data_def.h"
#include <Arduino.h>
#include <cstdint>

namespace cooboc {

constexpr std::uint8_t PACKET_TYPE_REQUEST_GEAR{0x03};

RompParser::RompParser(const Configuration &configuration)
    : configuration_{configuration} {}

ServerRequestOpt RompParser::prase(std::uint8_t *data, std::size_t len) {
  ServerRequestOpt result;
  // check head
  bool isHeadCorrect = (data[0] == 'A') && (data[1] == '5');
  //   Serial.print("head: ");
  //   Serial.println(isHeadCorrect ? "OK" : "Failed");
  bool isVersionSupported = data[2] == 0x01;
  //   Serial.print("Version: ");
  //   Serial.println((int)data[2]);
  if (isHeadCorrect && isVersionSupported) {
    const std::uint8_t &packetType{data[9]};
    switch (packetType) {
    case (PACKET_TYPE_REQUEST_GEAR): {
      for (std::size_t i{0U}; i < 10U; ++i) {
        result.data[i] = data[10 + i];
      }
      result.hasData = true;
      break;
    }
    default: {
      Serial.println("unknown packet type");
      break;
    }
    }
  } else {
    Serial.println("packet head or procotol invalid.");
  }

  return result;
}

} // namespace cooboc