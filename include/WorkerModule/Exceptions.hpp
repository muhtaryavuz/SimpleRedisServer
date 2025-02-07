#pragma once

#include <exception>

namespace worker_utility {
class WorkerInitializationError : public std::exception {
  const char *what() const noexcept final {
    return "Worker thread initialization is failed";
  }
};

class WorkerSelectionError : public std::exception {
  const char *what() const noexcept final {
    return "Worker thread couldnt be selected";
  }
};
} // namespace worker_utility