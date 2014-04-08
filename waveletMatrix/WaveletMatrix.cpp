#include "WaveletMatrix.hpp"

#include <iostream>

#include "common.hpp"

void alloc(
        const SIZE_T bitsNum,
        const SIZE_T length,
        std::vector<std::vector<SIZE_T> >& bitMatrix,
        std::vector<SIZE_T>& zeroNumberArray) {
    bitMatrix.resize(bitsNum);
    for(auto& bitLine : bitMatrix) {
        bitLine.resize(length);
    }
    zeroNumberArray.resize(bitsNum);
}

void createMatrixAndArray(
        const std::vector<VALUE>& array,
        const SIZE_T bitsNum,
        const SIZE_T length,
        std::vector<std::vector<SIZE_T> >& bitMatrix,
        std::vector<SIZE_T>& zeroNumberArray,
        std::vector<VALUE>& tmpArray) {
    const auto max = bitsNum - 1;
    for(auto i = 0U; i < bitsNum; ++i) {
        const auto mask = (1U << (max - i));
        //std::cout << mask << std::endl;
        auto count = 0U;
        std::vector<VALUE> tmpArray_zero;
        std::vector<VALUE> tmpArray_one;
        for(auto j = 0U; j < length; ++j) {
            const auto& element = tmpArray[j];
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
                end(tmpArray_zero),
                begin(tmpArray_one),
                end  (tmpArray_one));
        tmpArray = tmpArray_zero;
        zeroNumberArray[i] = count;
    }
}

void createStartIdxList(
        const std::vector<VALUE>& array,
        const SIZE_T length,
        std::map<SIZE_T, SIZE_T>& startIdxList) {
    auto oldNum = array[0];
    VALUE newNum;
    startIdxList.insert(std::make_pair(oldNum, 0));
    for(auto i = 1U; i < length; ++i) {
        newNum = array[i];
        if(oldNum != newNum) {
            startIdxList.insert(std::make_pair(newNum, i));
            oldNum = newNum;
        }
    }
}

void create(
        const std::vector<VALUE>& array,
        const SIZE_T bitsNum,
        const SIZE_T length,
        std::vector<std::vector<SIZE_T> >& bitMatrix,
        std::vector<SIZE_T>& zeroNumberArray,
        std::map<SIZE_T, SIZE_T>& startIdxList) {
    auto tmpArray = array;
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

WaveletMatrix::WaveletMatrix(const std::vector<VALUE>& array)
        : bitsNum(sizeof(SIZE_T) * 8), length(array.size()) {
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

SIZE_T rank_linear(
        const std::vector<SIZE_T>& vec,
        const SIZE_T idx,
        const SIZE_T bit) {
    //本当はここを完備辞書でやらないと意味が全くないが簡単のためサボる
    auto count = 0;
    for(auto i = 0U; i < idx; ++i) {
        if(vec[i] == bit) {
            ++count;
        }
    }
    return count;
}

SIZE_T WaveletMatrix::access(const SIZE_T idx) const {
    auto num = 0U;
    auto bit = 0U;
    auto tmpIdx = idx;
    for(auto i = 0U; i < bitsNum; ++i) {
        const auto& vector = bitMatrix[i];
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

SIZE_T WaveletMatrix::rank(
        const SIZE_T idx,
        const SIZE_T val) const {
    const SIZE_T max = bitsNum - 1;
    auto tmpIdx = idx;
    for(auto i = 0U; i < bitsNum; ++i) {
        SIZE_T bit;
        const auto mask = (1 << (max - i));
        if((val & mask) == 0) {
            bit = 0;
        } else {
            bit = 1;
        }
        const auto& vec = bitMatrix[i];
        tmpIdx = rank_linear(vec, tmpIdx, bit);
        if(bit == 1) {
            tmpIdx += zeroNumberArray[i];
        }
        //std::cout << tmpIdx << ", " << bit << std::endl;
    }
    const auto it = startIdxList.find(val);
    if(it != end(startIdxList)) {
        return (tmpIdx - (*it).second);
    } else {
        return 0;
    }
}

int select_linear(
        const std::vector<SIZE_T>& vec,
        const SIZE_T num,
        const SIZE_T bit) {
    //本当はここを完備辞書でやらないと意味が全くないが簡単のためサボる
    auto count = 0U;
    const auto length = vec.size();
    for(auto i = 0U; i < length; ++i) {
        if(vec[i] == bit) {
            ++count;
        }
        if(num == count) {
            return (i + 1);
        }
    }
    return -1;
}

int WaveletMatrix::select(const SIZE_T val, const SIZE_T n) const {
    const auto it = startIdxList.find(val);
    if((n == 0) || (it == end(startIdxList))) {
        return -1;
    } else {
        auto idx = (*it).second + n;
        const SIZE_T max = bitsNum - 1;
        for(auto i = 0U; i < bitsNum; ++i) {
            const auto mask = (1 << i);
            SIZE_T bit;
            int num = idx;
            const SIZE_T col = max - i;
            if((val & mask) == 0) {
                bit = 0;
            } else {
                bit = 1;
                num -= zeroNumberArray[col];
            }
            const auto& vec = bitMatrix[col];
            if((num > -1) && (num <= static_cast<int>(vec.size()))) {
                idx = select_linear(vec, num, bit);
                //std::cout << idx << std::endl;
            } else {
                return -1;
            }
        }
        return idx;
    }
}

void WaveletMatrix::print() const {
    for(auto i = 0U; i < bitsNum; ++i) {
        for(auto j = 0U; j < length; ++j) {
            std::cout << bitMatrix[i][j] << " ";
        }
        std::cout << ". zero num: " << zeroNumberArray[i] << std::endl;
    }
    std::cout << "Start idx list : " << std::endl;
    for(const auto& startIdx : startIdxList) {
        std::cout << startIdx.first << ":" << startIdx.second << std::endl;
    }
}
