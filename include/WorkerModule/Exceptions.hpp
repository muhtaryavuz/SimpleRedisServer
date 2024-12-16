#pragma once

#include <exception>

namespace worker_utility {
class WorkerInitializationError : public std::exception {
  virtual const char *what() const throw() {
    return "Worker thread initialization is failed";
  }
};

class WorkerSelectionError : public std::exception {
  virtual const char *what() const throw() {
    return "Worker thread couldnt be selected";
  }
};
} // namespace worker_utility