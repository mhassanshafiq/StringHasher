#include "Mocks.h"
#include "../src/Server.h"


class ServerTest : public testing::Test {
protected:
    void SetUp() override {
        // Add any necessary setup code
    }

    void TearDown() override {
        // Add any necessary cleanup code
    }
};

TEST_F(ServerTest, StartAndStopServer) {
    // Mocks
    std::shared_ptr<MockThreadPool> mockThreadPool = std::make_shared<MockThreadPool>(4);
    std::shared_ptr<MockClientHandler> mockClientHandler = std::make_shared<MockClientHandler>();

    // Server instance
    Server server(8080, 5, "SHA", mockThreadPool, mockClientHandler);

    // Set expectations on mocks
    EXPECT_CALL(*mockThreadPool, start());
    EXPECT_CALL(*mockThreadPool, stop());
    EXPECT_CALL(*mockClientHandler, handle(testing::_, testing::_));

    // Start and stop the server
    server.start();
    server.stop();
}

// Add more test cases as needed based on different scenarios and edge cases
