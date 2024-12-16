#pragma once

#include "WorkerModule/Events.hpp"

namespace service{
class EventExecutor {
  public:
  void operator()(const std::shared_ptr<worker_utility::NetworkEvent> ) const;
};
}  // namespace service