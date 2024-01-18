#include "Mocks.h"

class ClientHandlerTest : public testing::Test {
protected:
    MockHashCalculator hashCalculator;
    ClientHandler clientHandler;

    static ssize_t MockRecv(int clientSocket, void* buffer, size_t size, int flags) {
        const std::string testMessage = "The brown fox\r\n";
        strncpy(static_cast<char*>(buffer), testMessage.c_str(), size);
        return testMessage.length();
    }

    static ssize_t MockSelect(int clientSocket, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout) {
       
        return 1;
    }

    static ssize_t MockSend(int clientSocket, const void* buffer, size_t size, int flags) {
        return size;
    }

    static ssize_t MockRecvError(int clientSocket, void* buffer, size_t size, int flags) {
        return -1;
    }

    static ssize_t MockSelectError(int clientSocket, fd_set* readfds, fd_set* writefds, fd_set* exceptfds, timeval* timeout) {

        return -1;
    }

    static ssize_t MockSendError(int clientSocket, const void* buffer, size_t size, int flags) {
        return -1;
    }
};

TEST_F(ClientHandlerTest, HandleWithSHA) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSend, MockSelect);

}

TEST_F(ClientHandlerTest, HandleWithSTL) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSTL(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSHA(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSTL, &hashCalculator, clientInput), MockRecv, MockSend, MockSelect);

}

TEST_F(ClientHandlerTest, HandleSendError) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSendError, MockSelect);

}

TEST_F(ClientHandlerTest, HandleRecvError) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSTL(clientInput)).Times(0);
    EXPECT_CALL(hashCalculator, calculateSHA(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSTL, &hashCalculator, clientInput), MockRecvError, MockSend, MockSelect);

}

TEST_F(ClientHandlerTest, HandleSelect) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSend, MockSelect);

}

TEST_F(ClientHandlerTest, HandleSelectError) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(0);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSend, MockSelectError);

}