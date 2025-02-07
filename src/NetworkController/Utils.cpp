#include <regex>

#include "NetworkController/Utils.hpp"

bool app::net::Utils::IsValidIPv4(const std::string &ip) {
  const std::regex pattern(R"((\d{1,3}\.){3}\d{1,3})");
  return std::regex_match(ip, pattern);
}