#include <cstring>
#include <iostream>

#include "NetworkController/Socket.hpp"

app::net::Socket::Socket(
    int socket_id,
    const std::shared_ptr<worker_utility::WorkerController> &workers)
    : id_(socket_id), workers_(workers) {};

app::net::Socket::~Socket() { close(id_); }

bool app::net::Socket::Read() {

  ssize_t bytes_read{0};
  char buffer[kBufferLength] = {0};

  while (true) {
    bytes_read = read(id_, buffer, kBufferLength);
    if (bytes_read == 0)
      return false;

    if (bytes_read > 0) {
      std::cout << "FROM fd: " << id_ << " -> " << buffer << "\n";

      for (int i = 0; i < bytes_read - 1; i++) {
        if (i > 0 and buffer[i] == app::net::kLF and
            buffer[i - 1] == app::net::kCR) {
          cache_buffer_[head_ - 1] = 0;
          head_ = 0;

          workers_->sendToWorkers(worker_utility::worker_variant_t{
              std::make_shared<worker_utility::NetworkEvent>(
                  worker_utility::NetworkEvent{
                      .source_id = id_,
                      .message = std::string(cache_buffer_)})});
          memset(cache_buffer_, 0, kBufferLength);
          continue;
        }
        cache_buffer_[head_] = buffer[i];
        head_ = (head_ + 1) % kBufferLength;
        std::cout << "head: " << head_ << "buff: " << cache_buffer_ << std::endl;
      }
      return true;
    } 
    if (errno != EINTR) {
      std::cerr << "Couldnt read from socket: " << id_
                << " -> error: " << strerror(errno) << "\n";
      break;
    }
  }
  return false;
}

void app::net::Socket::Send(const std::string &message) const {

  if (send(id_, message.c_str(), message.length(), 0) == -1)
    std::cerr << "Couldnt send to socket: " << id_
              << " -> error: " << strerror(errno) << "\n";
}
