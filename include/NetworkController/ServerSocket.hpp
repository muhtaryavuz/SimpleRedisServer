#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "NetworkController/Definitions.hpp"

namespace app::net {
class ServerSocket {
public:
  ServerSocket() = default;
  ~ServerSocket();

  [[nodiscard]] int CreateSocket(const std::string&, port_t);
  [[nodiscard]] int Accept() const;
  [[nodiscard]] int GetSocketId() const { return id_; }

private:
  struct sockaddr_in address_;
  int id_{kInvalidSocketId};
  int options_{1};
};
} // namespace app::net