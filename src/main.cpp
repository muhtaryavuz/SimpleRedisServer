#include <csignal>
#include <iostream>

#include "Server.hpp"

void signalHandler(int signal)
{
    std::cout << " Interrupt signal (" << signal << ") received : Shutting down server." << std::endl;
    exit(signal);
}

int main(int, char**){
    std::signal(SIGINT, signalHandler);
    
    app::Server server;
    if (server.Init())
    {
        server.Run(); 
    }
    else{
        std::cout << "Server couldnt be initialized\n";
    }   
    return 0; 
}
