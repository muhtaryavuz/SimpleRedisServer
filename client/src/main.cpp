#include <csignal>
#include <iostream>

#include "Client.hpp"

void signalHandler(int signal)
{
    std::cout << " Interrupt signal (" << signal << ") received : Shutting down client." << std::endl;
    exit(signal);
}

int main(int, char**){
    std::signal(SIGINT, signalHandler);
    
    app::Client client;
    if (client.Init())
    {
        client.Run(); 
    }
    else{
        std::cout << "Client couldnt be initialized\n";
    }   
    return 0; 
}
