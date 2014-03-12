#include "common.hpp"

#include <iostream>
#include <iomanip>

void printArray(const std::vector<int>& array) {
    for(int i = 0; i < array.size(); ++i) {
        std::cout << std::setw(1) << std::setfill('0') << array[i];
        std::cout << " ";
    }
    std::cout << std::endl;
}
