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
  void insertToQueue(worker_variant_t const &);
  std::size_t getQueueSize();
  void stopExecution();

private:
  void process();

private:
  std::queue<worker_variant_t> m_queue;
  std::unique_ptr<std::thread> m_thread;
  std::shared_ptr<Publishable> m_publisher;
  std::mutex m_mutex;
  std::condition_variable m_cv;
  int m_id{-1};
  bool m_is_running{true};
};
} // namespace worker_utility