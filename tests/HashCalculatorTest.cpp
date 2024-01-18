#include "gtest/gtest.h"
#include "../src/HashCalculator.h"

TEST(HashCalculatorTest, CalculateSHA) {
    std::string data = "The brown fox";
    std::string expectedSHA = "a60d8a2186de318f7dc0e89e47a04d3d1e167ace0b49bbd11d47c988cbbead3d\n"; // SHA-256 hash of "The brown fox"

    std::string actualSHA = HashCalculator::calculateSHA(data);

    EXPECT_EQ(actualSHA, expectedSHA);
}

TEST(HashCalculatorTest, CalculateSTL) {
    std::string data = "The brown fox";
    std::string expectedSTL = "c1771f17571328b1\n"; 

    std::string actualSTL = HashCalculator::calculateSTL(data);

    EXPECT_EQ(actualSTL, expectedSTL);
}

TEST(HashCalculatorTest, CalculateSHAEmpty) {
    std::string data = "";
    std::string expectedSHA = "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855\n"; 

    std::string actualSHA = HashCalculator::calculateSHA(data);

    EXPECT_EQ(actualSHA, expectedSHA);
}

TEST(HashCalculatorTest, CalculateSTLEmpty) {
    std::string data = "";
    std::string expectedSTL = "553e93901e462a6e\n";

    std::string actualSTL = HashCalculator::calculateSTL(data);

    EXPECT_EQ(actualSTL, expectedSTL);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
