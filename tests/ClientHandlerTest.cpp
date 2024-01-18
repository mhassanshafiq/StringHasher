#include "Mocks.h"

class ClientHandlerTest : public testing::Test {
protected:
    MockHashCalculator hashCalculator;
    ClientHandler clientHandler;

    // Mock recv function for testing
    static ssize_t MockRecv(int clientSocket, void* buffer, size_t size, int flags) {
        const std::string testMessage = "The brown fox\r\n";
        strncpy(static_cast<char*>(buffer), testMessage.c_str(), size);
        return testMessage.length();
    }

    // Mock send function for testing
    static ssize_t MockSend(int clientSocket, const void* buffer, size_t size, int flags) {
        return size;  // Assuming successful send for simplicity
    }

    // Mock recv function for testing
    static ssize_t MockRecvError(int clientSocket, void* buffer, size_t size, int flags) {
        return -1;
    }

    // Mock send function for testing
    static ssize_t MockSendError(int clientSocket, const void* buffer, size_t size, int flags) {
        return -1;  // Assuming successful send for simplicity
    }
};

TEST_F(ClientHandlerTest, HandleWithSHA) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSend);

}

TEST_F(ClientHandlerTest, HandleWithSTL) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSTL(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSHA(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSTL, &hashCalculator, clientInput), MockRecv, MockSend);

}

TEST_F(ClientHandlerTest, HandleSendError) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSHA(clientInput)).Times(1);
    EXPECT_CALL(hashCalculator, calculateSTL(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSHA, &hashCalculator, clientInput), MockRecv, MockSendError);

}

TEST_F(ClientHandlerTest, HandleRecvError) {
    std::string clientInput = "The brown fox\r\n";

    EXPECT_CALL(hashCalculator, calculateSTL(clientInput)).Times(0);
    EXPECT_CALL(hashCalculator, calculateSHA(testing::_)).Times(0);

    clientHandler.handle(0, std::bind(&MockHashCalculator::calculateSTL, &hashCalculator, clientInput), MockRecvError, MockSend);

}