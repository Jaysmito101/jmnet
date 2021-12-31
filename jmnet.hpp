#ifndef JMNET_HPP
#define JMNET_HPP

// The Library is documented within the header itself so just read along
// For any additional queries you can either
// 1. Open a issue on github : https://github.com/Jaysmito101/jmnet/issues
// 2. Email me at : jaysmito101@gmail.com
// 3. Reach out to me on discord : https://discord.gg/BmCFa5X8GB

// Define this before including jmnet.hpp to get access
// to the native platform dependent functions adn handles
#ifdef JMNET_NATIVE_ACCESS

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#else // For Unix Systems

#endif // _WIN32

#endif // JMNET_NATIVE_ACCESS

#include <functional>
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>

namespace jmnet
{
    // The socket protocol
    enum Protocol
    {
        TCP,
        UDP
    };

    // To be reported to the users
    struct Error
    {
        Error(std::string message = "", int code = 0);

        std::string message;
        int code; // OPTIONAL : Only provided if required else 0
    };

    // This is used to hold important data for initialization
    struct NetworkInfo
    {
        NetworkInfo();

        // This function is called whenever an error occurs
        // Its return value is used to determine whether to throw a std::runtime_error or not
        std::function<bool(Error)> OnError;
    };

    // This function is used to initialize low level networking interface (like WinSock2 on Windows)
    // Returns false if an error occurs
    // This must be called before any other function in this library
    bool Initialize(NetworkInfo info = NetworkInfo());

    // This function is used to shutdown low level networking interface (like WinSock2 on Windows)
    // Returns false if an error occurs
    // This must be called after you are done with the networking
    bool Shutdown();

    // The Address class
    class Address
    {
        public:
        Address(std::string ip = "127.0.0.1", int port = 80);

        void* GetNativeAddress(Protocol protocol);

        private:
        std::string ip;
        int port;
    };

    // The Socket Class
    class Socket
    {
        public:        
        Socket(Protocol protocol = Protocol::TCP);
        ~Socket();

        // Returns the native socket handle
        void* GetHandle();

        // This function is used to set the handle
        void SetHandle(void* handle);

        // Recieve data from the socket
        int Receive(void* data, int size);

        // Send data to the socket
        int Send(void* data, int size);

        // Shutdown the socket's Send Functionality
        // If you call Send after this, it will cause an error
        bool ShutdownSend();

        // Shutdown the socket's Recieve Functionality
        // If you call Recieve after this, it will cause an error
        bool ShutdownRecieve();
        
        // Close the socket
        bool Close();

        template<typename T>
        T Receive()
        {
            T data;
            int size = sizeof(T);
            int recieved = Receive(&data, size);
            return data;
        }

        template<typename T>
        inline int Send(T data)
        {
            return Send(&data, sizeof(T));
        }

        int Send(std::string data);

        int Send(char* data);

        protected:
        // The native socket handle
        void* nativeHandle;        
        bool isOpen, isSendOpen, isRecieveOpen;
        Protocol protocol;
    };   

    // The TCP Client Class
    class TCPClient : public Socket
    {
        public:
        TCPClient();
        ~TCPClient();

        // Connect to a server
        bool Connect(Address address);

        // Disconnect from server
        bool Disconnect();

        bool Reconnect();
                
        private:
        Address lastAddress;
    };

    // The TCP Server
    class TCPServer
    {
        public:
        TCPServer(int port = 3000);
        ~TCPServer();

        void* GetNativeHandle();
        bool Listen(int maxConnections = 10);
        Socket* Accept();
        bool Close();

        private:
        int port;
        int maxConnections;
        void* nativeHandle;
    };
}



// Following are the implementations of the classes and functions
// which should be in a seperate .cpp file. But they have been
// included inside the header itself with ifdefs for convinience of setup.
// 
// You can make a cpp file and include jmnet.hpp like :
// 
// #define JMNET_IMPLEMENTATION
// #include "jmnet.hpp"

#ifdef JMNET_IMPLEMENTATION

#ifndef JMNET_NATIVE_ACCESS

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#else // For Unix Systems

#endif // _WIN32

#endif // JMNET_NATIVE_ACCESS

namespace jmnet
{
    static NetworkInfo s_info;

#ifdef _WIN32

    static WSADATA s_wsaData; // The main WSA Context.

#else // For Unix Systems

#endif // _WIN32

    // This is the default error handler
    // This just prints the error message to the console
    static bool DefaultOnError(Error error)
    {
        std::cout << error.message << std::endl;
        if(error.code != 0)
            std::cout << "Error Code : " << error.code << std::endl;
        return false;
    }

