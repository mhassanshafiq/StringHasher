#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include "HashCalculator.h"
#include <sstream>
#include <functional>
#include <cstring>
#include <openssl/sha.h>

std::string HashCalculator::calculateSHA(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.length());
    SHA256_Final(hash, &sha256);

    // Convert hash to hex string
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        char hex[3];
        snprintf(hex, sizeof(hex), "%02x", hash[i]);
        oss << hex;
    }
    oss << '\n';

    return oss.str();
}

std::string HashCalculator::calculateSTL(const std::string& data) {
    std::hash<std::string> hasher;
    size_t hashValue = hasher(data);

    // Convert hash to hex string
    std::ostringstream oss;
    oss << std::hex << hashValue;
    oss << '\n';

    return oss.str();
}