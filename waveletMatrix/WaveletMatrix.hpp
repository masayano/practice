#pragma once

#include <map>
#include <vector>

#include "common.hpp"

class WaveletMatrix {
    const SIZE_T bitsNum;
    const SIZE_T length;
    std::vector<std::vector<SIZE_T> > bitMatrix;
    std::vector<SIZE_T> zeroNumberArray;
    std::map<SIZE_T, SIZE_T> startIdxList;
public:
    WaveletMatrix(const std::vector<VALUE>& array);
    SIZE_T access(const SIZE_T idx) const;
    SIZE_T rank  (const SIZE_T idx, const SIZE_T val) const;
    int select(const SIZE_T val, const SIZE_T n) const;
    void print() const;
};
