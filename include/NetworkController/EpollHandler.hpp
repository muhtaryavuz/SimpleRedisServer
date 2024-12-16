#pragma once

#include <array>

namespace app::net {
class EpollHandler {
private:
  static const int c_max_events = 10;
  static const int c_wait_time_in_milliseconds = 50;
public:
  EpollHandler() = default;
  ~EpollHandler() = default;

  bool Init();
  void AddToList(int);
  std::array<int, c_max_events> GetSocketEvents();

private:
  int m_epoll_fd{-1};
};
} // namespace app::net