#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>

#include "WorkerModule/Events.hpp"
#include "WorkerModule/Publishable.hpp"

namespace worker_utility {
class Worker {

public:
  explicit Worker(int, std::shared_ptr<Publishable>);
  ~Worker() = default;

  void init(std::string);
  void insertToQueue(worker_variant_t &&);
  std::size_t getQueueSize();
  void stopExecution();

private:
  void process();

  std::queue<worker_variant_t> queue_;
  std::unique_ptr<std::thread> thread_;
  std::shared_ptr<Publishable> publisher_;
  std::mutex mutex_;
  std::condition_variable cv_;
  int id_{-1};
  bool is_running_{true};
};
} // namespace worker_utility