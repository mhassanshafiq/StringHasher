#!/bin/bash

# Check if gtest is installed
if [ -f "/usr/include/gtest/gtest.h" ]; then
    echo "Google Test is installed."
else
    echo "Google Test is not installed. Installing..."
    sudo apt-get update
    sudo apt-get install -y libgtest-dev
    sudo apt-get install -y cmake # required for building gtest
    cd /usr/src/gtest
    sudo cmake CMakeLists.txt
    sudo make
    sudo cp *.a /usr/lib
    sudo mkdir /usr/include/gtest
    sudo cp -r include/gtest/* /usr/include/gtest
fi

# Check if gmock is installed
if [ ! -d "/usr/include/gmock" ]; then
    echo "Google Mock not found. Installing..."

    # Install gmock
    sudo apt-get update
    sudo apt-get install -y libgmock-dev

    echo "Google Mock installed successfully."
else
    echo "Google Mock is already installed."
fi

# Erase the 'out' directory if it exists
if [ -d "out" ]; then
    echo "Removing existing 'out' directory..."
    rm -r out
fi

# Recreate the 'out' directory
mkdir out

# Compile and build the tests
g++ -o out/HashCalculatorTest HashCalculatorTest.cpp ../src/HashCalculator.cpp -lgtest -lgtest_main -pthread -lssl -lcrypto
g++ -o out/ThreadPoolTest ThreadPoolTest.cpp ../src/ThreadPool.cpp -lgtest -lgtest_main -pthread
g++ -o out/ClientHandlerTest ClientHandlerTest.cpp ../src/ClientHandler.cpp ../src/HashCalculator.cpp -lgtest -lgtest_main -lgmock -lgmock_main -pthread -lssl -lcrypto

echo "Tests have been built successfully."

# Create run_tests.sh script
echo "#!/bin/bash" > run_tests.sh
echo "" >> run_tests.sh
echo "# Run the tests" >> run_tests.sh
echo "./out/HashCalculatorTest" >> run_tests.sh
echo "./out/ThreadPoolTest" >> run_tests.sh
echo "./out/ClientHandlerTest" >> run_tests.sh
# ... Repeat for other test executables ...

# Make run_tests.sh executable
chmod +x run_tests.sh
