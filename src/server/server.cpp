#include <iostream>
#include <algorithm>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#include "server.h"
#include "defines.h"

#include <arpa/inet.h> // inet_ntoa

Socket::Socket() :
m_fd(-1)
{
    m_fd = socket(AF_INET, SOCK_STREAM, DEFAULT_PROTOCOL);
    if (m_fd < 0) 
    {
        std::cout << "ERROR opening server socket" << std::endl;
    }

    m_ServAddr.sin_family = AF_INET;  
    m_ServAddr.sin_addr.s_addr = INADDR_ANY;
    m_ServAddr.sin_port = htons(PORTNR);
}

Socket::Socket(int fd) :
    m_fd(fd)
{}

Socket::~Socket() 
{
    close(m_fd);
}

bool Socket::isOpen()
{
    bool ret = true;
    if (m_fd < 0)
    {
        std::cout << "Clinet socket " << m_fd << "is not open";
        ret = false;
    }
    
    return ret;
}

bool Socket::listen()
{
    bool ret = true;

    if (::bind(m_fd, (struct sockaddr *) &m_ServAddr, sizeof(m_ServAddr)) < 0) 
    {
        std::cout << "ERROR on binding" << std::endl;
        ret = false;
    }

    if (::listen(m_fd, MAX_CONN) < 0)
    {
        std::cout << "ERROR on listen to new connections" << std::endl;
        ret = false;    
    }

    std::cout << "Server ready, listening on ip: " << static_cast<uint32_t>(m_ServAddr.sin_addr.s_addr) << 
                 " and port: " << static_cast<uint16_t>(m_ServAddr.sin_port) << std::endl;

    return ret;
}

Socket Socket::accept()
{
    socklen_t cliLen = sizeof(m_CliAddr);

    int cliFd = ::accept(m_fd, (struct sockaddr *) &m_CliAddr, &cliLen);
    if (cliFd < 0) 
    {
        std::cout << "ERROR on accepting new connection with fd: " << cliFd << std::endl;
    }

    std::cout << "Got connection with id: "<< cliFd << std::endl;

    Socket cliSocket = Socket(cliFd);

    return cliSocket;
}

bool Socket::send(std::string message)
{
    bool ret = true;

     if (write(m_fd, message.c_str(), message.size()) < 0)
     { 
          std::cout << "ERROR to send message to" << m_fd << std::endl;
     }

    return ret;
}

bool Socket::receive(char* message)
{
    bool ret = true;

    if (read(m_fd, message, MESSAGE_MAX_SIZE) < 0)
    {
        std::cout << "ERROR to read message from socket" << m_fd << std::endl;
        ret = false;
    }
    
    return ret;
}

int main()
{
    Socket serverSocket = Socket();
    bool ret = serverSocket.listen();
    Socket cliSocket = serverSocket.accept();

    if (ret == true && cliSocket.isOpen())
    {
        std::string message = "Hello Client !";
        cliSocket.send(message);

        char buffer[256];
        cliSocket.receive(buffer);

        std::cout << "Message from client: " << buffer << std::endl;
    }
    else
    {
        std::cout << "ERROR Couldn't establish connection";
    }
    

    return 0;
}