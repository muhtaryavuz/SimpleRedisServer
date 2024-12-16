#include <iostream>
#include <cstring>

#include "NetworkController/Socket.hpp"

app::net::Socket::Socket(int socket_id) : m_id(socket_id){};

app::net::Socket::~Socket() { close(m_id); }

std::string app::net::Socket::Read(){
    
    ssize_t bytes_read{0};
    std::memset(m_buffer, 0, c_buffer_length);
    while (true)
    {
        bytes_read = read(m_id, m_buffer, c_buffer_length);

        if (bytes_read >= 0)
        {
            std::cout<< "FROM fd: " << m_id << " -> " <<m_buffer<<"\n";
            return std::string(m_buffer);
        }
        else if (errno == EINTR)
        {
            continue;
        }
        else 
        {
            std::cerr << "Couldnt read from socket: " << m_id <<" -> error: " << strerror(errno)<< "\n";
            break;
        }
    }
    return "";
}

void app::net::Socket::Send(std::string message){
    
    if (send(m_id, message.c_str(), message.length(), 0) == -1)
        std::cerr << "Couldnt send to socket: " << m_id <<" -> error: " << strerror(errno)<< "\n";
}
