#ifndef FULLY_INDEXABLE_DICTIONARY_HPP_
#define FULLY_INDEXABLE_DICTIONARY_HPP_

#include <vector>

typedef int BIT_CONTAINER_TYPE;
typedef char SMALL_BLOCK_RECORD_TYPE;
typedef char TAIL_TABLE_RECORD_TYPE;

class FullyIndexableDictionary {
    const std::vector<BIT_CONTAINER_TYPE> bits;
    int bitsLength;
    int bigBlockSize;
    int smallBlockSize;
    int smallBlock_per_bigBlock;
    std::vector<int> bigBlockArray;
    std::vector<SMALL_BLOCK_RECORD_TYPE> smallBlockArray;
    std::vector<TAIL_TABLE_RECORD_TYPE> tailTable;
public:
    FullyIndexableDictionary(const std::vector<BIT_CONTAINER_TYPE>& b);
    int rank(const int idx) const;
    int select(const int num) const;
};

#endif
