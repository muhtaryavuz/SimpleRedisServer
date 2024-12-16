#pragma once

#include <memory>
#include <string>
#include <variant>

namespace worker_utility {
struct NetworkEvent {
  int source_id{-1};
  std::string message;
};

using worker_variant_t = std::variant<std::shared_ptr<NetworkEvent>>;
} // namespace worker_utility