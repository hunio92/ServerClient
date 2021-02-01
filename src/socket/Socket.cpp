#include <iostream>
#include <algorithm>
#include <cstring>

#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>
#include <netdb.h>

#include "Socket.hpp"
#include "Defines.hpp"

ClientSocket::ClientSocket(std::string ipAddr, std::int32_t port) :
m_fd(-1)
{
    m_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (!m_fd)
    {
        std::cerr << "ERROR: " << "Unable to create socket !" << std::endl;
        std::exit(-1);
    }

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ipAddr.c_str());
    address.sin_port = htons(port);

    std::cout << "CLIENT: "<< "Client is connecting to ip: " << static_cast<std::uint32_t>(address.sin_addr.s_addr) << 
                 " and port: " << static_cast<uint16_t>(address.sin_port) <<std::endl;

    if(::connect(m_fd, (sockaddr*)(&address), sizeof(address)) != 0)
    {
        std::cerr << "ERROR: " << "Unable to connect to server !" << std::endl;
        exit(-1);
    }
}

ClientSocket::ClientSocket(std::uint32_t fd) :
    m_fd(fd)
{}

ClientSocket::~ClientSocket() 
{
    close(m_fd);
}

bool ClientSocket::isOpen()
{
    bool ret = true;

    if (m_fd < 0)
    {
        std::cerr << "ERROR: " << "Client with fd" << m_fd << "is not open";
        ret = false;
    }

    return ret;
}

bool ClientSocket::send(const std::vector<std::uint8_t>& buffer)
{
    std::size_t bytesSent = ::send(m_fd, buffer.data(), buffer.size(), 0);
    return bytesSent == buffer.size();
}

// bool ClientSocket::receive(std::vector<std::uint8_t>& buffer)
// {
//     std::size_t bytesRead = ::recv(m_fd, buffer.data(), buffer.size(), 0);
//     return bytesRead == buffer.size();
// }

ServerSocket::ServerSocket() :
m_fd(-1)
{
        m_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (!m_fd)
        {
            std::cerr << "ERROR: " << "Unable to create socket !" << std::endl;
            std::exit(-1);
        }

        m_ServAddr.sin_family = AF_INET;
        m_ServAddr.sin_addr.s_addr = inet_addr(IPADDR);
        m_ServAddr.sin_port = htons(PORTNR);

        std::int32_t opt = 1;
        std::uint8_t setOptRet = setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));
        if (setOptRet < 0)
        {
            std::cerr << "ERROR: " << "Unable to set options for socket !" << std::endl;
            std::exit(-1);
        }

        if (listen())
        {
            std::exit(-1);
        }
}

ServerSocket::~ServerSocket() 
{
    close(m_fd);
}

bool ServerSocket::isOpen()
{
    bool ret = true;
    if (m_fd < 0)
    {
        std::cerr << "ERROR: " << "Server socket with fd " << m_fd << " is not open" << std::endl;
        ret = false;
    }
    
    return ret;
}

bool ServerSocket::listen()
{
    bool ret = false;

    if (::bind(m_fd, (struct sockaddr *) &m_ServAddr, sizeof(m_ServAddr)) < 0) 
    {
        std::cerr << "ERROR: " << "Unable to bind to fd " << m_fd << " !" << std::endl;
        ret = true;
    }

    if (::listen(m_fd, MAX_CONN) < 0)
    {
        std::cerr << "ERROR: " << "Unable to listen on socket " << m_fd << " !" << std::endl;
        ret = true;    
    }

    return ret;
}

ClientSocket ServerSocket::accept()
{
    socklen_t len = sizeof(sockaddr_in);

    int cliFd = ::accept(m_fd, (struct sockaddr *) &m_ServAddr, &len);
    if (cliFd < 0) 
    {
        std::cerr << "ERROR: " << "Could not accept new connection !" << std::endl;
    }

    std::cout << "SERVER: " << "Got connection with fd: "<< cliFd << std::endl;

    return ClientSocket(cliFd);
}

// bool ServerSocket::send(const std::vector<std::uint8_t>& buffer)
// {
//     std::size_t bytesSent = ::send(m_fd, buffer.data(), buffer.size(), 0);
//     return bytesSent == buffer.size();
// }

bool ServerSocket::receive(std::uint32_t fd, std::vector<std::uint8_t>& buffer)
{
    std::size_t bytesRead = ::recv(fd, buffer.data(), buffer.size(), 0);
    return bytesRead == buffer.size();
}

std::vector<ClientSocket> ServerSocket::select()
{
    fd_set master, read_fds;

    FD_ZERO(&master); /* clear the set */
    FD_ZERO(&read_fds); /* clear the set */
    FD_SET(m_fd, &master); /* add our file descriptor to the set */

    std::uint32_t fdmax = m_fd;

    struct timeval timeout;
    timeout.tv_sec = 20;
    timeout.tv_usec = 0;

    std::cout << "### Start Server ###" << std::endl;
    while(true)
    {
        read_fds = master;

        std::uint8_t ret = ::select(m_fd + 1, &read_fds, NULL, NULL, &timeout);
        if (ret == -1)
        {
            std::cerr << "ERROR: " << "select" << std::endl;
            exit(-1);
        }
        else if (ret == 0)
        {
            std::cerr << "ERROR: " << "timeout occurred (20 second) !" << std::endl;
            exit(-1);
        }
        else
        {
            for(int i = 0; i <= fdmax; i++) {
                if(FD_ISSET(i, &read_fds)) 
                {  
                    if(i == m_fd) 
                    {    
                        ClientSocket newsockfd = accept();

                        FD_SET(newsockfd.m_fd, &master);

                        std::cout << "SERVER: " << " server fd " << i << ": accept new client with fd: " << newsockfd.m_fd << std::endl;

                        if(newsockfd.m_fd > fdmax) {
                            fdmax = newsockfd.m_fd;
                        }
                    }
                    else
                    {
                        std::cout << "SERVER: read message from: " << i << std::endl;
                        std::vector<std::uint8_t> recBuffer(MESSAGE_MAX_SIZE);
                        std::string msg{};

                        receive(i, recBuffer);

                        std::transform(recBuffer.begin(), recBuffer.end(), std::back_inserter(msg),
                                [](std::uint8_t b) { return static_cast<char>(b); });

                        std::cout << "SERVER: received message from CLIENT " << i << ": " << msg << std::endl;
                    }
                }
            }
        }
    }
}