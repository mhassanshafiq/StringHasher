#pragma once

#include <string>

class HashCalculator {
public:
    static std::string calculateSHA(const std::string& data);
    static std::string calculateSTL(const std::string& data);
};