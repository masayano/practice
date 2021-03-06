#include <iostream>
#include <string>

#include "encoding.hpp"
#include "decoding.hpp"

int main() {
    const std::string origin = "cacao";
    std::cout << "Original string: " << origin << std::endl << std::endl;

    const auto burrowsWheelerSeries = encoding(origin);

    decoding(burrowsWheelerSeries);

    return 0;
}
