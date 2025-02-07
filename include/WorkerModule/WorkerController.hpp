#pragma once

#include "WorkerModule/SelectionStrategy.hpp"
#include "WorkerModule/Worker.hpp"

namespace worker_utility {
class WorkerController {
public:
  explicit WorkerController(
      uint16_t num_of_workers,
      std::shared_ptr<worker_utility::SelectionStrategy> &&strategy);
  ~WorkerController();

  void sendToWorkers(worker_variant_t&& event);

private:
  uint16_t num_of_workers_{1};
  std::vector<std::unique_ptr<Worker>> workers_;
  std::shared_ptr<SelectionStrategy> thread_selector_{nullptr};
};
} // namespace worker_utility