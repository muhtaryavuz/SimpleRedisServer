#include <cstring>
#include <iostream>
#include <sys/epoll.h>

#include "NetworkController/Definitions.hpp"
#include "NetworkController/EpollHandler.hpp"

bool app::net::EpollHandler::Init() {

  epoll_fd_ = epoll_create1(0);
  if (epoll_fd_ == kInvalidSocketId) {
    std::cerr << "Epoll create failed\n";
    return false;
  }
  return true;
}

void app::net::EpollHandler::AddToList(int socket_id) const {
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = socket_id;
  if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_id, &event) == -1) {
    std::cerr << "Epoll ctl failed" << std::endl;
  }
}

std::array<int, app::net::EpollHandler::kMaxEvents>
app::net::EpollHandler::GetSocketEvents() const {

  std::array<int, app::net::EpollHandler::kMaxEvents> sockets = {0};
  struct epoll_event events[kMaxEvents];

  int num_of_events =
      epoll_wait(epoll_fd_, events, kMaxEvents, kWaitTimeInMilliseconds);
  if (num_of_events != -1) {
    for (int i = 0; i < num_of_events; ++i) {
      sockets[i] = events[i].data.fd;
    }
  } else {
    std::cerr << "Epoll wait failed: errno: " << strerror(errno) << std::endl;
  }

  return sockets;
}
