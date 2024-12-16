#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "Server.hpp"
#include "WorkerModule/SelectionStrategy.hpp"
#include "WorkerModule/WorkerController.hpp"

app::Server::~Server() {
  std::cout << "Closing server\n";
  m_network_controller.Stop();
}

bool app::Server::Init() {
  std::cout << "Initializing server ...\n";

  m_workers = std::make_shared<worker_utility::WorkerController>(
      5, std::make_shared<worker_utility::LoadBalanceStrategy>());

  if (not m_network_controller.Init(m_workers))
    return false;
  return true;
}

void app::Server::Run() {
  std::cout << "Server is RUNNING\n";
  if (m_network_controller.Start())
    std::cout << "Network controller is RUNNING\n";
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}