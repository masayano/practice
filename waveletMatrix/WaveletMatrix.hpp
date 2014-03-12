#ifndef WAVELET_MATRIX_HPP_
#define WAVELET_MATRIX_HPP_

#include <map>
#include <vector>

class WaveletMatrix {
    const int bitsNum;
    const int length;
    std::vector<std::vector<int> > bitMatrix;
    std::vector<int> zeroNumberArray;
    std::map<int, int> startIdxList;
public:
    WaveletMatrix(const std::vector<int>& array);
    int access(const int idx) const;
    int rank  (const int idx, const int val) const;
    int select(const int val, const int num) const;
    void print() const;
};

#endif
