#include "../headers/BItSequence.h"
#include "../headers/DynamicArray.h"
#include "../headers/Sequence.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <stdexcept>

void BitSequence::ensureReductionCapacity() {
    size_t currentCap = bytes->GetSize();
    if (bitSize / 8 > currentCap) return;
    size_t newCap = std::max(static_cast<size_t>(1), static_cast<size_t>(currentCap) - 1);
    bytes->Resize(newCap);
}

void BitSequence::ensureIncreaseCapacity() {
    size_t currentCap = bytes->GetSize();
    if (bitSize / 8 < currentCap) return;
    size_t newCap = currentCap + 1;
    bytes->Resize(newCap);
}

uint8_t BitSequence::getByte(size_t byteIdx) const {
    return bytes->Get(byteIdx);
}

void BitSequence::setByte(size_t byteIdx, uint8_t val) {
    bytes->Set(byteIdx, val);
}

bool BitSequence::getBit(size_t bitIdx) const {
    if (bitIdx >= bitSize)
        throw std::out_of_range("BitSequence: bit index out of range");
    size_t byteIdx = bitIdx / 8;
    size_t bitPos = bitIdx % 8;
    return (getByte(byteIdx) >> bitPos) & 1;
}

BitSequence *BitSequence::setBit(size_t bitIdx, bool value) {
    if (bitIdx >= bitSize)
        throw std::out_of_range("BitSequence: bit index out of range");

    size_t byteIdx = bitIdx / 8;
    size_t bitPos = bitIdx % 8;
    uint8_t byteVal = bytes->Get(byteIdx);

    if (value) {
        byteVal |= static_cast<uint8_t>(1u << bitPos);
    } else {
        byteVal &= static_cast<uint8_t>(~(1u << bitPos));
    }

    bytes->Set(byteIdx, byteVal);
    return this;
}

BitSequence::BitSequence(DynamicArray<uint8_t> *b, size_t bits)
    : bytes(b), bitSize(bits) {
}

BitSequence::BitSequence(const bool *bits, size_t numBits) : bitSize(numBits) {
    if (!bits && numBits > 0)
        throw std::invalid_argument("BitSequence: bits is nullptr");

    size_t byteCount = (numBits + 7) / 8;
    bytes = new DynamicArray<uint8_t>(byteCount);

    for (size_t i = 0; i < numBits; ++i) {
        if (bits[i]) {
            size_t bIdx = i / 8;
            size_t bPos = i % 8;
            uint8_t val = bytes->Get(bIdx);
            val |= static_cast<uint8_t>(1u << bPos);
            bytes->Set(bIdx, val);
        }
    }
}

BitSequence::BitSequence(const char *bitStr) {
    if (!bitStr)
        throw std::invalid_argument("BitSequence: bitStr is nullptr");

    bitSize = std::strlen(bitStr);
    for (size_t i = 0; i < bitSize; ++i) {
        if (bitStr[i] != '0' && bitStr[i] != '1')
            throw std::invalid_argument("BitSequence: invalid character");
    }

    size_t byteCount = (bitSize + 7) / 8;
    bytes = new DynamicArray<uint8_t>(byteCount);

    for (size_t i = 0; i < bitSize; ++i) {
        if (bitStr[i] == '1') {
            size_t bIdx = i / 8;
            size_t bPos = i % 8;
            uint8_t val = bytes->Get(bIdx);
            val |= static_cast<uint8_t>(1u << bPos);
            bytes->Set(bIdx, val);
        }
    }
}

size_t BitSequence::GetLength() const {
    return bitSize;
}

bool BitSequence::GetFirst() const {
    if (bitSize == 0) throw std::out_of_range("BitSequence: empty");
    return getBit(0);
}

bool BitSequence::GetLast() const {
    if (bitSize == 0) throw std::out_of_range("BitSequence: empty");
    return getBit(bitSize - 1);
}

