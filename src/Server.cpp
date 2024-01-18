#include "Server.h"
#include "ClientHandler.h"
#include "ThreadPool.h"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <csignal>

class Server::ServerImpl {
public:
    ServerImpl(int port, int numThreads, const std::string& hashType);
    ServerImpl(int port, int numThreads, const std::string& hashType, std::shared_ptr <ThreadPool> threadPool, std::shared_ptr <ClientHandler> clientHandler);
    void start();
    void stop();

private:
    int serverSocket;
    int port;
    int numThreads;
    volatile sig_atomic_t g_signalReceived = 0;
    std::string hashType;
    std::shared_ptr < ThreadPool> threadPool;
    std::shared_ptr < ClientHandler> clientHandler;

    void acceptClients(int serverSocket, const std::string& hashType);
};

Server::Server(int port, int numThreads, const std::string& hashType)
    : impl(new ServerImpl(port, numThreads, hashType)) {}

Server::Server(int port, int numThreads, const std::string& hashType, std::shared_ptr<ThreadPool> threadPool, std::shared_ptr <ClientHandler> clientHandler)
    : impl(new ServerImpl(port, numThreads, hashType, threadPool, clientHandler)) {}


Server::~Server() {
    delete impl;
}

void Server::start() {
    impl->start();
}

void Server::stop() {
    impl->stop();
}

Server::ServerImpl::ServerImpl(int port, int numThreads, const std::string& hashType)
    : serverSocket(-1), port(port), numThreads(numThreads), hashType(hashType), threadPool(std::make_shared<ThreadPool>(numThreads)) {}

Server::ServerImpl::ServerImpl(int port, int numThreads, const std::string& hashType, std::shared_ptr <ThreadPool> threadPool, std::shared_ptr <ClientHandler> clientHandler)
    : serverSocket(-1), port(port), numThreads(numThreads), hashType(hashType), threadPool(threadPool), clientHandler(clientHandler)
{
}


void Server::ServerImpl::start() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cerr << "Error creating server socket" << std::endl;
        return;
    }

    // Set SO_REUSEADDR option
    int reuse = 1; // 1 enables the option
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        std::cerr << "Error setting SO_REUSEADDR option" << std::endl;
        close(serverSocket);
        return;
    }

    sockaddr_in serverAddress{};
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(port);

    if (bind(serverSocket, reinterpret_cast<struct sockaddr*>(&serverAddress), sizeof(serverAddress)) == -1) {
        std::cerr << "Error binding server socket" << std::endl;
        close(serverSocket);
        return;
    }

    if (listen(serverSocket, 5) == -1) {
        std::cerr << "Error listening on server socket" << std::endl;
        close(serverSocket);
        return;
    }

    std::cout << "[INFO] Server listening on 0.0.0.0:" << port << std::endl;

    threadPool->start();

    // Accept clients in the main thread
    acceptClients(serverSocket, hashType);
}

void Server::ServerImpl::stop() {
    
     g_signalReceived = 1;
     std::cerr << "closing server socket" << std::endl;
     close(serverSocket);
}

void Server::ServerImpl::acceptClients(int serverSocket, const std::string& hashType) {
    try {
        while (!g_signalReceived) {
            sockaddr_in clientAddress{};
            socklen_t clientAddressLength = sizeof(clientAddress);
            int clientSocket = accept(serverSocket, reinterpret_cast<struct sockaddr*>(&clientAddress), &clientAddressLength);

            if (g_signalReceived)
                break;
            if (clientSocket == -1) {
                std::cerr << "Error accepting client connection" << std::endl;
                continue;
            }
            std::cout << "[INFO] Accepted connection from " << inet_ntoa(clientAddress.sin_addr) << std::endl;

            // Add the client socket to the thread pool for processing
            threadPool->enqueue([clientHandler = this->clientHandler, clientSocket, hashType]() {
                auto hasher = hashType == "SHA" ? HashCalculator::calculateSHA : HashCalculator::calculateSTL;
                clientHandler ? clientHandler->handle(clientSocket, hasher) : ClientHandler::handle(clientSocket, hasher);
                });
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in server: " << e.what() << std::endl;
    }
    threadPool->stop();
}
