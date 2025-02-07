#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "Server.hpp"
#include "WorkerModule/SelectionStrategy.hpp"
#include "WorkerModule/WorkerController.hpp"

app::Server::~Server() {
  std::cout << "Closing server\n";
  network_controller_.Stop();
}

bool app::Server::Init() {
  std::cout << "Initializing server ...\n";

  workers_ = std::make_shared<worker_utility::WorkerController>(
      5, std::make_shared<worker_utility::LoadBalanceStrategy>());

  return network_controller_.Init(workers_);
}

void app::Server::Run() {
  std::cout << "Server is RUNNING\n";
  if (network_controller_.Start())
    std::cout << "Network controller is RUNNING\n";
  while (true) {
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}