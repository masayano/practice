#include "FullyIndexableDictionary.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>

template <class T> void recordBlock(
        const int count,
        std::vector<T>& blockArray,
        int& blockIdx) {
    blockArray[blockIdx] = count;
    ++blockIdx;
}

void printBlockArray(
        const std::vector<int>& bigBlockArray,
        const std::vector<SMALL_BLOCK_RECORD_TYPE>& smallBlockArray) {
    const int bigLength = bigBlockArray.size();
    std::cout << "Big block array: ";
    for(int i = 0; i < bigLength; ++i) {
        std::cout << bigBlockArray[i] << " ";
    }
    std::cout << std::endl;
    const int smallLength = smallBlockArray.size();
    std::cout << "Small block array: ";
    for(int i = 0; i < smallLength; ++i) {
        std::cout << static_cast<int>(smallBlockArray[i]) << " ";
    }
    std::cout << std::endl;
}

void scanning(
        const std::vector<BIT_CONTAINER_TYPE>& bits,
        std::vector<int>& bigBlockArray,
        std::vector<SMALL_BLOCK_RECORD_TYPE>& smallBlockArray,
        const int intBitsLength,
        const int vectorSize,
        const int bigBlockSize,
        const int smallBlockSize) {
    int global_idx    = 0;
    int local_idx     = 0;
    int global_count  = 0;
    int local_count   = 0;
    int bigBlockIdx   = 0;
    int smallBlockIdx = 0;
    recordBlock(global_count, bigBlockArray,   bigBlockIdx);
    recordBlock(local_count,  smallBlockArray, smallBlockIdx);
    const int maxShift = intBitsLength - 1;
    for(int i = 0; i < vectorSize; ++i) {
        for(int j = 0; j < intBitsLength; ++j) {
            //std::cout << global_idx << ":" << bigBlockArray[bigBlockIdx-1] << ":" << smallBlockArray[smallBlockIdx-1] << std::endl;
            if(bits[i] & (1 << (maxShift - j))) {
                ++global_count;
                ++local_count;
            }
            ++global_idx;
            ++local_idx;
            if((local_idx % smallBlockSize) == 0) {
                recordBlock(local_count, smallBlockArray, smallBlockIdx);
            }
            if(local_idx == bigBlockSize) {
                recordBlock(global_count, bigBlockArray, bigBlockIdx);
                local_idx   = 0;
                local_count = 0;
                recordBlock(local_count, smallBlockArray, smallBlockIdx);
            }
        }
    }
}

void makeTailTable(
        const int smallBlockSize,
        std::vector<TAIL_TABLE_RECORD_TYPE>& tailTable) {
    const int tableSize = (1 << smallBlockSize);
    tailTable.resize(tableSize);
    for(int i = 0; i < tableSize; ++i) {
        int count = 0;
        for(int j = 0; j < smallBlockSize; ++j) {
            if((i & (1 << j)) != 0) {
              ++count;
            }
        }
        tailTable[i] = count;
    }
}

void printTailTable(
        const int smallBlockSize,
        const std::vector<TAIL_TABLE_RECORD_TYPE>& tailTable) {
    const int tableSize = tailTable.size();
    std::cout << "Small block table: " << std::endl;
    for(int i = 0; i < tableSize; ++i) {
        std::cout << "[" << std::setw(2) << std::setfill('0') << i << "] ";
        for(int j = (smallBlockSize - 1); j >= 0; --j) {
            if((i & (1 << j)) != 0) {
                std::cout << 1;
            } else {
                std::cout << 0;
            }
        }
        std::cout << " : " << static_cast<int>(tailTable[i]) << std::endl;
    }
}

void blockIdx(
        const int idx,
        const int bigBlockSize,
        const int smallBlockSize,
        const int smallBlock_per_bigBlock,
        int& bigBlockIdx,
        int& smallBlockIdx) {
    //
    //       0                18                36                54                72           85
    //       0   4   8   12  16   22  26  30  34    40  44  48  52    58  62  66  70    76  80  84
    //       |------------------------------------------------------------------------------------|
    // big   o-----------------o-----------------o-----------------o-----------------o
    //      [0]               [1]               [2]               [3]               [4]
    // small o---o---o---o---o o---o---o---o---o o---o---o---o---o o---o---o---o---o o---o---o---o
    //      [0] [1] [2] [3] [4]                [10][11][12][13][14]                [20][21][22][23]
    //                        [5] [6] [7] [8] [9]                [15][16][17][18][19]
    //
              bigBlockIdx                = static_cast<int>(idx / bigBlockSize);
    const int smallBlock_in_bigBlock_num = bigBlockIdx * smallBlock_per_bigBlock;
    const int lastSmallBlockNum          = static_cast<int>((idx - (bigBlockSize * bigBlockIdx)) / smallBlockSize);
              smallBlockIdx              = smallBlock_in_bigBlock_num + lastSmallBlockNum;
}

