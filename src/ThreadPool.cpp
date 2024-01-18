#include "ThreadPool.h"

ThreadPool::ThreadPool(int numThreads) : numThreads(numThreads), stopFlag(false) {}

ThreadPool::~ThreadPool() {
    stop();
}

void ThreadPool::start() {
    for (int i = 0; i < numThreads; ++i) {
        threads.emplace_back(&ThreadPool::worker, this);
    }
}

void ThreadPool::stop() {
    stopFlag = true;
    condition.notify_all();

    for (auto& thread : threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
}

void ThreadPool::enqueue(std::function<void()> task) {
    {
        std::unique_lock<std::mutex> lock(mutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void ThreadPool::worker() {
    while (true) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(mutex);

            condition.wait(lock, [this] { return stopFlag || !tasks.empty(); });

            if (stopFlag && tasks.empty()) {
                return;
            }

            task = tasks.front();
            tasks.pop();
        }

        task();
    }
}
