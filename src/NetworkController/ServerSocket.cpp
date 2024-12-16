#include <iostream>

#include "NetworkController/ServerSocket.hpp"

app::net::ServerSocket::~ServerSocket()
{
    close(m_id);
}

int app::net::ServerSocket::CreateSocket(std::string ip, port_t port) {

  m_id = socket(AF_INET, SOCK_STREAM, 0);
  if (m_id == InvalidSocketId) {
    std::cerr << "Socket creation failed\n";
    return InvalidSocketId;
  }
  if (setsockopt(m_id, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &m_options,
                 sizeof(m_options)) == -1) {
    std::cerr << "Setsockopt error\n";
    return InvalidSocketId;
  }

  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = INADDR_ANY;
  m_address.sin_port = htons(port.value);

  if (bind(m_id, (struct sockaddr *)&m_address, sizeof(m_address)) < 0) {
    std::cerr << "Socket binding failed\n";
    return InvalidSocketId;
  }
  
  if (listen(m_id, 3) < 0) {
    std::cerr << "Socket listening failed\n";
    return InvalidSocketId;
  }
  std::cout << "Socket is created successfully fd: " << m_id << "\n";
  return m_id;
}

int app::net::ServerSocket::Accept() const{
  struct sockaddr_in client_address;
  int new_socket = accept(m_id, (struct sockaddr*)&client_address, (socklen_t*)&client_address);
  if (new_socket < 0) {
    std::cerr << "Socket accept error" << std::endl; 
    return InvalidSocketId;
  }

  char client_ip[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, &client_address.sin_addr, client_ip, INET_ADDRSTRLEN);
  int client_port = ntohs(client_address.sin_port);
  std::cout << "New connection -> socket: " << m_id << ", IP: " << client_ip << ", PORT: " << client_port << "\n";

  return new_socket;
}
