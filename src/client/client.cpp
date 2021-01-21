#include <iostream>
#include <algorithm>
#include <cstring>

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>

#include "client.h"
#include "defines.h"

Socket::Socket() :
m_fd(-1)
{
    int clientFd;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_fd < 0) 
    {
        std::cout << "ERROR opening client socket" << std::endl;
        exit(1);
    }

    server = gethostbyname(IPADDR);
    if (server == NULL)
    {
        std::cout << "ERROR no such host" << std::endl;
        exit(1);
    }
    
    serv_addr.sin_family = AF_INET;
    std::memcpy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(PORTNR);

    m_ServAddr = serv_addr;
}

Socket::~Socket() 
{
    close(m_fd);
}

bool Socket::isOpen()
{
    bool ret = true;

    if (m_fd < 0)
    {
        std::cout << "Socket or Host" << m_fd << "is not open";
        ret = false;
    }

    return ret;
}

bool Socket::connect()
{
    bool ret = true;
    
    if (::connect(m_fd, (struct sockaddr *) &m_ServAddr, sizeof(m_ServAddr)) < 0)
    {
        std::cout << "ERROR connecting" << std::endl;
        ret = false;
    }

    std::cout << "Client is connecting to ip: " << static_cast<uint32_t>(m_ServAddr.sin_addr.s_addr) << 
                 " and port: " << static_cast<uint16_t>(m_ServAddr.sin_port) << std::endl;

    return ret;
}

bool Socket::send(std::string message)
{
    bool ret = true;

    if (write(m_fd, message.c_str(), message.size()) < 0)
    { 
        std::cout << "ERROR to send message to" << m_fd << std::endl;
        ret = false;
    }

    return ret;
}

bool Socket::receive(char* message)
{
    bool ret = true;

    if (read(m_fd, message, 256) < 0) // cast needed or not ?!
    {
        std::cout << "ERROR to read message from socket" << m_fd << std::endl;
        ret = false;
    }
    
    return ret;
}

int main()
{
    Socket cliSocket = Socket();

    if (cliSocket.isOpen())
    {
        if(cliSocket.connect())
        {
            std::string message = "Hello Server !";
            cliSocket.send(message);

            char buffer[256];
            cliSocket.receive(buffer);

            std::cout << "Message from server: " << buffer << std::endl;
        }
        else
        {
            std::cout << "ERROR Couldn't establish connection";
        }
    }

    return 0;
}
