#pragma once

#include <array>

namespace app::net {
class EpollHandler {
private:
  static const int kMaxEvents = 10;
  static const int kWaitTimeInMilliseconds = 50;

public:
  EpollHandler() = default;
  ~EpollHandler() = default;

  bool Init();
  void AddToList(int) const;
  std::array<int, kMaxEvents> GetSocketEvents() const;

private:
  int epoll_fd_{-1};
};
} // namespace app::net