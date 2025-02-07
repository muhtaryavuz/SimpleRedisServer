#include "WorkerModule/WorkerController.hpp"
#include "WorkerModule/Exceptions.hpp"
#include <algorithm>
#include <iterator>
#include <memory>

worker_utility::WorkerController::WorkerController(
    uint16_t num_of_workers,
    std::shared_ptr<worker_utility::SelectionStrategy> &&strategy)
    : num_of_workers_(num_of_workers), thread_selector_(std::move(strategy)) {

  if (num_of_workers_ == 0 || not thread_selector_)
    throw worker_utility::WorkerInitializationError();

  thread_selector_->publishWorkerCount(num_of_workers_);

  try {
    for (int i = 0; i < num_of_workers_; i++) {
      workers_.emplace_back(std::make_unique<Worker>(i, thread_selector_));
      workers_[i]->init("Worker" + std::to_string(i));
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    throw worker_utility::WorkerInitializationError();
  }
}

worker_utility::WorkerController::~WorkerController() {
  std::for_each(workers_.begin(), workers_.end(),
                [](auto &itr) { itr->stopExecution(); });
}

void worker_utility::WorkerController::sendToWorkers(worker_variant_t&& event) {

  auto index = thread_selector_->execute();
  if (not index.has_value() || index.value() >= num_of_workers_)
    throw worker_utility::WorkerSelectionError();
  workers_[index.value()]->insertToQueue(std::move(event));
}