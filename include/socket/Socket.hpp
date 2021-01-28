#include <cstddef>
#include <vector>
#include <netinet/in.h>
// #include <netdb.h>

#include <arpa/inet.h> // inet_ntoa

class ClientSocket
{
public:
    ClientSocket(std::string ipAddr, std::int32_t port);
    ClientSocket(std::uint32_t m_fd);
    ~ClientSocket();

    bool isOpen();
    bool connect(sockaddr_in &address);
    bool send(const std::vector<std::uint8_t>& buffer);
    bool receive(std::vector<std::uint8_t>& buffer);

private:
    std::int32_t m_fd;
    sockaddr_in m_CliAddr;
};

class ServerSocket
{
public:
    ServerSocket();
    ServerSocket(std::uint32_t m_fd);
    ~ServerSocket();

    bool isOpen();
    bool listen();
    ClientSocket accept();
    bool send(const std::vector<std::uint8_t>& buffer);
    bool receive(std::vector<std::uint8_t>& buffer);
    
private:
    std::uint32_t m_fd;
    sockaddr_in m_ServAddr;
    // std::vector<Socket> m_SocketArr;
};