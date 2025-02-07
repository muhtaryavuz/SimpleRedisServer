#pragma once

#include <string>
#include <regex>

namespace app::net {
class Utils {
public:
  Utils() = default;
  ~Utils() = default;

  [[nodiscard]] static bool IsValidIPv4(const std::string &ip){
    const std::regex pattern(R"((\d{1,3}\.){3}\d{1,3})");
    return std::regex_match(ip, pattern);
  }
};
} // namespace app::net