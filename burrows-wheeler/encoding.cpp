#include "encoding.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<std::pair<std::string, std::size_t> > makeCirculantMatrix(const std::string& origin) {
    std::vector<std::pair<std::string, std::size_t> > circulantMatrix;
    const auto length = origin.size();
    for(auto i = 0U; i < length; ++i) {
        const auto line = origin.substr(i) + origin.substr(0, i);
        circulantMatrix.push_back(std::make_pair(line, i));
    }
    return circulantMatrix;
}

void printCirculantMatrix(const std::vector<std::pair<std::string, std::size_t> >& circulantMatrix) {
    for(const auto& circulantPair : circulantMatrix) {
        std::cout << circulantPair.second << " - " << circulantPair.first << std::endl;
    }
}

std::pair<std::string, std::size_t> makeBWS(const std::vector<std::pair<std::string, std::size_t> >& circulantMatrix) {
    std::pair<std::string, std::size_t> burrowsWheelerSeries;
    const auto length = circulantMatrix.size();
    const auto back = length - 1;
    for(auto i = 0U; i < length; ++i) {
        burrowsWheelerSeries.first += circulantMatrix[i].first.at(back);
        if(circulantMatrix[i].second == 0) {
            burrowsWheelerSeries.second = i;
        }
    }
    return burrowsWheelerSeries;
}

std::pair<std::string, std::size_t> encoding(const std::string& origin) {
    auto circulantMatrix = makeCirculantMatrix(origin);
    std::cout << "Circulant matrix:" << std::endl;
    printCirculantMatrix(circulantMatrix);
    std::cout << std::endl;

    std::sort(circulantMatrix.begin(), circulantMatrix.end());
    std::cout << "Sorted circulant matrix:" << std::endl;
    printCirculantMatrix(circulantMatrix);
    std::cout << std::endl;

    const auto burrowsWheelerSeries = makeBWS(circulantMatrix);
    std::cout
            << "Burrows-Wheeler series: "
            << burrowsWheelerSeries.second
            << " - "
            << burrowsWheelerSeries.first
            << std::endl << std::endl;
    return burrowsWheelerSeries;
}
