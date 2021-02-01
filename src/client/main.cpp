#include <iostream>
#include <algorithm>
#include "Defines.hpp"
#include "Socket.hpp"

#include <unistd.h>

int main()
{
    ClientSocket cliSocket = ClientSocket("127.0.0.1", 8080);

    if (cliSocket.isOpen())
    {
        // SEND
        std::vector<std::uint8_t> buffer;
        std::string sendMsg = "Hello SERVER!\n";
        // std::cin >> sendMsg;

        std::transform(sendMsg.begin(), sendMsg.end(), std::back_inserter(buffer),
                    [](char c) { return static_cast<std::uint8_t>(c); });

        cliSocket.send(buffer);
    }
    
    return 0;
}
