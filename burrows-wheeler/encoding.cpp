#include "encoding.hpp"

#include <algorithm>
#include <iostream>
#include <vector>

std::vector<std::pair<std::string, int> > makeCirculantMatrix(const std::string& origin) {
    std::vector<std::pair<std::string, int> > circulantMatrix;
    const int length = origin.size();
    for(int i = 0; i < length; ++i) {
        const std::string line = origin.substr(i) + origin.substr(0, i);
        circulantMatrix.push_back(std::make_pair(line, i));
    }
    return circulantMatrix;
}

void printCirculantMatrix(const std::vector<std::pair<std::string, int> >& circulantMatrix) {
    const int length = circulantMatrix.size();
    for(int i = 0; i < length; ++i) {
        std::cout << circulantMatrix[i].second << " - " << circulantMatrix[i].first << std::endl;
    }
}

std::pair<std::string, int> makeBWS(const std::vector<std::pair<std::string, int> >& circulantMatrix) {
    std::pair<std::string, int> burrowsWheelerSeries;
    const int length = circulantMatrix.size();
    const int back = length - 1;
    for(int i = 0; i < length; ++i) {
        burrowsWheelerSeries.first += circulantMatrix[i].first.at(back);
        if(circulantMatrix[i].second == 0) {
            burrowsWheelerSeries.second = i;
        }
    }
    return burrowsWheelerSeries;
}

std::pair<std::string, int> encoding(const std::string& origin) {
    std::vector<std::pair<std::string, int> > circulantMatrix = makeCirculantMatrix(origin);
    std::cout << "Circulant matrix:" << std::endl;
    printCirculantMatrix(circulantMatrix);
    std::cout << std::endl;

    std::sort(circulantMatrix.begin(), circulantMatrix.end());
    std::cout << "Sorted circulant matrix:" << std::endl;
    printCirculantMatrix(circulantMatrix);
    std::cout << std::endl;

    std::pair<std::string, int> burrowsWheelerSeries = makeBWS(circulantMatrix);
    std::cout
            << "Burrows-Wheeler series: "
            << burrowsWheelerSeries.second
            << " - "
            << burrowsWheelerSeries.first
            << std::endl << std::endl;
    return burrowsWheelerSeries;
}
