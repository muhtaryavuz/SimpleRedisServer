#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "NetworkController/Definitions.hpp"

namespace app::net {
class ServerSocket {
public:
  ServerSocket() = default;
  ~ServerSocket();

  [[nodiscard]] int CreateSocket(std::string, port_t);
  [[nodiscard]] int Accept() const;
  [[nodiscard]] int GetSocketId() const {return m_id;}
 
private:
  struct sockaddr_in m_address;
  int m_id{InvalidSocketId};
  int m_options{1};
};
} // namespace app::net