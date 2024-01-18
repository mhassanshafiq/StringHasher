#pragma once
#include <string>
#include <memory>
#include "ThreadPool.h"
#include "ClientHandler.h"

class Server {
public:
    Server(int port, int numThreads, const std::string& hashType);
    Server(int port, int numThreads, const std::string& hashType, std::shared_ptr <ThreadPool> threadPool, std::shared_ptr <ClientHandler> clientHandler);

    ~Server();

    void start();
    void stop();

private:
    class ServerImpl;
    ServerImpl* impl;
};
