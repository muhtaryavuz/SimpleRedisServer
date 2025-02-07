
#include "WorkerModule/SelectionStrategy.hpp"
#include <algorithm>
#include <atomic>
#include <iterator>

std::optional<uint16_t> worker_utility::RoundRobinStrategy::execute() {
  return std::make_optional(++current_index_ % num_of_workers_);
}

void worker_utility::RoundRobinStrategy::publishWorkerCount(
    uint16_t num_of_workers) {
  num_of_workers_ = num_of_workers;
}

std::optional<uint16_t> worker_utility::LoadBalanceStrategy::execute() {

  return std::make_optional(std::distance(
      size_vector_.begin(),
      std::min_element(size_vector_.begin(), size_vector_.end())));
}

void worker_utility::LoadBalanceStrategy::publishSize(uint16_t key,
                                                      size_t value) {
  size_vector_[key].m_var.store(value);
}

void worker_utility::LoadBalanceStrategy::publishWorkerCount(
    uint16_t num_of_workers) {
  num_of_workers_ = num_of_workers;

  std::generate_n(std::back_inserter(size_vector_), num_of_workers_,
                  []() { return AtomicWrapper<size_t>(0); });
}