#ifndef SECONDLAB_BITSEQUENCE_H
#define SECONDLAB_BITSEQUENCE_H

#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include <cstdint>

#include "Sequence.h"
#include "DynamicArray.h"
#include <string>
#include <stdexcept>

enum class Bit : bool { Zero = 0, One = 1 };

inline char bitToChar(Bit b) { return (b == Bit::One) ? '1' : '0'; }

class BitSequence : public Sequence<Bit> {
private:
    DynamicArray<uint8_t> data;   // упакованные биты
    size_t bitCount;               // общее количество битов

    void setBit(size_t index, Bit value);
    Bit getBit(size_t index) const;
    void ensureCapacity(size_t newBitCount);

public:
    // Конструкторы
    BitSequence();
    BitSequence(const Bit* items, size_t count);
    BitSequence(size_t count, Bit defaultValue = Bit::Zero);
    BitSequence(const std::string& bitString);
    BitSequence(const BitSequence& other);
    ~BitSequence() = default;

    // Операции чтения (Sequence<Bit>)
    Bit GetFirst() const override;
    Bit GetLast() const override;
    Bit Get(size_t index) const override;
    Sequence<Bit>* GetSubsequence(size_t startIndex, size_t endIndex) const override;
    size_t GetLength() const override;

    // Операции изменения (mutable)
    Sequence<Bit>* Append(const Bit& elem) override;
    Sequence<Bit>* Prepend(const Bit& elem) override;
    Sequence<Bit>* InsertAt(const Bit& elem, size_t index) override;
    Sequence<Bit>* Concat(Sequence<Bit>* other) override;

    // Побитовые операции (возвращают новые объекты)
    BitSequence And(const BitSequence& other) const;
    BitSequence Or(const BitSequence& other) const;
    BitSequence Xor(const BitSequence& other) const;
    BitSequence Not() const;

    // Дополнительные методы
    std::string ToString() const;
    IEnumerator<Bit>* GetEnumerator() const;

protected:
    // Реализация protected методов Sequence
    Sequence<Bit>* instance() override;
    Sequence<Bit>* appendImpl(const Bit& elem) override;
    Sequence<Bit>* prependImpl(const Bit& elem) override;
    Sequence<Bit>* insertAtImpl(const Bit& elem, size_t index) override;
    Sequence<Bit>* concatImpl(Sequence<Bit>* other) override;
    Sequence<Bit>* createEmpty() const override;

public:
    IEnumerator<Bit> * GetEnumerator() override;
};

#endif // BITSEQUENCE_H

#endif //SECONDLAB_BITSEQUENCE_H