BitSequence *BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (endIndex > bitSize)
        throw std::out_of_range("BitSequence: endIndex out of bounds");

    size_t newLen = endIndex - startIndex;
    if (newLen == 0)
        return new BitSequence(new DynamicArray<uint8_t>(0), 0);

    auto tmp = new bool[newLen];
    for (size_t i = 0; i < newLen; ++i)
        tmp[i] = getBit(startIndex + i);

    auto *res = new BitSequence(tmp, newLen);
    delete[] tmp;
    return res;
}

IEnumerator<bool> *BitSequence::GetEnumerator() const {
    return new BitEnumerator(this);
}

BitSequence *BitSequence::appendImpl(const bool &elem) {
    ensureIncreaseCapacity();

    size_t byteIdx = bitSize / 8;
    size_t bitPos = bitSize % 8;

    uint8_t byteVal = bytes->Get(byteIdx);
    if (elem) {
        byteVal |= static_cast<uint8_t>(1u << bitPos);
    } else {
        byteVal &= static_cast<uint8_t>(~(1u << bitPos));
    }
    bytes->Set(byteIdx, byteVal);

    ++bitSize;
    return this;
}

BitSequence *BitSequence::prependImpl(const bool &elem) {
    if (bitSize == 0) {
        return appendImpl(elem);
    }

    ensureIncreaseCapacity();

    for (size_t i = bitSize; i > 0; --i) {
        size_t srcByteIdx = (i - 1) / 8;
        size_t srcBitPos = (i - 1) % 8;
        uint8_t srcByte = bytes->Get(srcByteIdx);
        bool bit = (srcByte >> srcBitPos) & 1u;

        size_t dstByteIdx = i / 8;
        size_t dstBitPos = i % 8;
        uint8_t dstByte = bytes->Get(dstByteIdx);
        if (bit) {
            dstByte |= static_cast<uint8_t>(1u << dstBitPos);
        } else {
            dstByte &= static_cast<uint8_t>(~(1u << dstBitPos));
        }
        bytes->Set(dstByteIdx, dstByte);
    }

    uint8_t firstByte = bytes->Get(0);
    if (elem) {
        firstByte |= 0x01;
    } else {
        firstByte &= ~0x01u;
    }
    bytes->Set(0, firstByte);

    ++bitSize;
    return this;
}


BitSequence *BitSequence::insertAtImpl(const bool &elem, size_t index) {
    if (index > bitSize)
        throw std::out_of_range("BitSequence: insert index out of range");

    if (index == bitSize) return appendImpl(elem);
    if (index == 0) return prependImpl(elem);

    ensureIncreaseCapacity();

    size_t newBitSize = bitSize + 1;

    for (size_t i = bitSize; i > index; --i) {
        size_t srcByteIdx = (i - 1) / 8;
        size_t srcBitPos = (i - 1) % 8;
        uint8_t srcByte = bytes->Get(srcByteIdx);
        bool bit = (srcByte >> srcBitPos) & 1u;

        size_t dstByteIdx = i / 8;
        size_t dstBitPos = i % 8;
        uint8_t dstByte = bytes->Get(dstByteIdx);
        if (bit) {
            dstByte |= static_cast<uint8_t>(1u << dstBitPos);
        } else {
            dstByte &= static_cast<uint8_t>(~(1u << dstBitPos));
        }
        bytes->Set(dstByteIdx, dstByte);
    }

    size_t bIdx = index / 8;
    size_t bPos = index % 8;
    uint8_t byteVal = bytes->Get(bIdx);
    if (elem) {
        byteVal |= static_cast<uint8_t>(1u << bPos);
    } else {
        byteVal &= static_cast<uint8_t>(~(1u << bPos));
    }
    bytes->Set(bIdx, byteVal);

    bitSize = newBitSize;
    return this;
}

