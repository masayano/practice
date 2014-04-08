#include <cstdlib>
#include <iostream>
#include <vector>

#include "WaveletMatrix.hpp"
#include "common.hpp"

const SIZE_T arraySize = 20;
const SIZE_T maxVal    = 10;

std::vector<VALUE> createArray() {
    std::vector<VALUE> array(arraySize);
    for(auto i = 0U; i < arraySize; ++i) {
        array[i] = std::rand() % maxVal;
    }
    return array;
}

int main() {
    const std::vector<VALUE> array = createArray();
    std::cout << "origin: ";
    printArray(array);

    const WaveletMatrix waveletMatrix(array);
    waveletMatrix.print();

    std::cout << "rank(idx, val):" << std::endl;
    for(auto i = 0U; i < maxVal; ++i) {
        for(auto j = 0U; j < arraySize+1; ++j) {
            std::cout << "(" << j << ", " << i << "):" << waveletMatrix.rank(j, i) << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "select(val, num):" << std::endl;
    for(auto i = 0U; i < maxVal; ++i) {
        for(auto j = 0U; j < arraySize+1; ++j) {
            std::cout << "(" << i << ", " << j << "):" << waveletMatrix.select(i, j) << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