    Error::Error(std::string message, int code)
    {
        this->message = message;
        this->code = code;
    }

    NetworkInfo::NetworkInfo()
    {
        OnError = DefaultOnError;
    }

    Address::Address(std::string ip, int port)
    {
        this->ip = ip;
        this->port = port;
    }

    void* Address::GetNativeAddress(Protocol protocol)
    {
        #ifdef _WIN32
        addrinfo *result = NULL, *ptr = NULL, hints;
        ZeroMemory(&hints, sizeof (hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        if(protocol == Protocol::TCP)
            hints.ai_protocol = IPPROTO_TCP;
        else
            hints.ai_protocol = IPPROTO_UDP;
        hints.ai_flags = AI_PASSIVE;

        // Resolve the local address and port to be used by the server
        int resultI = getaddrinfo(ip.data(), std::to_string(port).data(), &hints, &result);
        if (resultI != 0) {
            if(s_info.OnError(Error("getaddrinfo failed", resultI)))
                return nullptr;
        }
        return result;

    #else // For Unix Systems
    #endif // _WIN32

    }

    int Socket::Send(std::string data)
    {
        return Send((void*)data.data(), (int)data.length());
    }

    int Socket::Send(char* data)
    {
        return Send(data, strlen(data));
    }

    void* Socket::GetHandle()
    {
        return nativeHandle;
    }

    void Socket::SetHandle(void* handle)
    {
        nativeHandle = handle;
    }

    int Socket::Send(void* data, int size)    
    {
        if(!isOpen)
        {
            if(s_info.OnError(Error("Socket is not open", 0)))
                throw std::runtime_error("Socket is not open");
            return -1;
        }

        if(!isSendOpen)
        {
            if(s_info.OnError(Error("Socket is not open for sending", 0)))
                throw std::runtime_error("Socket is not open for sending");
            return -1;
        }
        
        #ifdef _WIN32

        int result = send(reinterpret_cast<SOCKET>(nativeHandle), (const char*)data, size, 0);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while sending data", error)))
                throw std::runtime_error("Error while sending data");
            return -1;
        }
        return result;

        #else // For Unix Systems
        #endif // _WIN32
    }

    int Socket::Receive(void* data, int size)
    {
        if(!isOpen)
        {
            if(s_info.OnError(Error("Socket is not open", 0)))
                throw std::runtime_error("Socket is not open");
            return -1;
        }

        if(!isRecieveOpen)
        {
            if(s_info.OnError(Error("Socket is not open for recieving", 0)))
                throw std::runtime_error("Socket is not open for recieving");
            return -1;
        }

        #ifdef _WIN32

        int result = recv(reinterpret_cast<SOCKET>(nativeHandle), (char*)data, size, 0);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while receiving data", error)))
                throw std::runtime_error("Error while receiving data");
            return -1;
        }
        return result;
        
        #else // For Unix Systems
        #endif // _WIN32
    }

    bool Socket::ShutdownRecieve()
    {
        #ifdef _WIN32

        int result = shutdown(reinterpret_cast<SOCKET>(nativeHandle), SD_RECEIVE);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while shutting down recieve", error)))
                throw std::runtime_error("Error while shutting down recieve");
            return false;
        }
        isRecieveOpen = false;
        return true;

