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
    
    std::string op = R"(HTTP/1.1 200 OK
Date: Mon, 27 Jul 2009 12:28:53 GMT
Server: Apache/2.2.14 (Win32)
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT
Content-Length: 88
Content-Type: text/html
Connection: Closed

<html>
<body>
<h1>Hello, World!</h1>
</body>
</html>
    )";
    client->Send(op);
    }   
    // Cleanup
    jmnet::Shutdown();
}