#include <sys/epoll.h>

#include <iostream>

#include "NetworkController/Definitions.hpp"
#include "NetworkController/EpollHandler.hpp"

bool app::net::EpollHandler::Init() {

  int m_epoll_fd = epoll_create1(0);
  if (m_epoll_fd == InvalidSocketId) {
    std::cerr << "Epoll create failed\n";
    return false;
  }

  return true;
}

void app::net::EpollHandler::AddToList(int socket_id) {
  struct epoll_event ev;
  ev.events = EPOLLIN;
  ev.data.fd = socket_id;
  if (epoll_ctl(m_epoll_fd, EPOLL_CTL_ADD, socket_id, &ev) == -1) {
    std::cerr << "Epoll ctl failed" << std::endl;
  }
}

std::array<int, app::net::EpollHandler::c_max_events>
app::net::EpollHandler::GetSocketEvents() {

  std::array<int, app::net::EpollHandler::c_max_events> sockets = {
      InvalidSocketId};
  struct epoll_event ev, events[c_max_events];

  int num_of_events =
      epoll_wait(m_epoll_fd, events, c_max_events, c_wait_time_in_milliseconds);
  if (num_of_events == -1) {
    for (int i = 0; i < num_of_events; ++i) {
      sockets[i] = events[i].data.fd;
    }
  } else {
    std::cerr << "Epoll wait failed\n";
  }

  return sockets;
}