BitSequence *BitSequence::delImpl(size_t index) {
    if (index >= bitSize)
        throw std::out_of_range("BitSequence: delete index out of range");

    if (index == bitSize - 1) {
        --bitSize;
        if (bitSize > 0 && bitSize % 8 != 0) {
            size_t lastByteIdx = bitSize / 8;
            auto mask = static_cast<uint8_t>((1u << (bitSize % 8)) - 1);
            uint8_t lastByte = bytes->Get(lastByteIdx);
            bytes->Set(lastByteIdx, lastByte & mask);
        }
        ensureReductionCapacity();
        return this;
    }

    for (size_t i = index; i < bitSize - 1; ++i) {
        size_t srcByteIdx = (i + 1) / 8;
        size_t srcBitPos = (i + 1) % 8;
        uint8_t srcByte = bytes->Get(srcByteIdx);
        bool bit = (srcByte >> srcBitPos) & 1u;

        size_t dstByteIdx = i / 8;
        size_t dstBitPos = i % 8;
        uint8_t dstByte = bytes->Get(dstByteIdx);
        if (bit) {
            dstByte |= static_cast<uint8_t>(1u << dstBitPos);
        } else {
            dstByte &= static_cast<uint8_t>(~(1u << dstBitPos));
        }
        bytes->Set(dstByteIdx, dstByte);
    }

    --bitSize;

    if (bitSize > 0 && bitSize % 8 != 0) {
        size_t lastByteIdx = bitSize / 8;
        auto mask = static_cast<uint8_t>((1u << (bitSize % 8)) - 1);
        uint8_t lastByte = bytes->Get(lastByteIdx);
        bytes->Set(lastByteIdx, lastByte & mask);
    } else if (bitSize == 0) {
        bytes->Set(0, 0);
    }

    ensureReductionCapacity();

    return this;
}

BitSequence *BitSequence::concatImpl(const Sequence<bool> &other) {
    size_t otherLen = other.GetLength();
    if (otherLen == 0) return this;

    size_t oldBitSize = bitSize;
    size_t newBitSize = bitSize + otherLen;
    size_t newByteCount = (newBitSize + 7) / 8;
    size_t oldByteCount = (oldBitSize + 7) / 8;

    if (newByteCount > oldByteCount) {
        bytes->Resize(newByteCount);
    }

    bitSize = newBitSize;

    auto *enumPtr = other.GetEnumerator();
    size_t targetIdx = oldBitSize;
    while (enumPtr->MoveNext()) {
        setBit(targetIdx, enumPtr->Current());
        ++targetIdx;
    }
    delete enumPtr;
    return this;
}

BitSequence *BitSequence::And(const BitSequence &other) const {
    size_t len = std::min(bitSize, other.bitSize);

    if (len == 0) {
        return new BitSequence(new DynamicArray<uint8_t>(0), 0);
    }

    size_t byteCount = (len + 7) / 8;
    auto *newBytes = new DynamicArray<uint8_t>(byteCount);

    for (size_t i = 0; i < byteCount; ++i) {
        uint8_t resultByte = bytes->Get(i) & other.bytes->Get(i);
        newBytes->Set(i, resultByte);
    }

    if (len % 8 != 0) {
        size_t lastByteIdx = len / 8;
        auto mask = static_cast<uint8_t>((1u << (len % 8)) - 1);
        uint8_t lastByte = newBytes->Get(lastByteIdx);
        newBytes->Set(lastByteIdx, lastByte & mask);
    }

    return new BitSequence(newBytes, len);
}

