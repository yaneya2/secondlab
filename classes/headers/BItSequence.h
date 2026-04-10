#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include <cstdint>
#include <stdexcept>
#include <functional>
#include "Sequence.h"
#include "DynamicArray.h"
#include "IEnumerator.h"

template<typename T>
class MutableArraySequence;

class BitSequence : public Sequence<bool> {
private:
    DynamicArray<uint8_t>* bytes;
    size_t bitSize;

    BitSequence(DynamicArray<uint8_t>* b, size_t bits);
    uint8_t getByte(size_t byteIdx) const;
    void setByte(size_t byteIdx, uint8_t val);
    bool getBit(size_t bitIdx) const;
    BitSequence* setBit(size_t bitIdx, bool value);
    size_t numBytes() const;
    BitSequence* createCopy() const;

    BitSequence* createEmpty() const override;
    BitSequence* instance() override;

    BitSequence* bitwiseAndOrXor(const BitSequence* other, std::function<bool(bool,bool)> op) const;

public:
    BitSequence(const bool* bits, size_t numBits);
    BitSequence(const char* bitStr);

    bool GetFirst() const override;
    bool GetLast() const override;
    bool Get(size_t index) const override;
    size_t GetLength() const override;
    BitSequence* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    IEnumerator<bool>* GetEnumerator() override;

    BitSequence* And(const BitSequence* other) const;
    BitSequence* Or(const BitSequence* other) const;
    BitSequence* Xor(const BitSequence* other) const;
    BitSequence* Not() const;

    template<typename U>
    Sequence<U>* Mask(const Sequence<U>* other) const;

    BitSequence* appendImpl(const bool& elem) override;
    BitSequence* prependImpl(const bool& elem) override;
    BitSequence* insertAtImpl(const bool& elem, size_t index) override;
    BitSequence* concatImpl(const Sequence<bool>* other) override;

    ~BitSequence() override;
};

#endif