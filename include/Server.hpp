#pragma once

#include "NetworkController/NetworkController.hpp"
#include "WorkerModule/WorkerController.hpp"

namespace app {
class Server {
public:
  Server() = default;
  ~Server();

  bool Init();
  void Run();

private:
  app::net::NetworkController network_controller_;
  std::shared_ptr<worker_utility::WorkerController> workers_;
};
} // namespace app