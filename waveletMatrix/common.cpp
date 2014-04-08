#include "common.hpp"

#include <iostream>
#include <iomanip>

void printArray(const std::vector<VALUE>& array) {
    const auto length = array.size();
    for(auto i = 0U; i < length; ++i) {
        std::cout << std::setw(1) << std::setfill('0') << array[i];
        std::cout << " ";
    }
    std::cout << std::endl;
}
