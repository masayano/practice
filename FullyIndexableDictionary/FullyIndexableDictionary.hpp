#pragma once

#include <vector>

typedef unsigned int SIZE_T;
typedef unsigned int BIT_CONTAINER_TYPE;
typedef unsigned char SMALL_BLOCK_RECORD_TYPE;
typedef unsigned char TAIL_TABLE_RECORD_TYPE;

class FullyIndexableDictionary {
    const std::vector<BIT_CONTAINER_TYPE> bits;
    SIZE_T bitsLength;
    SIZE_T bigBlockSize;
    SIZE_T smallBlockSize;
    SIZE_T smallBlock_per_bigBlock;
    std::vector<SIZE_T> bigBlockArray;
    std::vector<SMALL_BLOCK_RECORD_TYPE> smallBlockArray;
    std::vector<TAIL_TABLE_RECORD_TYPE> tailTable;
public:
    FullyIndexableDictionary(const std::vector<BIT_CONTAINER_TYPE>& b);
    SIZE_T rank(const SIZE_T idx) const;
    SIZE_T select(const SIZE_T num) const;
};
