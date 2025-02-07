#pragma once

#include "WorkerModule/Publishable.hpp"
#include "WorkerModule/Utils.hpp"

#include <memory>
#include <optional>
#include <vector>

namespace worker_utility {

class SelectionStrategy : public Publishable {
public:
  SelectionStrategy() = default;
  virtual ~SelectionStrategy() = default;

  virtual std::optional<uint16_t> execute() = 0;
  void publishSize(uint16_t, size_t) override {};
  void publishWorkerCount(uint16_t) override {};
};

class RoundRobinStrategy : public SelectionStrategy {
public:
  RoundRobinStrategy() = default;
  ~RoundRobinStrategy() = default;

  std::optional<uint16_t> execute() override;

  void publishWorkerCount(uint16_t) override;

private:
  uint16_t current_index_{0};
  uint16_t num_of_workers_{0};
};

class LoadBalanceStrategy : public SelectionStrategy {
public:
  LoadBalanceStrategy() = default;
  ~LoadBalanceStrategy() = default;

  std::optional<uint16_t> execute() override;

  void publishSize(uint16_t key, size_t value) override;
  void publishWorkerCount(uint16_t) override;

private:
  uint16_t num_of_workers_{0};
  std::vector<AtomicWrapper<size_t>> size_vector_;
};
} // namespace worker_utility