#define JMNET_IMPLEMENTATION
#include "jmnet.hpp"

#include <iostream>

int main()
{
    // Initialize the low level networking stuff
    jmnet::Initialize();
    
    std::cout << "Create Socket" << std::endl;
    jmnet::TCPClient client; // Create the client

    std::cout << "Connect" << std::endl;
    client.Connect(jmnet::Address("example.com", 80));

    std::cout << "Request" << std::endl;
    std::string request = R"(GET / HTTP/1.1
Accept-Language: en-us
Accept-Encoding: gzip, deflate
Connection: Keep-Alive)";
    client.Send(request);

    char data[4096];
    std::cout << "Recieve" << std::endl;
    client.Receive(data, sizeof(data));

    std::cout << "Data : \n" << data << std::endl;

    // Cleanup
    jmnet::Shutdown();
}