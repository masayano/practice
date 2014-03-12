#include <iostream>
#include <cstdlib>
#include <bitset>
#include <vector>

#include "FullyIndexableDictionary.hpp"

const int l = 16;

std::vector<BIT_CONTAINER_TYPE> makeBits(const int length) {
    std::vector<BIT_CONTAINER_TYPE> bits(length);
    for(int i = 0; i < length; ++i) {
        bits[i] = std::rand();
    }
    return bits;
}

void printBits(const std::vector<BIT_CONTAINER_TYPE>& bits) {
    const int length = bits.size();
    for(int i = 0; i < length; ++i) {
        const std::bitset<sizeof(BIT_CONTAINER_TYPE)*8> b(bits[i]);
        std::cout << "[" << i << "] " << b << " (" << static_cast<int>(bits[i]) << ")" << std::endl;
    }
}

int main() {
    const std::vector<BIT_CONTAINER_TYPE> bits = makeBits(l);
    const FullyIndexableDictionary fid(bits);
    std::cout << "bits: " << std::endl;
    printBits(bits);
    std::cout << std::endl;
    const int max = l * sizeof(BIT_CONTAINER_TYPE) * 8;
    for(int i = 0; i < max; ++i) {
        std::cout << "rank(" << i << "): " << fid.rank(i) << ", ";
        if((i != 0) && ((i % 8) == 0)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl << std::endl;
    for(int i = 0; i < max; ++i) {
        std::cout << "select(" << i << "): " << fid.select(i) << ", ";
        if((i != 0) && ((i % 8) == 0)) {
            std::cout << std::endl;
        }
    }
    std::cout << std::endl;
    return 0;
}
