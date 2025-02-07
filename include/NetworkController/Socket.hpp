#pragma once

#include <cstddef>
#include <netinet/in.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#include "NetworkController/Definitions.hpp"
#include "WorkerModule/WorkerController.hpp"

namespace app::net {
class Socket {
private:
  static const std::size_t kBufferLength = 1024;

public:
  explicit Socket(int,
                  const std::shared_ptr<worker_utility::WorkerController> &);
  ~Socket();

  bool Read();
  void Send(const std::string&) const;

private:
  const std::shared_ptr<worker_utility::WorkerController> workers_;
  char cache_buffer_[kBufferLength] = {0};
  int head_{0};
  int id_{kInvalidSocketId};
};
} // namespace app::net
