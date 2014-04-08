#include <iostream>
#include <cstdlib>
#include <bitset>
#include <vector>

#include "FullyIndexableDictionary.hpp"

std::vector<BIT_CONTAINER_TYPE> makeBits(const SIZE_T length) {
    std::vector<BIT_CONTAINER_TYPE> bits(length);
    for(auto i = 0U; i < length; ++i) {
        bits[i] = std::rand();
    }
    return bits;
}

void printBits(const std::vector<BIT_CONTAINER_TYPE>& bits) {
    const auto length = bits.size();
    for(auto i = 0U; i < length; ++i) {
        const std::bitset<sizeof(BIT_CONTAINER_TYPE)*8> b(bits[i]);
        std::cout << "[" << i << "] " << b << " (" << static_cast<BIT_CONTAINER_TYPE>(bits[i]) << ")" << std::endl;
    }
}

const auto l = 4U;

int main() {
    const std::vector<BIT_CONTAINER_TYPE> bits = makeBits(l);
    const FullyIndexableDictionary fid(bits);
    std::cout << "bits: " << std::endl;
    printBits(bits);
    std::cout << std::endl;
    const SIZE_T max = l * sizeof(BIT_CONTAINER_TYPE) * 8 + 1;
    std::cout << "rank:" << std::endl;
    for(auto i = 0U; i < max; ++i) {
        std::cout << "[" << i << "]->" << fid.rank(i) << ", ";
        if((i != 0) && ((i % 8) == 0)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
    std::cout << "select:" << std::endl;
    for(auto i = 0U; i < max; ++i) {
        std::cout << "[" << i << "]->" << fid.select(i) << ", ";
        if((i != 0) && ((i % 8) == 0)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    return 0;
}