FullyIndexableDictionary::FullyIndexableDictionary(const std::vector<BIT_CONTAINER_TYPE>& b) : bits(b) {
    const int intBitsLength  = sizeof(BIT_CONTAINER_TYPE) * 8;
    const int vectorSize     = bits.size();
    bitsLength               = intBitsLength * vectorSize;
    std::cout << "bitLength     : " << bitsLength     << std::endl;
    const int log2BitsLength = static_cast<int>(std::log(bitsLength) / std::log(2));
    bigBlockSize             = std::pow(log2BitsLength, 2);
    std::cout << "bigBlockSize  : " << bigBlockSize   << std::endl;
    smallBlockSize           = log2BitsLength / 2;
    std::cout << "smallBlockSize: " << smallBlockSize << std::endl;
    smallBlock_per_bigBlock  = 1 + static_cast<int>(bigBlockSize / smallBlockSize);
    std::cout << "Small / Big   : " << smallBlock_per_bigBlock << std::endl;
    int bigBlockIdx;
    int smallBlockIdx;
    blockIdx(
        bitsLength,
        bigBlockSize,
        smallBlockSize,
        smallBlock_per_bigBlock,
        bigBlockIdx,
        smallBlockIdx);
    const int bigBlockNum    = bigBlockIdx + 1;
    const int smallBlockNum  = smallBlockIdx + 1;
    bigBlockArray  .resize(bigBlockNum);
    std::cout << "bigBlockNum   : " << bigBlockNum    << std::endl;
    smallBlockArray.resize(smallBlockNum);
    std::cout << "smallBlockNum : " << smallBlockNum  << std::endl;
    scanning(
            bits,
            bigBlockArray,
            smallBlockArray,
            intBitsLength,
            vectorSize,
            bigBlockSize,
            smallBlockSize);
    printBlockArray(
            bigBlockArray,
            smallBlockArray);
    makeTailTable(
            smallBlockSize,
            tailTable);
    printTailTable(
            smallBlockSize,
            tailTable);
}

int getTail(
        const std::vector<BIT_CONTAINER_TYPE>& bits,
        const int begin,
        const int end) {
    const int containerSize = sizeof(BIT_CONTAINER_TYPE) * 8;
    const int maxBitIdx     = containerSize - 1;
    const int beginContainerIdx = begin / containerSize;
    const int beginBitIdx       = begin - beginContainerIdx * containerSize;
    const int endContainerIdx   = end   / containerSize;
    const int endBitIdx         = end - endContainerIdx * containerSize;
    BIT_CONTAINER_TYPE tmp  = bits[beginContainerIdx];
    for(int i = 0; i < beginBitIdx; ++i) {
        tmp <<= 1;
    }
    const BIT_CONTAINER_TYPE mask = (1 << maxBitIdx);
    int tail = 0;
    for(
            int containerIdx = beginContainerIdx, bitIdx = beginBitIdx;
            (containerIdx != endContainerIdx) || (bitIdx != endBitIdx);
            ) {
        tail <<= 1;
        if(((mask & tmp) >> maxBitIdx) == 0) {
            tail += 0;
        } else {
            tail += 1;
        }
        ++bitIdx;
        tmp <<= 1;
        if(bitIdx == containerSize) {
            bitIdx = 0;
            ++containerIdx;
            tmp = bits[containerIdx];
        }
        //std::cout << static_cast<int>(tmp) << std::endl;
        //std::cout << tail << std::endl;
    }
    return tail;
}

int FullyIndexableDictionary::rank(const int idx) const {
    if(idx > bitsLength) {
        return -1;
    } else {
        int bigBlockIdx;
        int smallBlockIdx;
        blockIdx(
                idx,
                bigBlockSize,
                smallBlockSize,
                smallBlock_per_bigBlock,
                bigBlockIdx,
                smallBlockIdx);
        const int lastBlockIdx = (bigBlockIdx * bigBlockSize) + ((smallBlockIdx - (bigBlockIdx * smallBlock_per_bigBlock)) * smallBlockSize);
        int out = bigBlockArray[bigBlockIdx] + smallBlockArray[smallBlockIdx];
        if(lastBlockIdx == idx) {
            return out;
        } else {
            const int tail = getTail(
                    bits,
                    lastBlockIdx,
                    idx);
            return (out + tailTable[tail]);
        }
    }
}

int FullyIndexableDictionary::select(const int num) const {
    int begin = 0;
    int end   = bigBlockArray.size();
    //std::cout << "Selecting big block." << std::endl;
    while((end - begin) > 1) {
        const int mid = (begin + end) / 2;
        const int val = bigBlockArray[mid];
        if(val > num) {
            end = mid;
        } else if(val == num) {
            int idx = bigBlockSize * mid;
            while(rank(idx) == num) {
                --idx;
            }
            return idx+1;
        } else {
            begin = mid;
        }
        //std::cout << begin << "~" << end << std::endl;
    }
    const int bigBlockIdx = begin;
    int newNum = num - bigBlockArray[begin];
    begin *= smallBlock_per_bigBlock;
    end   *= smallBlock_per_bigBlock;
    const int smallBlockNum = smallBlockArray.size();
    if(end > smallBlockNum) {
        end = smallBlockNum;
    }
    const int smallBlockBaseIdx = begin;
    //std::cout << "Selecting small block." << std::endl;
    while((end - begin) > 1) {
        const int mid = (begin + end) / 2;
        const int val = smallBlockArray[mid];
        if(val > newNum) {
            end = mid;
        } else if(val == newNum) {
            int idx = bigBlockSize * bigBlockIdx + (mid - smallBlockBaseIdx) * smallBlockSize;
            while(rank(idx) == num) {
                --idx;
            }
            return idx+1;
        } else {
            begin = mid;
        }
        //std::cout << begin << "~" << end << std::endl;
    }
    newNum -= smallBlockArray[begin];
    int idx1 = bigBlockSize * bigBlockIdx + (begin - smallBlockBaseIdx) * smallBlockSize;
    int idx2 = idx1 + 1;
    while(tailTable[getTail(bits, idx1, idx2)] != newNum) {
        ++idx2;
        if(idx2 == bitsLength) {
            return -1;
        }
    }
    return idx2;
}
