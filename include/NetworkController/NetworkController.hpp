#pragma once

#include <memory>
#include <mutex>
#include <unordered_map>
#include <string>
#include <thread>

#include "NetworkController/EpollHandler.hpp"
#include "NetworkController/ServerSocket.hpp"
#include "NetworkController/Socket.hpp"
#include "WorkerModule/WorkerController.hpp"

namespace app::net {
class NetworkController {
public:
  NetworkController() = default;
  ~NetworkController();

  bool Init(const std::shared_ptr<worker_utility::WorkerController>&);
  bool Start();
  void Stop();

private:
  void listenServerSocket();
  void addToConnections(int);

private:
  const std::string c_server_ip_env = "APP_SERVER_IP";
  const std::string c_server_port_env = "APP_SERVER_PORT";

private:
  std::shared_ptr<worker_utility::WorkerController> m_workers;
  std::unique_ptr<std::thread> m_thread;
  std::shared_ptr<ServerSocket> m_server_socket;
  EpollHandler m_epoll_handler;
  std::unordered_map<int, std::unique_ptr<Socket>> m_connections;
  std::mutex m_conn_mutex;
  bool m_is_running{false};
};
} // namespace app::net