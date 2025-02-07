#include <cstring>
#include <iostream>

#include "NetworkController/ServerSocket.hpp"

app::net::ServerSocket::~ServerSocket() { close(id_); }

int app::net::ServerSocket::CreateSocket(const std::string &ip, port_t port) {

  id_ = socket(AF_INET, SOCK_STREAM, 0);
  if (id_ == kInvalidSocketId) {
    std::cerr << "Socket creation failed\n";
    return kInvalidSocketId;
  }
  if (setsockopt(id_, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &options_,
                 sizeof(options_)) == -1) {
    std::cerr << "Setsockopt error\n";
    return kInvalidSocketId;
  }

  address_.sin_family = AF_INET;
  address_.sin_port = htons(port.value);

  if (inet_pton(AF_INET, ip.c_str(), &address_.sin_addr) <= 0) {
    std::cerr << "Geçersiz adres veya adres desteklenmiyor ip: " << ip
              << std::endl;
    close(id_);
    exit(EXIT_FAILURE);
  }

  if (bind(id_, reinterpret_cast<struct sockaddr *>(&address_),
           sizeof(address_)) < 0) {
    std::cerr << "Socket binding failed\n";
    return kInvalidSocketId;
  }

  if (listen(id_, 3) < 0) {
    std::cerr << "Socket listening failed\n";
    return kInvalidSocketId;
  }
  std::cout << "Socket is created successfully fd: " << id_ << "\n";
  return id_;
}

int app::net::ServerSocket::Accept() const {
  struct sockaddr_in client_address;
  socklen_t addr_length = sizeof(client_address);
  int new_socket = accept(id_, (struct sockaddr *)&client_address, &addr_length);
  if (new_socket < 0) {
    std::cerr << "Socket accept error: " << strerror(errno) << std::endl;
    return kInvalidSocketId;
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(client_address.sin_port);
  std::cout << "New connection -> socket: " << new_socket
            << ", IP: " << client_ip << ", PORT: " << client_port << "\n";

  return new_socket;
}
