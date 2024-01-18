#pragma once

#include <thread>
#include <vector>
#include <functional>
#include <queue>
#include <mutex>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(int numThreads);
    ~ThreadPool();

    void start();
    void stop();
    void enqueue(std::function<void()> task);

private:
    void worker();

    int numThreads;
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stopFlag;
};
