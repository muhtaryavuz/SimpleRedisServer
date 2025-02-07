#pragma once

#include <string>

namespace app {
class Client {
private:
  static constexpr char kServerIpEnvName[] = "APP_SERVER_IP";
  static constexpr char kServerPortEnvName[] = "APP_SERVER_PORT";

  static constexpr char kStringMessage[] = "+text1\r\n+text2";

public:
  Client() = default;
  ~Client();

  bool Init();
  void Run();

private:
  std::string server_ip_;
  uint16_t server_port_ {std::numeric_limits<uint16_t>::max()};
};
} // namespace app