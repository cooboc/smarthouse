#ifndef __ROMP_CLIENT_H__
#define __ROMP_CLIENT_H__

#include "configuration.h"
#include <ESPAsyncTCP.h>

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
  void end();

private:
  const Configuration &configuration_;
  AsyncClient *socketClient_ = nullptr;

  void onSocketConnected();
};
} // namespace cooboc

#endif
