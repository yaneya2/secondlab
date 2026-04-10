#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include "IEnumerator.h"
#include <cstdint>

class BitSequence : public Sequence<bool> {
private:
    DynamicArray<uint8_t> data;
    size_t bitSize;

    void clearUnusedBits();

    void ensureCapacity(size_t newBitSize);

public:
    BitSequence();

    explicit BitSequence(size_t size, bool initialValue = false);

    BitSequence(const bool *items, size_t count);

    BitSequence(const BitSequence &other);

    BitSequence(BitSequence &&other) noexcept;

    ~BitSequence() = default;

    BitSequence &operator=(const BitSequence &other);

    BitSequence &operator=(BitSequence &&other) noexcept;

    size_t GetLength() const override { return bitSize; }

    bool Get(size_t index) const override;

    bool GetFirst() const override;

    bool GetLast() const override;

    void Set(size_t index, bool value);

    Sequence<bool> *GetSubsequence(size_t startIndex, size_t endIndex) const override;

    class Enumerator : public IEnumerator<bool> {
    private:
        const BitSequence *seq;
        size_t currentIndex;

    public:
        explicit Enumerator(const BitSequence *sequence) : seq(sequence), currentIndex(0) {
        }

        bool MoveNext() override {
            if (currentIndex < seq->bitSize) {
                ++currentIndex;
                return true;
            }
            return false;
        }

        void Reset() override {
            currentIndex = 0;
        }

        bool Current() const override {
            if (currentIndex == 0 || currentIndex > seq->bitSize)
                throw std::out_of_range("Enumerator::Current out of range");
            return seq->Get(currentIndex - 1);
        }

        bool IsValid() const {
            return currentIndex > 0 && currentIndex <= seq->bitSize;
        }
    };

    IEnumerator<bool> *GetEnumerator() override;

protected:
    Sequence<bool> *instance() override { return this; }

    Sequence<bool> *createEmpty() const override;

    Sequence<bool> *appendImpl(const bool &elem) override;

    Sequence<bool> *prependImpl(const bool &elem) override;

    Sequence<bool> *insertAtImpl(const bool &elem, size_t index) override;

    Sequence<bool> *concatImpl(const Sequence<bool> *other) override;

public:
    BitSequence *And(const BitSequence &other) const;

    BitSequence *Or(const BitSequence &other) const;

    BitSequence *Xor(const BitSequence &other) const;

    BitSequence *Not() const;

    bool operator==(const BitSequence &other) const;

    bool operator!=(const BitSequence &other) const { return !(*this == other); }
};

#endif
