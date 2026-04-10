#include "BitSequence.h"
#include <stdexcept>
#include <algorithm>

// ------------------- Вспомогательные методы -------------------

void BitSequence::clearUnusedBits() {
    if (bitSize == 0) return;
    size_t lastByte = byteIndex(bitSize - 1);
    size_t usedBitsInLastByte = (bitSize - 1) % 8 + 1;
    if (usedBitsInLastByte < 8) {
        uint8_t mask = 0xFF >> usedBitsInLastByte; // оставляем только старшие usedBitsInLastByte битов
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

// ------------------- Конструкторы ----------------------

BitSequence::BitSequence() : data(0), bitSize(0) {}

BitSequence::BitSequence(size_t size, bool initialValue)
    : data((size + 7) / 8), bitSize(size) {
    uint8_t fillByte = initialValue ? 0xFF : 0x00;
    for (size_t i = 0; i < data.GetSize(); ++i) {
        data.Set(i, fillByte);
    }
    if (initialValue) {
        clearUnusedBits(); // обнуляем неиспользуемые биты
    } // иначе нули уже стоят, но на всякий случай тоже вызовем
    else {
        clearUnusedBits();
    }
}

BitSequence::BitSequence(const bool* items, size_t count)
    : data((count + 7) / 8), bitSize(count) {
    for (size_t i = 0; i < data.GetSize(); ++i)
        data.Set(i, 0);
    for (size_t i = 0; i < count; ++i) {
        if (items[i])
            setBitUnchecked(i, true);
    }
}

BitSequence::BitSequence(const BitSequence& other)
    : data(other.data), bitSize(other.bitSize) {}

BitSequence::BitSequence(BitSequence&& other) noexcept
    : data(std::move(other.data)), bitSize(other.bitSize) {
    other.bitSize = 0;
}

BitSequence& BitSequence::operator=(const BitSequence& other) {
    if (this != &other) {
        data = other.data;
        bitSize = other.bitSize;
    }
    return *this;
}

BitSequence& BitSequence::operator=(BitSequence&& other) noexcept {
    if (this != &other) {
        data = std::move(other.data);
        bitSize = other.bitSize;
        other.bitSize = 0;
    }
    return *this;
}

// ------------------- Методы доступа --------------------

bool BitSequence::Get(size_t index) const {
    if (index >= bitSize)
        throw std::out_of_range("BitSequence::Get index out of range");
    return getBitUnchecked(index);
}

bool BitSequence::GetFirst() const {
    if (bitSize == 0)
        throw std::out_of_range("BitSequence::GetFirst on empty sequence");
    return getBitUnchecked(0);
}

bool BitSequence::GetLast() const {
    if (bitSize == 0)
        throw std::out_of_range("BitSequence::GetLast on empty sequence");
    return getBitUnchecked(bitSize - 1);
}

void BitSequence::Set(size_t index, bool value) {
    if (index >= bitSize)
        throw std::out_of_range("BitSequence::Set index out of range");
    setBitUnchecked(index, value);
}

// ------------------- Операции Sequence -----------------

Sequence<bool>* BitSequence::GetSubsequence(size_t startIndex, size_t endIndex) const {
    if (startIndex > endIndex || endIndex > bitSize)
        throw std::out_of_range("BitSequence::GetSubsequence invalid range");
    size_t subLen = endIndex - startIndex;
    BitSequence* result = new BitSequence(subLen, false);
    for (size_t i = 0; i < subLen; ++i) {
        if (getBitUnchecked(startIndex + i))
            result->setBitUnchecked(i, true);
    }
    return result;
}

IEnumerator<bool>* BitSequence::GetEnumerator() {
    return new Enumerator(this);
}

// ------------------- Реализация protected методов -----------------

Sequence<bool>* BitSequence::createEmpty() const {
    return new BitSequence();
}

Sequence<bool>* BitSequence::appendImpl(const bool& elem) {
    BitSequence* result = new BitSequence(bitSize + 1, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (getBitUnchecked(i))
            result->setBitUnchecked(i, true);
    }
    if (elem)
        result->setBitUnchecked(bitSize, true);
    return result;
}

Sequence<bool>* BitSequence::prependImpl(const bool& elem) {
    BitSequence* result = new BitSequence(bitSize + 1, false);
    if (elem)
        result->setBitUnchecked(0, true);
    for (size_t i = 0; i < bitSize; ++i) {
        if (getBitUnchecked(i))
            result->setBitUnchecked(i + 1, true);
    }
    return result;
}

Sequence<bool>* BitSequence::insertAtImpl(const bool& elem, size_t index) {
    if (index > bitSize)
        throw std::out_of_range("BitSequence::insertAtImpl index out of range");
    BitSequence* result = new BitSequence(bitSize + 1, false);
    for (size_t i = 0; i < index; ++i) {
        if (getBitUnchecked(i))
            result->setBitUnchecked(i, true);
    }
    if (elem)
        result->setBitUnchecked(index, true);
    for (size_t i = index; i < bitSize; ++i) {
        if (getBitUnchecked(i))
            result->setBitUnchecked(i + 1, true);
    }
    return result;
}

Sequence<bool>* BitSequence::concatImpl(const Sequence<bool>* other) {
    size_t otherLen = other->GetLength();
    BitSequence* result = new BitSequence(bitSize + otherLen, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (getBitUnchecked(i))
            result->setBitUnchecked(i, true);
    }
    for (size_t i = 0; i < otherLen; ++i) {
        if (other->Get(i))
            result->setBitUnchecked(bitSize + i, true);
    }
    return result;
}

// ------------------- Побитовые операции -----------------

BitSequence* BitSequence::And(const BitSequence& other) const {
    size_t minSize = std::min(bitSize, other.bitSize);
    BitSequence* result = new BitSequence(minSize, false);
    for (size_t i = 0; i < minSize; ++i) {
        bool bit = getBitUnchecked(i) && other.getBitUnchecked(i);
        if (bit)
            result->setBitUnchecked(i, true);
    }
    return result;
}
//исрправить не рабоатет с разными размерностями
BitSequence* BitSequence::Or(const BitSequence& other) const {
    size_t maxSize = std::max(bitSize, other.bitSize);
    BitSequence* result = new BitSequence(maxSize, false);
    for (size_t i = 0; i < maxSize; ++i) {
        bool bit = (i < bitSize && getBitUnchecked(i)) || (i < other.bitSize && other.getBitUnchecked(i));
        if (bit)
            result->setBitUnchecked(i, true);
    }
    return result;
}

BitSequence* BitSequence::Xor(const BitSequence& other) const {
    size_t maxSize = std::max(bitSize, other.bitSize);
    BitSequence* result = new BitSequence(maxSize, false);
    for (size_t i = 0; i < maxSize; ++i) {
        bool a = (i < bitSize) ? getBitUnchecked(i) : false;
        bool b = (i < other.bitSize) ? other.getBitUnchecked(i) : false;
        if (a ^ b)
            result->setBitUnchecked(i, true);
    }
    return result;
}

BitSequence* BitSequence::Not() const {
    BitSequence* result = new BitSequence(bitSize, false);
    for (size_t i = 0; i < bitSize; ++i) {
        if (!getBitUnchecked(i))
            result->setBitUnchecked(i, true);
    }
    return result;
}

// ------------------- Побитовые операции на месте -----------------

void BitSequence::AndInPlace(const BitSequence& other) {
    size_t minSize = std::min(bitSize, other.bitSize);
    for (size_t i = 0; i < minSize; ++i) {
        bool bit = getBitUnchecked(i) && other.getBitUnchecked(i);
        setBitUnchecked(i, bit);
    }
    for (size_t i = minSize; i < bitSize; ++i) {
        setBitUnchecked(i, false);
    }
}

void BitSequence::OrInPlace(const BitSequence& other) {
    if (other.bitSize > bitSize) {
        size_t oldSize = bitSize;
        resize(other.bitSize);
        for (size_t i = oldSize; i < bitSize; ++i) {
            setBitUnchecked(i, false);
        }
    }
    for (size_t i = 0; i < other.bitSize; ++i) {
        if (other.getBitUnchecked(i))
            setBitUnchecked(i, true);
    }
}

void BitSequence::XorInPlace(const BitSequence& other) {
    if (other.bitSize > bitSize) {
        size_t oldSize = bitSize;
        resize(other.bitSize);
        for (size_t i = oldSize; i < bitSize; ++i) {
            setBitUnchecked(i, false);
        }
    }
    for (size_t i = 0; i < other.bitSize; ++i) {
        bool bit = getBitUnchecked(i) ^ other.getBitUnchecked(i);
        setBitUnchecked(i, bit);
    }
}

void BitSequence::NotInPlace() {
    for (size_t i = 0; i < bitSize; ++i) {
        setBitUnchecked(i, !getBitUnchecked(i));
    }
}

// ------------------- Изменение размера -----------------

void BitSequence::resize(size_t newSize) {
    if (newSize == bitSize) return;
    size_t newBytes = (newSize + 7) / 8;
    size_t oldBytes = data.GetSize();
    data.Resize(newBytes);
    if (newSize > bitSize) {
        for (size_t i = oldBytes; i < newBytes; ++i) {
            data.Set(i, 0);
        }
    }
    bitSize = newSize;
    clearUnusedBits();
}

// ------------------- Операторы сравнения -----------------

bool BitSequence::operator==(const BitSequence& other) const {
    if (bitSize != other.bitSize) return false;
    for (size_t i = 0; i < bitSize; ++i) {
        if (getBitUnchecked(i) != other.getBitUnchecked(i))
            return false;
    }
    return true;
}