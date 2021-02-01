#include <iostream>
#include <algorithm>
#include "Defines.hpp"
#include "Socket.hpp"

// https://cpp.hotexamples.com/examples/-/-/FD_SET/cpp-fd_set-function-examples.html

int main()
{
    ServerSocket servSocket = ServerSocket();

    if (servSocket.isOpen())
    {
        while(true)
        {
            std::vector<ClientSocket> cliSockets = servSocket.select();

            

            // for (auto &&cliSocket : cliSockets)
            // {
            //     std::vector<std::uint8_t> recBuffer(MESSAGE_MAX_SIZE);
            //     std::string msg{};

            //     cliSocket.receive(recBuffer);

            //     std::transform(recBuffer.begin(), recBuffer.end(), std::back_inserter(msg),
            //                 [](std::uint8_t b) { return static_cast<char>(b); });

            //     std::cout << "SERVER: " << msg;
            // }
        }
    }

    return 0;
}