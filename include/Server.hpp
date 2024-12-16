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
    app::net::NetworkController m_network_controller;
    std::shared_ptr<worker_utility::WorkerController> m_workers;
};
} // namespace app