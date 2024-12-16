#include <cstdlib>
#include <iostream>
#include <mutex>

#include "NetworkController/Definitions.hpp"
#include "NetworkController/NetworkController.hpp"
#include "NetworkController/Utils.hpp"
#include "WorkerModule/Events.hpp"

app::net::NetworkController::~NetworkController() {
  std::cout << "Closing network controller\n";
  Stop();
  if (m_thread->joinable())
    m_thread->join();
}

bool app::net::NetworkController::Init(
    const std::shared_ptr<worker_utility::WorkerController> &workers) {
  std::string server_ip;
  uint16_t port;
  try {
    server_ip = std::string(std::getenv(c_server_ip_env.c_str()) != nullptr
                                ? std::getenv(c_server_ip_env.c_str())
                                : "127.0.0.1");
    auto server_port =
        std::string(std::getenv(c_server_port_env.c_str()) != nullptr
                        ? std::getenv(c_server_port_env.c_str())
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

  m_server_socket = std::make_shared<ServerSocket>();
  if (m_server_socket->CreateSocket(server_ip, port_t{port}) == InvalidSocketId)
    return false;

  if (not m_epoll_handler.Init())
    return false;
  m_epoll_handler.AddToList(m_server_socket->GetSocketId());

  m_workers = workers;
  return true;
}

bool app::net::NetworkController::Start() {
  m_is_running = true;

  m_thread = std::make_unique<std::thread>(
      &app::net::NetworkController::listenServerSocket, this);
  return true;
}

void app::net::NetworkController::Stop() { m_is_running = false; }

void app::net::NetworkController::listenServerSocket() {

  while (m_is_running) {
    auto sockets = m_epoll_handler.GetSocketEvents();

    for (int i = 0; i < sockets.size(); i++) {
      if (sockets[i] != InvalidSocketId) {
        if (sockets[i] == m_server_socket->GetSocketId()) {
          auto new_socket = m_server_socket->Accept();
          if (new_socket != InvalidSocketId) {
            m_epoll_handler.AddToList(new_socket);
            addToConnections(sockets[i]);
          }
        } else {
          auto message = m_connections[sockets[i]]->Read();
          if (not message.empty()) {
            auto event = std::make_shared<worker_utility::NetworkEvent>(worker_utility::NetworkEvent
                {sockets[i], std::move(message)});
            m_workers->sendToWorkers(worker_utility::worker_variant_t{event});
          }
        }
      } else
        break;
    }
  }
}

void app::net::NetworkController::addToConnections(int socket_id) {
  auto new_socket = std::make_unique<Socket>(socket_id);

  std::lock_guard lock(m_conn_mutex);
  m_connections.emplace(socket_id, std::move(new_socket));
}