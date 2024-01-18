#pragma once
#include <string>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>
#include <functional>
#include "HashCalculator.h"
constexpr int BUFFER_SIZE = 1024;

class ClientHandler {
private:
    using RecvFunction = std::function<ssize_t(int, void*, size_t, int)>;
    using SendFunction = std::function<ssize_t(int, const void*, size_t, int)>;
    using SelectFunction = std::function<int(int, fd_set*, fd_set*, fd_set*, timeval*)>;
    using HashFunction = std::function<std::string(const std::string& data)>;
    static bool Close(int clientSocket);
public:
    static bool handle(int clientSocket, const HashFunction hasher);
    static bool handle(int clientSocket, const HashFunction hasher, RecvFunction recvFunc, SendFunction sendFunc, SelectFunction selectFunc);
};