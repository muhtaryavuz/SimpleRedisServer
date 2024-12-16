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
  void publishSize(uint16_t, size_t) override{};
  void publishWorkerCount(uint16_t) override{};
};

class RoundRobinStrategy : public SelectionStrategy {
public:
  RoundRobinStrategy() = default;
  ~RoundRobinStrategy() = default;

  std::optional<uint16_t> execute() override;

  void publishWorkerCount(uint16_t) override;

private:
  uint16_t m_current_index{0};
  uint16_t m_num_of_workers{0};
};

class LoadBalanceStrategy : public SelectionStrategy {
public:
  LoadBalanceStrategy() = default;
  ~LoadBalanceStrategy() = default;

  std::optional<uint16_t> execute() override;

  void publishSize(uint16_t key, size_t value) override;
  void publishWorkerCount(uint16_t) override;

private:
  uint16_t m_num_of_workers{0};
  std::vector<AtomicWrapper<size_t>> m_size_vector;
};
} // namespace worker_utility