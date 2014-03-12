#include <cstdlib>
#include <iostream>
#include <vector>

#include "WaveletMatrix.hpp"
#include "common.hpp"

const int arraySize = 20;
const int maxVal    = 10;

std::vector<int> createArray() {
    std::vector<int> array(arraySize);
    for(int i = 0; i < arraySize; ++i) {
        array[i] = std::rand() % maxVal;
    }
    return array;
}

int main() {
    const std::vector<int> array = createArray();
    std::cout << "origin: ";
    printArray(array);

    const WaveletMatrix waveletMatrix(array);
    waveletMatrix.print();

    std::cout << "rank(idx, val):" << std::endl;
    for(int i = 0; i < maxVal; ++i) {
        for(int j = 0; j < arraySize+1; ++j) {
            std::cout << "(" << j << ", " << i << "):" << waveletMatrix.rank(j, i) << ", ";
        }
        std::cout << std::endl;
    }

    std::cout << "select(val, num):" << std::endl;
    for(int i = 0; i < maxVal; ++i) {
        for(int j = 0; j < arraySize+1; ++j) {
            std::cout << "(" << i << ", " << j << "):" << waveletMatrix.select(i, j) << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
