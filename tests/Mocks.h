#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "../src/ClientHandler.h"
#include "../src/HashCalculator.h"
#include "../src/ThreadPool.h"
#include <string>

class MockSocket {
public:
    MockSocket(const std::string& input) : input(input), position(0) {}
    ~MockSocket() {}

    ssize_t recv(char* buffer, size_t size, int flags) {
        size_t bytesRead = 0;
        while (position < input.size() && bytesRead < size) {
            buffer[bytesRead++] = input[position++];
        }
        return bytesRead;
    }

private:
    std::string input;
    size_t position;
};

class MockHashCalculator : public HashCalculator {
public:
    MOCK_METHOD1(calculateSHA, std::string(const std::string&));
    MOCK_METHOD1(calculateSTL, std::string(const std::string&));
};

class MockClientHandler : public ClientHandler {
public:
    MOCK_METHOD2(handle, void(int clientSocket, const std::string& hashType));
};

class MockThreadPool : public ThreadPool {
public:
    MockThreadPool(int n) : ThreadPool(4) {}
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD1(enqueue, void(std::function<void()> task));
};
