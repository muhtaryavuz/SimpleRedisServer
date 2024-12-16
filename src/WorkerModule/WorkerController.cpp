#include "WorkerModule/WorkerController.hpp"
#include "WorkerModule/Exceptions.hpp"
#include <algorithm>
#include <iterator>
#include <memory>

worker_utility::WorkerController::WorkerController(
    uint16_t num_of_workers,
    std::shared_ptr<worker_utility::SelectionStrategy> &&strategy)
    : m_num_of_workers(num_of_workers), m_thread_selector(std::move(strategy)) {

  if (m_num_of_workers == 0 || not m_thread_selector)
    throw worker_utility::WorkerInitializationError();

  m_thread_selector->publishWorkerCount(m_num_of_workers);

  try {
    for (int i = 0; i < m_num_of_workers; i++) {
      m_workers.emplace_back(std::make_unique<Worker>(i, m_thread_selector));
      m_workers[i]->init("Worker" + std::to_string(i));
    }
  } catch (std::exception &e) {
    std::cout << e.what() << std::endl;
    throw worker_utility::WorkerInitializationError();
  }
}

worker_utility::WorkerController::~WorkerController() {
  std::for_each(m_workers.begin(), m_workers.end(),
                [](auto &itr) { itr->stopExecution(); });
}

void worker_utility::WorkerController::sendToWorkers(worker_variant_t event) {

  auto index = m_thread_selector->execute();
  if (not index.has_value() || index.value() >= m_num_of_workers)
    throw worker_utility::WorkerSelectionError();
  m_workers[index.value()]->insertToQueue(event);
}