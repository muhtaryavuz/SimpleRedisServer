#pragma once

#include <cstddef>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>

#include "NetworkController/Definitions.hpp"

namespace app::net {
class Socket {
public:
  explicit Socket(int);
  ~Socket();

  std::string Read();
  void Send(std::string);

private:
  static const std::size_t c_buffer_length = 1024;

private:
  char m_buffer[c_buffer_length] = {0};
  int m_id{InvalidSocketId};
};
} // namespace app::net
