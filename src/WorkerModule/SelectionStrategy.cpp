
#include "WorkerModule/SelectionStrategy.hpp"
#include <algorithm>
#include <atomic>
#include <iterator>
#include <mutex>

std::optional<uint16_t> worker_utility::RoundRobinStrategy::execute() {
  return std::make_optional(++m_current_index % m_num_of_workers);
}

void worker_utility::RoundRobinStrategy::publishWorkerCount(
    uint16_t num_of_workers) {
  m_num_of_workers = num_of_workers;
}

std::optional<uint16_t> worker_utility::LoadBalanceStrategy::execute() {

  return std::make_optional(std::distance(
      m_size_vector.begin(),
      std::min_element(m_size_vector.begin(), m_size_vector.end())));
}

void worker_utility::LoadBalanceStrategy::publishSize(uint16_t key,
                                                      size_t value) {
  m_size_vector[key].m_var.store(value);
}

void worker_utility::LoadBalanceStrategy::publishWorkerCount(
    uint16_t num_of_workers) {
  m_num_of_workers = num_of_workers;

  std::generate_n(std::back_inserter(m_size_vector), m_num_of_workers,
                  []() { return AtomicWrapper<size_t>(0); });
}