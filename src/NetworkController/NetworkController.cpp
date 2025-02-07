#include <cstdlib>
#include <iostream>
#include <mutex>

#include "NetworkController/Definitions.hpp"
#include "NetworkController/NetworkController.hpp"
#include "NetworkController/Utils.hpp"

app::net::NetworkController::~NetworkController() {
  std::cout << "Closing network controller\n";
  Stop();
  if (thread_->joinable())
    thread_->join();
}

bool app::net::NetworkController::Init(
    const std::shared_ptr<worker_utility::WorkerController> &workers) {
  std::string server_ip;
  uint16_t port;
  try {
    server_ip = std::string(std::getenv(kServerIpEnvName) != nullptr
                                ? std::getenv(kServerIpEnvName)
                                : "127.0.0.1");
    auto server_port = std::string(std::getenv(kServerPortEnvName) != nullptr
                                       ? std::getenv(kServerPortEnvName)
                                       : "8080");

    if (not app::net::Utils::IsValidIPv4(server_ip)) {
      std::cout << "Couldnt get valid ip\n";
      return false;
    }
    port = std::stoul(server_port);
  } catch (std::exception &ex) {
    std::cout << "Couldnt get ip/port from environment exception:" << ex.what()
              << "\n";
    return false;
  }

  server_socket_ = std::make_shared<ServerSocket>();
  if (server_socket_->CreateSocket(server_ip, port_t{port}) == kInvalidSocketId)
    return false;

  if (not epoll_handler_.Init())
    return false;
  epoll_handler_.AddToList(server_socket_->GetSocketId());

  workers_ = workers;
  return true;
}

bool app::net::NetworkController::Start() {
  is_running_ = true;

  thread_ = std::make_unique<std::thread>(
      &app::net::NetworkController::listenServerSocket, this);
  return true;
}

void app::net::NetworkController::Stop() { is_running_ = false; }

void app::net::NetworkController::listenServerSocket() {

  while (is_running_) {
    auto sockets = epoll_handler_.GetSocketEvents();

    for (auto socket : sockets) {
      if (socket != kInvalidSocketId and socket != 0) {
        std::cout << "socket: " << socket << std::endl;
        if (socket == server_socket_->GetSocketId()) {
          auto new_socket = server_socket_->Accept();
          if (new_socket != kInvalidSocketId) {
            epoll_handler_.AddToList(new_socket);
            addToConnections(new_socket);
          }
        } else {
          if (not connections_.at(socket)->Read()) {
            close(socket);
            connections_.erase(socket);
          }
        }
      } else {
        break;
      }
    }
  }
}

void app::net::NetworkController::addToConnections(int socket_id) {
  auto new_socket = std::make_unique<Socket>(socket_id, workers_);

  std::lock_guard lock(conn_mutex_);
  connections_.emplace(socket_id, std::move(new_socket));
}