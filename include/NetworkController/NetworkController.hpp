#pragma once

#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>

#include "NetworkController/EpollHandler.hpp"
#include "NetworkController/ServerSocket.hpp"
#include "NetworkController/Socket.hpp"
#include "WorkerModule/WorkerController.hpp"

namespace app::net {
class NetworkController {
private:
  static constexpr char kServerIpEnvName[] = "APP_SERVER_IP";
  static constexpr char kServerPortEnvName[] = "APP_SERVER_PORT";

public:
  NetworkController() = default;
  ~NetworkController();

  bool Init(const std::shared_ptr<worker_utility::WorkerController> &);
  bool Start();
  void Stop();

private:
  void listenServerSocket();
  void addToConnections(int);

  std::shared_ptr<worker_utility::WorkerController> workers_;
  std::unique_ptr<std::thread> thread_;
  std::shared_ptr<ServerSocket> server_socket_;
  EpollHandler epoll_handler_;
  std::unordered_map<int, std::unique_ptr<Socket>> connections_;
  std::mutex conn_mutex_;
  bool is_running_{false};
};
} // namespace app::net