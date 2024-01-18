#include "Server.h"
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <functional>

std::function<void(int)> shutdown_handler;
void signal_handler(int signal) { shutdown_handler(signal); }

int main(int argc, char* argv[]) {
    int port = 1234;
    int numThreads = std::thread::hardware_concurrency();
    std::string hashType("SHA");

    if (argc > 1)
    {
        port = std::atoi(argv[1]);
    }
    if (argc > 2)
    {
        numThreads = std::atoi(argv[2]);
    }
    if (argc > 3) {
        hashType= argv[3];
    }

    if (hashType != "SHA" && hashType != "STL") {
        std::cerr << "Invalid hash type. Use either SHA or STL." << std::endl;
        return 1;
    }
    Server server(port, numThreads, hashType);

    //shutdown handler
    shutdown_handler = [&](int signum) {
        std::cout << "Received signal("<<signum<<").Closing server..." << std::endl;
        server.stop();
    };
    std::signal(SIGINT, signal_handler);
    std::signal(SIGTSTP, signal_handler);

    server.start();

    return 0;
}
