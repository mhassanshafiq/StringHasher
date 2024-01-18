#pragma once
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <functional>
#include "HashCalculator.h"
constexpr int BUFFER_SIZE = 1024;

class ClientHandler {
private:
    using RecvFunction = std::function<ssize_t(int, void*, size_t, int)>;
    using SendFunction = std::function<ssize_t(int, const void*, size_t, int)>;
    using HashFunction = std::function<std::string(const std::string& data)>;
public:
    static void handle(int clientSocket, const HashFunction hasher);
    static void handle(int clientSocket, const HashFunction hasher, RecvFunction recvFunc, SendFunction sendFunc);
};