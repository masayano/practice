#include "decoding.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

void printDecodingVector(const std::vector<std::pair<char, std::size_t> >& decodingVector) {
    for(const auto& decodingPair : decodingVector) {
        std::cout
                << decodingPair.second
                << " - "
                << decodingPair.first
                << std::endl;
    }
    std::cout << std::endl;
}

void decoding(const std::pair<std::string, std::size_t>& burrowsWheelerSeries) {
    const auto& bwstring = burrowsWheelerSeries.first;
    const auto length = bwstring.size();
    std::vector<std::pair<char, std::size_t> > decodingVector;
    for(auto i = 0U; i < length; ++i) {
        decodingVector.push_back(std::make_pair(bwstring[i], i));
    }
    std::cout << "Decoding vector:" << std::endl;
    printDecodingVector(decodingVector);

    std::sort(decodingVector.begin(), decodingVector.end());
    std::cout << "Sorted decoding vector:" << std::endl;
    printDecodingVector(decodingVector);

    std::cout << "decoding:" << std::endl;
    auto start = burrowsWheelerSeries.second;
    for(auto i = 0U; i < length; ++i) {
        std::cout << decodingVector[start].first << "(" << start << ")" << std::endl;
        start = decodingVector[start].second;
    }
}
