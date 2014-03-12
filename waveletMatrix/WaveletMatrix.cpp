#include "WaveletMatrix.hpp"

#include <iostream>

#include "common.hpp"

void alloc(
        const int bitsNum,
        const int length,
        std::vector<std::vector<int> >& bitMatrix,
        std::vector<int>& zeroNumberArray) {
    bitMatrix.resize(bitsNum);
    for(int i = 0; i < bitsNum; ++i) {
        bitMatrix[i].resize(length);
    }
    zeroNumberArray.resize(bitsNum);
}

void createMatrixAndArray(
        const std::vector<int>& array,
        const int bitsNum,
        const int length,
        std::vector<std::vector<int> >& bitMatrix,
        std::vector<int>& zeroNumberArray,
        std::vector<int>& tmpArray) {
    const int max = bitsNum - 1;
    for(int i = 0; i < bitsNum; ++i) {
        const int mask = (1 << (max - i));
        //std::cout << mask << std::endl;
        int count = 0;
        std::vector<int> tmpArray_zero;
        std::vector<int> tmpArray_one;
        for(int j = 0; j < length; ++j) {
            const int element = tmpArray[j];
            if((element & mask) == 0) {
                ++count;
                bitMatrix[i][j] = 0;
                tmpArray_zero.push_back(element);
            } else {
                bitMatrix[i][j] = 1;
                tmpArray_one .push_back(element);
            }
        }
        tmpArray_zero.insert(
                tmpArray_zero.end(),
                tmpArray_one.begin(),
                tmpArray_one.end());
        tmpArray = tmpArray_zero;
        zeroNumberArray[i] = count;
    }
}

void createStartIdxList(
        const std::vector<int>& array,
        const int length,
        std::map<int,int>& startIdxList) {
    int oldNum = array[0];
    int newNum;
    startIdxList.insert(std::make_pair(oldNum, 0));
    for(int i = 1; i < length; ++i) {
        newNum = array[i];
        if(oldNum != newNum) {
            startIdxList.insert(std::make_pair(newNum, i));
            oldNum = newNum;
        }
    }
}

void create(
        const std::vector<int>& array,
        const int bitsNum,
        const int length,
        std::vector<std::vector<int> >& bitMatrix,
        std::vector<int>& zeroNumberArray,
        std::map<int, int>& startIdxList) {
    std::vector<int> tmpArray = array;
    createMatrixAndArray(
            array,
            bitsNum,
            length,
            bitMatrix,
            zeroNumberArray,
            tmpArray);
    createStartIdxList(
            tmpArray,
            length,
            startIdxList);
}

WaveletMatrix::WaveletMatrix(const std::vector<int>& array)
        : bitsNum(sizeof(int) * 8), length(array.size()) {
    alloc(
            bitsNum,
            length,
            bitMatrix,
            zeroNumberArray);
    create(
            array,
            bitsNum,
            length,
            bitMatrix,
            zeroNumberArray,
            startIdxList);
}

int rank_linear(
        const std::vector<int>& vec,
        const int idx,
        const int bit) {
    //本当はここを完備辞書でやらないと意味が全くないが簡単のためサボる
    int count = 0;
    for(int i = 0; i < idx; ++i) {
        if(vec[i] == bit) {
            ++count;
        }
    }
    return count;
}

int WaveletMatrix::access(const int idx) const {
    int num = 0;
    int bit = 0;
    int tmpIdx = idx;
    for(int i = 0; i < bitsNum; ++i) {
        const std::vector<int>& vector = bitMatrix[i];
        bit = vector[tmpIdx];
        tmpIdx = rank_linear(vector, tmpIdx, bit);
        if(bit == 1) {
            tmpIdx += zeroNumberArray[i];
        }
        num << 1;
        num += bit;
    }
    return num;
}

int WaveletMatrix::rank(
        const int idx,
        const int val) const {
    const int max = bitsNum - 1;
    int tmpIdx = idx;
    for(int i = 0; i < bitsNum; ++i) {
        int bit;
        const int mask = (1 << (max - i));
        if((val & mask) == 0) {
            bit = 0;
        } else {
            bit = 1;
        }
        const std::vector<int>& vec = bitMatrix[i];
        tmpIdx = rank_linear(vec, tmpIdx, bit);
        if(bit == 1) {
            tmpIdx += zeroNumberArray[i];
        }
        //std::cout << tmpIdx << ", " << bit << std::endl;
    }
    const std::map<int,int>::const_iterator it = startIdxList.find(val);
    if(it != startIdxList.end()) {
        return (tmpIdx - (*it).second);
    } else {
        return -1;
    }
}

int select_linear(
        const std::vector<int>& vec,
        const int idx,
        const int bit) {
    //本当はここを完備辞書でやらないと意味が全くないが簡単のためサボる
    int count = 0;
    const int length = vec.size();
    for(int i = 0; i < length; ++i) {
        if(vec[i] == bit) {
            ++count;
        }
        if(idx == count) {
            return (i + 1);
        }
    }
    return -1;
}

int WaveletMatrix::select(const int val, const int num) const {
    const std::map<int,int>::const_iterator it = startIdxList.find(val);
    if((num == 0) || (it == startIdxList.end())) {
        return -1;
    } else {
        int idx = (*it).second + num;
        const int max = bitsNum - 1;
        for(int i = 0; i < bitsNum; ++i) {
            const int mask = (1 << i);
            int bit;
            const int col = max - i;
            if((val & mask) == 0) {
                bit = 0;
            } else {
                bit = 1;
                idx -= zeroNumberArray[col];
            }
            const std::vector<int>& vec = bitMatrix[col];
            if((idx > -1) && (idx < vec.size())) {
                idx = select_linear(vec, idx, bit);
                //std::cout << idx << std::endl;
            } else {
                return -1;
            }
        }
        return idx;
    }
}

void WaveletMatrix::print() const {
    for(int i = 0; i < bitsNum; ++i) {
        for(int j = 0; j < length; ++j) {
            std::cout << bitMatrix[i][j] << " ";
        }
        std::cout << ". zero num: " << zeroNumberArray[i] << std::endl;
    }
    std::cout << "Start idx list : " << std::endl;
    for(std::map<int,int>::const_iterator it = startIdxList.begin(); it != startIdxList.end(); ++it) {
        std::cout << (*it).first << ":" << (*it).second << std::endl;
    }
}
