#include <iostream>
#include <vector>

#include <boost/timer/timer.hpp>

typedef float FLOATING_POINT_TYPE;
const int arraySize = 10000000;
const FLOATING_POINT_TYPE value = 1.23456;

std::vector<FLOATING_POINT_TYPE> makeArray() {
    std::vector<FLOATING_POINT_TYPE> array(arraySize, value);
    return array;
}

void simpleSummation(const std::vector<FLOATING_POINT_TYPE>& array) {
    std::cout << "Simple summation: ";
    boost::timer::cpu_timer timer;
    FLOATING_POINT_TYPE summation = 0;
    for    (std::vector<FLOATING_POINT_TYPE>::const_iterator it = array.begin();
            it != array.end();
            ++it) {
        summation += (*it);
    }
    std::cout << summation;
    std::cout << ", time: " << timer.format() << std::endl;
}

void kahanSummation(const std::vector<FLOATING_POINT_TYPE>& array) {
    std::cout << "Kahan summation : ";
    boost::timer::cpu_timer timer;
    FLOATING_POINT_TYPE summation  = 0;
    FLOATING_POINT_TYPE compliment = 0;
    for    (std::vector<FLOATING_POINT_TYPE>::const_iterator it = array.begin();
            it != array.end();
            ++it) {
        const FLOATING_POINT_TYPE y = (*it) - compliment;
        const FLOATING_POINT_TYPE t = summation + y;
        compliment = (t - summation) - y;
        summation  = t;
    }
    std::cout << summation;
    std::cout << ", time: " << timer.format() << std::endl;
}

int main() {
    std::vector<FLOATING_POINT_TYPE> array = makeArray();
    std::cout << "Array size: " << arraySize << std::endl;
    std::cout << "Value     : " << value     << std::endl;
    simpleSummation(array);
    kahanSummation (array);
    return 0;
}
