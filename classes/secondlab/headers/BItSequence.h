#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include <cstdint>
#include <stdexcept>
#include "Sequence.h"
#include "DynamicArray.h"
#include "IEnumerator.h"

template<typename T>
class MutableArraySequence;

class BitSequence : public Sequence<bool> {
public:
    BitSequence(const bool *bits, size_t numBits);

    BitSequence(DynamicArray<uint8_t> *b, size_t bits);

    BitSequence(const char *bitStr);

    size_t GetLength() const override;

    BitSequence *GetSubsequence(size_t startIndex, size_t endIndex) const override;

    IEnumerator<bool> *GetEnumerator() const override;

    BitSequence *And(const BitSequence &other) const;

    BitSequence *Or(const BitSequence &other) const;

    BitSequence *Xor(const BitSequence &other) const;

    BitSequence *Not() const;

    bool GetFirst() const override;

    bool GetLast() const override;

    uint8_t getByte(size_t byteIdx) const;

    void setByte(size_t byteIdx, uint8_t val);

    bool getBit(size_t bitIdx) const;

    BitSequence *setBit(size_t bitIdx, bool value);

    BitSequence *createEmpty() const override;

    std::string toDecimalString() const;

    ~BitSequence() override;

protected:
    BitSequence *delImpl(size_t index) override;

    BitSequence *appendImpl(const bool &elem) override;

    BitSequence *prependImpl(const bool &elem) override;

    BitSequence *insertAtImpl(const bool &elem, size_t index) override;

    BitSequence *concatImpl(const Sequence<bool> &other) override;

    BitSequence *instance() override;


    class BitEnumerator : public IEnumerator<bool> {
    private:
        const BitSequence *seq;
        size_t currentIdx;

    public:
        explicit BitEnumerator(const BitSequence *s) : seq(s), currentIdx(0) {
        }

        bool MoveNext() override {
            if (currentIdx < seq->GetLength()) {
                ++currentIdx;
                return true;
            }
            return false;
        }

        bool Current() const override {
            if (currentIdx == 0 || currentIdx > seq->GetLength())
                throw std::out_of_range("BitEnumerator: invalid access");
            return seq->getBit(currentIdx - 1);
        }

        void Reset() override {
            currentIdx = 0;
        }

        ~BitEnumerator() override = default;
    };

private:
    DynamicArray<uint8_t> *bytes;
    size_t bitSize;

    void ensureReductionCapacity();

    void ensureIncreaseCapacity();

};

#endif
