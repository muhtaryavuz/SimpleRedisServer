#pragma once

#include <iostream>

namespace worker_utility {
class Publishable {
public:
  Publishable() = default;
  virtual ~Publishable() = default;

  virtual void publishSize(uint16_t key, size_t value) = 0;
  virtual void publishWorkerCount(uint16_t) = 0;
};
} // namespace worker_utility