#include <cstddef>
#include <vector>
// #include <netdb.h>

class Socket
{
public:
    Socket();
    ~Socket();

    bool isOpen();
    bool connect();
    bool send(std::string message);
    bool receive(char* message);
    
private:
    int m_fd;
    struct sockaddr_in m_ServAddr;
};