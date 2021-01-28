#include <iostream>
#include <algorithm>
#include "Defines.hpp"
#include "Socket.hpp"

int main()
{
    ServerSocket servSocket = ServerSocket();

    if (servSocket.isOpen())
    {
        ClientSocket cliSocket = servSocket.accept();

        // SEND
        std::vector<std::uint8_t> buffer;
        std::string sendMsg = "Hi CLIENT!\n";

        std::transform(sendMsg.begin(), sendMsg.end(), std::back_inserter(buffer),
                    [](char c) { return static_cast<std::uint8_t>(c); });

        cliSocket.send(buffer);

        // RECEIVE
        std::vector<std::uint8_t> recBuffer(MESSAGE_MAX_SIZE);
        std::string msg{};

        cliSocket.receive(recBuffer);

        std::transform(recBuffer.begin(), recBuffer.end(), std::back_inserter(msg),
                    [](std::uint8_t b) { return static_cast<char>(b); });

        std::cout << "SERVER: " << msg;
    }

    return 0;
}