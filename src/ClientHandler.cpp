#include "ClientHandler.h"
#include <iostream>

bool ClientHandler::Close(int clientSocket) { 
    close(clientSocket); 
    return false; 
}

bool ClientHandler::handle(int clientSocket, const HashFunction hasher) {
    return handle(clientSocket, hasher, recv, send, select);
}

bool ClientHandler::handle(int clientSocket, const HashFunction hasher, RecvFunction recvFunc, SendFunction sendFunc, SelectFunction selectFunc) {
    char buffer[BUFFER_SIZE];
    std::string data;
    const int timeoutMicroSeconds = 100000;

    fd_set readSet;
    FD_ZERO(&readSet);
    FD_SET(clientSocket, &readSet);

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = timeoutMicroSeconds;

    int result = selectFunc(clientSocket + 1, &readSet, nullptr, nullptr, &timeout);

    if (result == -1) {
        std::cerr << "Error in select" << std::endl;
        return ClientHandler::Close(clientSocket);
    }
    else if (result == 0) {
        return true;
    }

    while (true) {
        ssize_t bytesReceived = recvFunc(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            return ClientHandler::Close(clientSocket);
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
    return ClientHandler::Close(clientSocket);
}
