#include "FullyIndexableDictionary.hpp"

#include <cmath>
#include <iostream>
#include <iomanip>

template <class T> void recordBlock(
        const SIZE_T count,
        std::vector<T>& blockArray,
        SIZE_T& blockIdx) {
    blockArray[blockIdx] = count;
    ++blockIdx;
}

void printBlockArray(
        const std::vector<SIZE_T>& bigBlockArray,
        const std::vector<SMALL_BLOCK_RECORD_TYPE>& smallBlockArray) {
    std::cout << "Big block array: ";
    for(const auto& bigBlock : bigBlockArray) {
        std::cout << bigBlock << " ";
    }
    std::cout << std::endl;
    std::cout << "Small block array: ";
    for(const auto& smallBlock : smallBlockArray) {
        std::cout << static_cast<std::size_t>(smallBlock) << " ";
    }
    std::cout << std::endl;
}

void scanning(
        const std::vector<BIT_CONTAINER_TYPE>& bits,
        std::vector<SIZE_T>& bigBlockArray,
        std::vector<SMALL_BLOCK_RECORD_TYPE>& smallBlockArray,
        const SIZE_T intBitsLength,
        const SIZE_T vectorSize,
        const SIZE_T bigBlockSize,
        const SIZE_T smallBlockSize) {
    auto global_idx    = 0U;
    auto local_idx     = 0U;
    auto global_count  = 0U;
    auto local_count   = 0U;
    auto bigBlockIdx   = 0U;
    auto smallBlockIdx = 0U;
    recordBlock(global_count, bigBlockArray,   bigBlockIdx);
    recordBlock(local_count,  smallBlockArray, smallBlockIdx);
    const auto maxShift = intBitsLength - 1;
    for(const auto& bit : bits) {
        for(auto i = 0U; i < intBitsLength; ++i) {
            //std::cout << global_idx << ":" << bigBlockArray[bigBlockIdx-1] << ":" << smallBlockArray[smallBlockIdx-1] << std::endl;
            if(bit & (1 << (maxShift - i))) {
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
        const SIZE_T smallBlockSize,
        std::vector<TAIL_TABLE_RECORD_TYPE>& tailTable) {
    const auto tableSize = (1U << smallBlockSize);
    tailTable.resize(tableSize);
    for(auto i = 0U; i < tableSize; ++i) {
        TAIL_TABLE_RECORD_TYPE count = 0;
        for(auto j = 0U; j < smallBlockSize; ++j) {
            if((i & (1 << j)) != 0) {
              ++count;
            }
        }
        tailTable[i] = count;
    }
}

void printTailTable(
        const SIZE_T smallBlockSize,
        const std::vector<TAIL_TABLE_RECORD_TYPE>& tailTable) {
    const SIZE_T tableSize = tailTable.size();
    std::cout << "Small block table: " << std::endl;
    for(auto i = 0U; i < tableSize; ++i) {
        std::cout << "[" << std::setw(2) << std::setfill('0') << i << "] ";
        for(int j = (smallBlockSize - 1); j >= 0; --j) {
            if((i & (1 << j)) != 0) {
                std::cout << 1;
            } else {
                std::cout << 0;
            }
        }
        std::cout << " : " << static_cast<SIZE_T>(tailTable[i]) << std::endl;
    }
}

void blockIdx(
        const SIZE_T idx,
        const SIZE_T bigBlockSize,
        const SIZE_T smallBlockSize,
        const SIZE_T smallBlock_per_bigBlock,
        SIZE_T& bigBlockIdx,
        SIZE_T& smallBlockIdx) {
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
               bigBlockIdx                = static_cast<SIZE_T>(idx / bigBlockSize);
    const auto smallBlock_in_bigBlock_num = bigBlockIdx * smallBlock_per_bigBlock;
    const auto lastSmallBlockNum          = static_cast<SIZE_T>((idx - (bigBlockSize * bigBlockIdx)) / smallBlockSize);
               smallBlockIdx              = smallBlock_in_bigBlock_num + lastSmallBlockNum;
}

FullyIndexableDictionary::FullyIndexableDictionary(const std::vector<BIT_CONTAINER_TYPE>& b) : bits(b) {
    const SIZE_T intBitsLength  = sizeof(BIT_CONTAINER_TYPE) * 8;
    const auto vectorSize       = bits.size();
    bitsLength                  = intBitsLength * vectorSize;
    std::cout << "bitLength     : " << bitsLength     << std::endl;
    const auto log2BitsLength   = static_cast<SIZE_T>(std::log(bitsLength) / std::log(2));
    bigBlockSize                = std::pow(log2BitsLength, 2);
    std::cout << "bigBlockSize  : " << bigBlockSize   << std::endl;
    smallBlockSize              = log2BitsLength / 2;
    std::cout << "smallBlockSize: " << smallBlockSize << std::endl;
    smallBlock_per_bigBlock     = 1 + static_cast<SIZE_T>(bigBlockSize / smallBlockSize);
    std::cout << "Small / Big   : " << smallBlock_per_bigBlock << std::endl;
    SIZE_T bigBlockIdx;
    SIZE_T smallBlockIdx;
    blockIdx(
        bitsLength,
        bigBlockSize,
        smallBlockSize,
        smallBlock_per_bigBlock,
        bigBlockIdx,
        smallBlockIdx);
    const auto bigBlockNum    = bigBlockIdx + 1;
    const auto smallBlockNum  = smallBlockIdx + 1;
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

SIZE_T getTail(
        const std::vector<BIT_CONTAINER_TYPE>& bits,
        const SIZE_T begin,
        const SIZE_T end) {
    const SIZE_T containerSize = sizeof(BIT_CONTAINER_TYPE) * 8;
    const auto maxBitIdx     = containerSize - 1;
    const auto beginContainerIdx = begin / containerSize;
    const auto beginBitIdx       = begin - beginContainerIdx * containerSize;
    const auto endContainerIdx   = end   / containerSize;
    const auto endBitIdx         = end - endContainerIdx * containerSize;
    auto tmp = bits[beginContainerIdx];
    tmp <<= beginBitIdx;
    const BIT_CONTAINER_TYPE mask = (1U << maxBitIdx);
    auto tail = 0U;
    for(
            SIZE_T containerIdx = beginContainerIdx, bitIdx = beginBitIdx;
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

SIZE_T FullyIndexableDictionary::rank(const SIZE_T idx) const {
    if(idx > bitsLength) {
        return -1;
    } else {
        SIZE_T bigBlockIdx;
        SIZE_T smallBlockIdx;
        blockIdx(
                idx,
                bigBlockSize,
                smallBlockSize,
                smallBlock_per_bigBlock,
                bigBlockIdx,
                smallBlockIdx);
        const auto lastBlockIdx = (bigBlockIdx * bigBlockSize) + ((smallBlockIdx - (bigBlockIdx * smallBlock_per_bigBlock)) * smallBlockSize);
        SIZE_T out = bigBlockArray[bigBlockIdx] + smallBlockArray[smallBlockIdx];
        if(lastBlockIdx == idx) {
            return out;
        } else {
            const auto tail = getTail(
                    bits,
                    lastBlockIdx,
                    idx);
            return (out + tailTable[tail]);
        }
    }
}

SIZE_T FullyIndexableDictionary::select(const SIZE_T num) const {
    auto begin = 0U;
    auto end   = bigBlockArray.size();
    //std::cout << "Selecting big block." << std::endl;
    while((end - begin) > 1) {
        const auto mid = (begin + end) / 2;
        const auto val = bigBlockArray[mid];
        if(val > num) {
            end = mid;
        } else if(val == num) {
            auto idx = bigBlockSize * mid;
            while(rank(idx) == num) {
                --idx;
            }
            return idx+1;
        } else {
            begin = mid;
        }
        //std::cout << begin << "~" << end << std::endl;
    }
    const auto bigBlockIdx = begin;
    auto newNum = num - bigBlockArray[begin];
    begin *= smallBlock_per_bigBlock;
    end   *= smallBlock_per_bigBlock;
    const auto smallBlockNum = smallBlockArray.size();
    if(end > smallBlockNum) {
        end = smallBlockNum;
    }
    const auto smallBlockBaseIdx = begin;
    //std::cout << "Selecting small block." << std::endl;
    while((end - begin) > 1) {
        const auto mid = (begin + end) / 2U;
        const auto val = smallBlockArray[mid];
        if(val > newNum) {
            end = mid;
        } else if(val == newNum) {
            auto idx = bigBlockSize * bigBlockIdx + (mid - smallBlockBaseIdx) * smallBlockSize;
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
    auto idx1 = bigBlockSize * bigBlockIdx + (begin - smallBlockBaseIdx) * smallBlockSize;
    auto idx2 = idx1;
    do {
        //std::cout << idx1 << "," << idx2 << std::endl;
        if(idx2 == bitsLength) {
            return -1;
        }
        ++idx2;
    } while(tailTable[getTail(bits, idx1, idx2)] != newNum);
    return idx2;
}
