#include "BitSequence.h"
#include <stdexcept>
#include <algorithm>

static size_t byteIndex(size_t bitIndex) {
    return bitIndex / 8;
}

static size_t bitOffset(size_t bitIndex) {
    return bitIndex % 8;
}

void BitSequence::clearUnusedBits() {
    if (bitSize == 0) return;
    size_t lastByte = byteIndex(bitSize - 1);
    size_t usedBitsInLastByte = (bitSize - 1) % 8 + 1;
    if (usedBitsInLastByte < 8) {
        uint8_t mask = 0xFF >> usedBitsInLastByte;
        uint8_t byte = data.Get(lastByte);
        byte &= ~mask;
        data.Set(lastByte, byte);
    }
}

void BitSequence::ensureCapacity(size_t newBitSize) {
    size_t requiredBytes = (newBitSize + 7) / 8;
    if (data.GetSize() < requiredBytes) {
        data.Resize(requiredBytes);
    }
}

BitSequence::BitSequence() : data(0), bitSize(0) {
}

BitSequence::BitSequence(size_t size, bool initialValue)
    : data((size + 7) / 8), bitSize(size) {
    uint8_t fillByte = initialValue ? 0xFF : 0x00;
    for (size_t i = 0; i < data.GetSize(); ++i) {
        data.Set(i, fillByte);
    }
    if (initialValue) {
        clearUnusedBits();
    } else {
        clearUnusedBits();
    }
}

BitSequence::BitSequence(const bool *items, size_t count)
    : data((count + 7) / 8), bitSize(count) {
    for (size_t i = 0; i < data.GetSize(); ++i)
        data.Set(i, 0);
    for (size_t i = 0; i < count; ++i) {
        if (items[i])
            Set(i, true);
    }
}

BitSequence::BitSequence(const BitSequence &other)
    : data(other.data), bitSize(other.bitSize) {
}

BitSequence::BitSequence(BitSequence &&other) noexcept
    : data(std::move(other.data)), bitSize(other.bitSize) {
    other.bitSize = 0;
}

BitSequence &BitSequence::operator=(const BitSequence &other) {
    if (this != &other) {
        data = other.data;
        bitSize = other.bitSize;
    }
    return *this;
}

BitSequence &BitSequence::operator=(BitSequence &&other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        bitSize = other.bitSize;
        other.bitSize = 0;
    }
    return *this;
}

bool BitSequence::Get(size_t index) const {
    if (index >= bitSize)
        throw std::out_of_range("BitSequence::Get index out of range");
    size_t byteIdx = byteIndex(index);
    size_t bitOff = bitOffset(index);
    return (data.Get(byteIdx) >> bitOff) & 1;
}

bool BitSequence::GetFirst() const {
    if (bitSize == 0)
        throw std::out_of_range("BitSequence::GetFirst on empty sequence");
    return Get(0);
}

bool BitSequence::GetLast() const {
    if (bitSize == 0)
        throw std::out_of_range("BitSequence::GetLast on empty sequence");
    return Get(bitSize - 1);
}

void BitSequence::Set(size_t index, bool value) {
    if (index >= bitSize)
        throw std::out_of_range("BitSequence::Set index out of range");
    size_t byteIdx = byteIndex(index);
    size_t bitOff = bitOffset(index);
    uint8_t byte = data.Get(byteIdx);
    uint8_t mask = 1 << bitOff;
    if (value)
        byte |= mask;
    else
        byte &= ~mask;
    data.Set(byteIdx, byte);
}

Sequence<bool> *BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex || endIndex > bitSize)
        throw std::out_of_range("BitSequence::GetSubsequence invalid range");
    size_t subLen = endIndex - startIndex;
    BitSequence *result = new BitSequence(subLen, false);
    for (size_t i = 0; i < subLen; ++i) {
        if (Get(startIndex + i))
            result->Set(i, true);
    }
    return result;
}

IEnumerator<bool> *BitSequence::GetEnumerator() {
    return new Enumerator(this);
}

Sequence<bool> *BitSequence::createEmpty() const {
    return new BitSequence();
}

Sequence<bool> *BitSequence::appendImpl(const bool &elem) {
    BitSequence *result = new BitSequence(bitSize + 1, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (Get(i))
            result->Set(i, true);
    }
    if (elem)
        result->Set(bitSize, true);
    return result;
}

Sequence<bool> *BitSequence::prependImpl(const bool &elem) {
    BitSequence *result = new BitSequence(bitSize + 1, false);
    if (elem)
        result->Set(0, true);
    for (size_t i = 0; i < bitSize; ++i) {
        if (Get(i))
            result->Set(i + 1, true);
    }
    return result;
}

Sequence<bool> *BitSequence::insertAtImpl(const bool &elem, size_t index) {
    if (index > bitSize)
        throw std::out_of_range("BitSequence::insertAtImpl index out of range");
    BitSequence *result = new BitSequence(bitSize + 1, false);
    for (size_t i = 0; i < index; ++i) {
        if (Get(i))
            result->Set(i, true);
    }
    if (elem)
        result->Set(index, true);
    for (size_t i = index; i < bitSize; ++i) {
        if (Get(i))
            result->Set(i + 1, true);
    }
    return result;
}

Sequence<bool> *BitSequence::concatImpl(const Sequence<bool> *other) {
    size_t otherLen = other->GetLength();
    BitSequence *result = new BitSequence(bitSize + otherLen, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (Get(i))
            result->Set(i, true);
    }
    for (size_t i = 0; i < otherLen; ++i) {
        if (other->Get(i))
            result->Set(bitSize + i, true);
    }
    return result;
}

BitSequence *BitSequence::And(const BitSequence &other) const {
    size_t minSize = std::min(bitSize, other.bitSize);
    BitSequence *result = new BitSequence(minSize, false);
    for (size_t i = 0; i < minSize; ++i) {
        bool bit = Get(i) && other.Get(i);
        if (bit)
            result->Set(i, true);
    }
    return result;
}

BitSequence *BitSequence::Or(const BitSequence &other) const {
    size_t maxSize = std::max(bitSize, other.bitSize);
    BitSequence *result = new BitSequence(maxSize, false);
    for (size_t i = 0; i < maxSize; ++i) {
        bool bit = (i < bitSize && Get(i)) || (i < other.bitSize && other.Get(i));
        if (bit)
            result->Set(i, true);
    }
    return result;
}

BitSequence *BitSequence::Xor(const BitSequence &other) const {
    size_t maxSize = std::max(bitSize, other.bitSize);
    BitSequence *result = new BitSequence(maxSize, false);
    for (size_t i = 0; i < maxSize; ++i) {
        bool a = (i < bitSize) ? Get(i) : false;
        bool b = (i < other.bitSize) ? other.Get(i) : false;
        if (a ^ b)
            result->Set(i, true);
    }
    return result;
}

BitSequence *BitSequence::Not() const {
    BitSequence *result = new BitSequence(bitSize, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (!Get(i))
            result->Set(i, true);
    }
    return result;
}

bool BitSequence::operator==(const BitSequence &other) const {
    if (bitSize != other.bitSize) return false;
    for (size_t i = 0; i < bitSize; ++i) {
        if (Get(i) != other.Get(i))
            return false;
    }
    return true;
}