        #else // For Unix Systems
        #endif // _WIN32
    }

    bool Socket::ShutdownSend()
    {
        #ifdef _WIN32

        int result = shutdown(reinterpret_cast<SOCKET>(nativeHandle), SD_SEND);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while shutting down send", error)))
                throw std::runtime_error("Error while shutting down send");
            return false;
        }
        isSendOpen = false;
        return true;

        #else // For Unix Systems
        #endif // _WIN32
    }

    bool Socket::Close()
    {
        #ifdef _WIN32

        int result = closesocket(reinterpret_cast<SOCKET>(nativeHandle));
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while closing socket", error)))
                throw std::runtime_error("Error while closing socket");
            return false;
        }
        isOpen = false;
        return true;
        #else // For Unix Systems
        #endif // _WIN32
    }


    Socket::Socket(Protocol protocol)
    {
        this->protocol = protocol;
        nativeHandle = nullptr;
        isSendOpen = true;
        isRecieveOpen = true;
        isOpen = true;
    }

    Socket::~Socket()
    {
        //if(isOpen)
        //    this->Close();
    }

    TCPClient::TCPClient()
    {
        
    }
    
    TCPClient::~TCPClient()
    {
        this->Close();
    }

    bool TCPClient::Connect(Address address)
    {
        lastAddress = address;
        #ifdef _WIN32

        addrinfo* addr = (addrinfo*)address.GetNativeAddress(Protocol::TCP);
        SOCKET sock = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        if (sock == INVALID_SOCKET) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while creating socket", error)))
                throw std::runtime_error("Error while creating socket");
            return false;
        }
        int result = connect(sock, addr->ai_addr, (int)addr->ai_addrlen);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while connecting to server", error)))
                throw std::runtime_error("Error while connecting to server");
            return false;
        }
        freeaddrinfo(addr);
        nativeHandle = reinterpret_cast<void*>(sock);
        #else // For Unix Systems
        #endif // _WIN32
        return true;
    }

    bool TCPClient::Reconnect()
    {
        return Connect(lastAddress);
    }

    bool TCPClient::Disconnect()
    {
        return this->Close();
    }

    TCPServer::TCPServer(int port)
    {
        this->port = port;
        int result;
        #ifdef _WIN32
        addrinfo *res = NULL, hints;
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        hints.ai_flags = AI_PASSIVE;
        result = getaddrinfo(NULL, std::to_string(port).c_str(), &hints, &res);
        if (result != 0) {
            if(s_info.OnError(Error("Error while getting address info", result)))
                throw std::runtime_error("Error while getting address info");
            return;
        }
        SOCKET sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
        if (sock == INVALID_SOCKET) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while creating server socket", error)))
                throw std::runtime_error("Error while creating server socket");
            return;
        }

        result = bind(sock, res->ai_addr, (int)res->ai_addrlen);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while binding server socket", error)))
                throw std::runtime_error("Error while binding server socket");
            return;
        }

        freeaddrinfo(res);

        nativeHandle = reinterpret_cast<void*>(sock);
        #else // For Unix Systems
        #endif // _WIN32
    }

    TCPServer::~TCPServer()
    {
        this->Close();
    }

    void* TCPServer::GetNativeHandle()
    {
        return nativeHandle;
    }

    bool TCPServer::Listen(int maxConnections)
    {
        this->maxConnections = maxConnections;
        int result;
        #ifdef _WIN32

        result = listen(reinterpret_cast<SOCKET>(nativeHandle), maxConnections);
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while listening", error)))
                throw std::runtime_error("Error while listening");
            return false;
        }

        #else // For Unix Systems
        #endif // _WIN32
        return true;
    }

    bool TCPServer::Close()
    {
        #ifdef _WIN32
        int result = closesocket(reinterpret_cast<SOCKET>(nativeHandle));
        if (result == SOCKET_ERROR) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while closing socket", error)))
                throw std::runtime_error("Error while closing socket");
            return false;
        }
        #else // For Unix Systems
        #endif // _WIN32
        return true;
    }

    Socket* TCPServer::Accept()
    {
        #ifdef _WIN32
        SOCKET sock = accept(reinterpret_cast<SOCKET>(nativeHandle), NULL, NULL);
        if (sock == INVALID_SOCKET) {
            int error = WSAGetLastError();
            if(s_info.OnError(Error("Error while accepting connection", error)))
                throw std::runtime_error("Error while accepting connection");
            return nullptr;
        }
        Socket* socket = new Socket(Protocol::TCP);
        socket->SetHandle(reinterpret_cast<void*>(sock));
        return socket;
        #else // For Unix Systems
        #endif // _WIN32
        return nullptr;
    }

    // This function is used to initialize low level networking interface (like WinSock2 on Windows)
    bool Initialize(NetworkInfo info)
    {
        s_info = info;
        #ifdef _WIN32
        
        if(WSAStartup(MAKEWORD(2, 2), &s_wsaData) != 0)
        {
            if(!s_info.OnError(Error("WSAStartup failed", WSAGetLastError())))
                throw std::runtime_error("WSAStartup failed");
            return false;
        }
        #else // For Unix Systems
        #endif
        return true;
    }

    // This function is used to shutdown low level networking interface (like WinSock2 on Windows)
    bool Shutdown()
    {
        #ifdef _WIN32
        if(WSACleanup() != 0)
        {
            if(!s_info.OnError(Error("WSACleanup failed", WSAGetLastError())))
                throw std::runtime_error("WSACleanup failed");
            return false;
        }        
        #else // For Unix Systems
        #endif
        return true;
    }


}

#endif // JMNET_IMPLEMENTATION

#endif