#!/bin/bash

# Check if g++ is installed
if command -v g++ &> /dev/null; then
    echo "g++ is already installed."
else
    echo "g++ is not installed. Attempting to install..."
    sudo apt-get update
    sudo apt-get install -y g++
    if [ $? -ne 0 ]; then
        echo "Failed to install g++. Exiting."
        exit 1
    fi
    echo "g++ installed successfully."
fi

# Check if openssl is installed
if command -v openssl &> /dev/null; then
    echo "openssl is already installed."
else
    echo "openssl is not installed. Attempting to install..."
    sudo apt-get update
    sudo apt-get install -y openssl
    if [ $? -ne 0 ]; then
        echo "Failed to install openssl. Exiting."
        exit 1
    fi
    echo "openssl installed successfully."
fi

# Erase the 'out' directory if it exists
if [ -d "out" ]; then
    echo "Removing existing 'out' directory..."
    rm -r out
fi

# Recreate the 'out' directory
mkdir out

# Run the g++ command to build the solution
g++ -o out/HashServer src/main.cpp src/Server.cpp src/ClientHandler.cpp src/ThreadPool.cpp src/HashCalculator.cpp -pthread -lssl -lcrypto

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful. You can run the executable using: ./out/HashServer <port> <num_threads> <hash_type (SHA/STL)>"
else
    echo "Build failed."
fi
