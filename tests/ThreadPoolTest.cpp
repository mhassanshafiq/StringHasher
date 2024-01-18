#include "gtest/gtest.h"
#include "../src/ThreadPool.h"

TEST(ThreadPoolTest, BasicFunctionality) {
    ThreadPool pool(4);
    pool.start();

    std::atomic<int> counter(0);

    auto task = [&counter]() {
        counter.fetch_add(1);
    };

    auto task_ = [&counter]() {
        counter.fetch_sub(1);
    };

    // Enqueue tasks
    for (int i = 0; i < 20; ++i) {
        pool.enqueue(task);
    }    

    for (int i = 0; i < 10; ++i) {
        pool.enqueue(task_);
    }

    // Wait for tasks to complete
    sleep(1);

    EXPECT_EQ(counter.load(), 10);
}

TEST(ThreadPoolTest, EmptyThreadPool) {
    ThreadPool pool(0);
    pool.start();

    std::atomic<int> counter(0);

    auto task = [&counter]() {
        counter.fetch_add(1);
    };

    // Enqueue tasks
    for (int i = 0; i < 20; ++i) {
        pool.enqueue(task);
    }
    // Wait for tasks to complete
    sleep(1);

    EXPECT_EQ(counter.load(), 0);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
