#define JMNET_IMPLEMENTATION
#include "jmnet.hpp"

#include <iostream>

int main()
{
    // Initialize the low level networking stuff
    jmnet::Initialize();
    std::cout << "Hello World!" << std::endl;

    // Create a server
    std::cout << "Creating server..." << std::endl;
    jmnet::TCPServer server(80);
    std::cout << "Server created!" << std::endl;
    server.Listen();
    
    std::cout << "Server listening!" << std::endl;

    while(true){
    jmnet::Socket* client = server.Accept();
    std::cout << "Client accepted!" << std::endl;
    
    std::string op = "Hello From Server!";
    client->Send(op);
    }   
    // Cleanup
    jmnet::Shutdown();
}