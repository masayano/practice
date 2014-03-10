#include "decoding.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

void printDecodingVector(const std::vector<std::pair<char, int> >& decodingVector) {
    const int length = decodingVector.size();
    for(int i = 0; i < length; ++i) {
        std::cout
                << decodingVector[i].second
                << " - "
                << decodingVector[i].first
                << std::endl;
    }
    std::cout << std::endl;
}

void decoding(const std::pair<std::string, int>& burrowsWheelerSeries) {
    const std::string& bwstring = burrowsWheelerSeries.first;
    const int length = bwstring.size();
    std::vector<std::pair<char, int> > decodingVector;
    for(int i = 0; i < length; ++i) {
        decodingVector.push_back(std::make_pair(bwstring[i], i));
    }
    std::cout << "Decoding vector:" << std::endl;
    printDecodingVector(decodingVector);

    std::sort(decodingVector.begin(), decodingVector.end());
    std::cout << "Sorted decoding vector:" << std::endl;
    printDecodingVector(decodingVector);

    std::cout << "decoding:" << std::endl;
    int start = burrowsWheelerSeries.second;
    for(int i = 0; i < length; ++i) {
        std::cout << decodingVector[start].first << "(" << start << ")" << std::endl;
        start = decodingVector[start].second;
    }
}
