#include <cstddef>
#include <vector>

class Socket
{
public:
    Socket();
    Socket(int m_fd);
    ~Socket();

    bool isOpen();
    bool listen();
    Socket accept();
    bool send(std::string message);
    bool receive(char* buffer);
    
private:
    int m_fd;
    struct sockaddr_in m_ServAddr, m_CliAddr;
    // std::vector<Socket> m_SocketArr;
};