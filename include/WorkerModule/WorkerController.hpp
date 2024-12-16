#pragma once

#include "WorkerModule/SelectionStrategy.hpp"
#include "WorkerModule/Worker.hpp"
#include <optional>

namespace worker_utility {
class WorkerController {
public:
  explicit WorkerController(
      uint16_t num_of_workers,
      std::shared_ptr<worker_utility::SelectionStrategy> &&strategy);
  ~WorkerController();

  void sendToWorkers(worker_variant_t event);

private:
  uint16_t m_num_of_workers{1};
  std::vector<std::unique_ptr<Worker>> m_workers;
  std::shared_ptr<SelectionStrategy> m_thread_selector{nullptr};
};
} // namespace worker_utility