#include "romp_parser.h"
#include "data_def.h"
#include <cstdint>

namespace cooboc {
ServerRequestOpt RompParser::prase(std::uint8_t *data, std::size_t len) {
  ServerRequestOpt result;
  result.data.portId = data[0];
  result.data.request = (data[1]) ? ServerRequest::RequestType::RELAY_ON
                                  : ServerRequest::RequestType::RELAY_OFF;
  result.hasData = true;
  return result;
}

} // namespace cooboc