BitSequence *BitSequence::Or(const BitSequence &other) const {
    size_t maxLen = std::max(bitSize, other.bitSize);

    if (maxLen == 0) {
        return new BitSequence(new DynamicArray<uint8_t>(0), 0);
    }

    size_t byteCount = (maxLen + 7) / 8;
    auto *newBytes = new DynamicArray<uint8_t>(byteCount);

    size_t thisByteCount = (bitSize + 7) / 8;
    size_t otherByteCount = (other.bitSize + 7) / 8;

    for (size_t i = 0; i < byteCount; ++i) {
        uint8_t a = (i < thisByteCount) ? bytes->Get(i) : 0;
        uint8_t b = (i < otherByteCount) ? other.bytes->Get(i) : 0;
        newBytes->Set(i, a | b);
    }

    if (maxLen % 8 != 0) {
        size_t lastByteIdx = maxLen / 8;
        auto mask = static_cast<uint8_t>((1u << (maxLen % 8)) - 1);
        uint8_t lastByte = newBytes->Get(lastByteIdx);
        newBytes->Set(lastByteIdx, lastByte & mask);
    }

    return new BitSequence(newBytes, maxLen);
}

BitSequence *BitSequence::Xor(const BitSequence &other) const {
    size_t maxLen = std::max(bitSize, other.bitSize);

    if (maxLen == 0) {
        return new BitSequence(new DynamicArray<uint8_t>(0), 0);
    }

    size_t byteCount = (maxLen + 7) / 8;
    auto *newBytes = new DynamicArray<uint8_t>(byteCount);

    size_t thisByteCount = (bitSize + 7) / 8;
    size_t otherByteCount = (other.bitSize + 7) / 8;

    for (size_t i = 0; i < byteCount; ++i) {
        uint8_t a = (i < thisByteCount) ? bytes->Get(i) : 0;
        uint8_t b = (i < otherByteCount) ? other.bytes->Get(i) : 0;
        newBytes->Set(i, a ^ b);
    }

    if (maxLen % 8 != 0) {
        size_t lastByteIdx = maxLen / 8;
        auto mask = static_cast<uint8_t>((1u << (maxLen % 8)) - 1);
        uint8_t lastByte = newBytes->Get(lastByteIdx);
        newBytes->Set(lastByteIdx, lastByte & mask);
    }

    return new BitSequence(newBytes, maxLen);
}

BitSequence *BitSequence::Not() const {
    if (bitSize == 0) {
        return new BitSequence(new DynamicArray<uint8_t>(0), 0);
    }

    size_t byteCount = (bitSize + 7) / 8;
    auto *newBytes = new DynamicArray<uint8_t>(byteCount);

    for (size_t i = 0; i < byteCount; ++i) {
        auto inverted = static_cast<uint8_t>(~bytes->Get(i));
        newBytes->Set(i, inverted);
    }

    if (bitSize % 8 != 0) {
        size_t lastByteIdx = bitSize / 8;
        auto mask = static_cast<uint8_t>((1u << (bitSize % 8)) - 1);
        uint8_t lastByte = newBytes->Get(lastByteIdx);
        newBytes->Set(lastByteIdx, lastByte & mask);
    }

    return new BitSequence(newBytes, bitSize);
}


BitSequence *BitSequence::createEmpty() const {
    return new BitSequence(new DynamicArray<uint8_t>(0), 0);
}

BitSequence *BitSequence::instance() {
    return this;
}

BitSequence::~BitSequence() {
    delete bytes;
}

std::string BitSequence::toDecimalString() const {
    if (bitSize == 0) return "0";

    std::string result = "0";


    for (size_t i = bitSize; i-- > 0;) {
        int carry = 0;
        for (size_t j = result.size(); j-- > 0;) {
            int digit = result[j] - '0';
            digit = digit * 2 + carry;
            carry = digit / 10;
            result[j] = static_cast<char>((digit % 10) + '0');
        }
        if (carry > 0) {
            result.insert(result.begin(), static_cast<char>(carry + '0'));
        }

        if (getBit(i)) {
            carry = 1;
            for (size_t j = result.size(); j-- > 0 && carry > 0;) {
                int digit = result[j] - '0' + carry;
                carry = digit / 10;
                result[j] = static_cast<char>((digit % 10) + '0');
            }
            if (carry > 0) {
                result.insert(result.begin(), '1');
            }
        }
    }

    return result;
}
