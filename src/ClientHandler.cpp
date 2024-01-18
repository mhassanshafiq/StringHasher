#include "ClientHandler.h"
#include <iostream>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>


void ClientHandler::handle(int clientSocket, const HashFunction hasher) {
    handle(clientSocket, hasher, recv, send);
}
void ClientHandler::handle(int clientSocket, const HashFunction hasher, RecvFunction recvFunc, SendFunction sendFunc) {
    char buffer[BUFFER_SIZE];
    std::string data;

    while (true) {
        ssize_t bytesReceived = recvFunc(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            break;
        }

        data.append(buffer, bytesReceived);
        std::string::size_type pos = 0;
        // Check for newline character to indicate end of input
        if (((pos = data.find("\r\n")) != std::string::npos) || ((pos = data.find("\n")) != std::string::npos))
        {
            data.resize(pos);
            std::cout << data << std::endl;
            std::string hashResult;
            hashResult = hasher(data);

            std::cout << "Hash: " << hashResult << std::endl;
            ssize_t bytesSent = sendFunc(clientSocket, hashResult.c_str(), hashResult.length(), 0);
            if (bytesSent == -1) {
                std::cerr << "Error sending hash back to client" << std::endl;
            }
            data.clear(); 
            break;
        }
    }

    close(clientSocket);
}
