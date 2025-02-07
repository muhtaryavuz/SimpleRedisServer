#include <chrono>
#include <iostream>
#include <cstring>
#include <unistd.h> 
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <thread>
#include "Utils.hpp"
#include "Client.hpp"

app::Client::~Client() {}

bool app::Client::Init() { 
    try {
    server_ip_ = std::string(std::getenv(kServerIpEnvName) != nullptr
                                ? std::getenv(kServerIpEnvName)
                                : "127.0.0.1");
    auto server_port = std::string(std::getenv(kServerPortEnvName) != nullptr
                                       ? std::getenv(kServerPortEnvName)
                                       : "8080");

    if (not app::net::Utils::IsValidIPv4(server_ip_)) {
      std::cout << "Couldnt get valid ip\n";
      return false;
    }
    server_port_ = std::stoul(server_port);
  } catch (std::exception &ex) {
    std::cout << "Couldnt get ip/port from environment exception:" << ex.what()
              << "\n";
    return false;
  }
  return true;
}

void app::Client::Run() {
  int sockfd;
  struct sockaddr_in serv_addr;
  // Socket oluştur
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    perror("Socket oluşturma hatası");
    exit(EXIT_FAILURE);
  }
  // Sunucu adresini ve portunu ayarla
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(server_port_);
  // Sunucu IP adresini ayarla
  if (inet_pton(AF_INET, server_ip_.c_str(), &serv_addr.sin_addr) <= 0) {
    perror("Geçersiz adres veya adres desteklenmiyor");
    close(sockfd);
    exit(EXIT_FAILURE);
  }
  // Bağlantıyı kur
  if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("Bağlantı hatası");
    close(sockfd);
    exit(EXIT_FAILURE);
  }
  printf("Bağlantı başarılı\n");
  // Bağlantıyı kapat
  if (send(sockfd, (const char *)kStringMessage, sizeof(kStringMessage)/sizeof(kStringMessage[0]), 0) < 0) { 
    perror("Mesaj gönderme hatası"); 
  }
  std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  if (send(sockfd, (const char *)kStringMessage, sizeof(kStringMessage)/sizeof(kStringMessage[0]), 0) < 0) { 
    perror("Mesaj gönderme hatası"); 
  }
  close(sockfd);
}
