#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"
#include "IEnumerator.h"
#include <stdexcept>
#include <cstdint>
#include <algorithm>

class BitSequence : public Sequence<bool> {
private:
    DynamicArray<uint8_t> data;  // хранение битов в байтах
    size_t bitSize;              // количество значащих битов

    // Вспомогательные функции работы с битами
    static size_t byteIndex(size_t bitIndex) {
        return bitIndex / 8;
    }

    static uint8_t bitMask(size_t bitIndex) {
        return 1 << (7 - (bitIndex % 8));  // старший бит байта соответствует меньшему индексу
    }

    // Получить значение бита (без проверки границ)
    bool getBitUnchecked(size_t index) const {
        return (data.Get(byteIndex(index)) & bitMask(index)) != 0;
    }

    // Установить бит (без проверки границ)
    void setBitUnchecked(size_t index, bool value) {
        size_t byteIdx = byteIndex(index);
        uint8_t mask = bitMask(index);
        uint8_t byte = data.Get(byteIdx);
        if (value)
            byte |= mask;
        else
            byte &= ~mask;
        data.Set(byteIdx, byte);
    }

    // Инициализация нулями всех битов от startBit до bitSize (неиспользуемые биты последнего байта)
    void clearUnusedBits();

    // Увеличить размер массива до необходимого для хранения newBitSize битов
    void ensureCapacity(size_t newBitSize);

public:
    // ------------------- Конструкторы ----------------------
    BitSequence();
    explicit BitSequence(size_t size, bool initialValue = false);
    BitSequence(const bool* items, size_t count);
    BitSequence(const BitSequence& other);
    BitSequence(BitSequence&& other) noexcept;
    ~BitSequence() = default;

    // Операторы присваивания
    BitSequence& operator=(const BitSequence& other);
    BitSequence& operator=(BitSequence&& other) noexcept;

    // ------------------- Методы доступа --------------------
    size_t GetLength() const override { return bitSize; }
    bool Get(size_t index) const override;
    bool GetFirst() const override;
    bool GetLast() const override;

    // Установка бита (не входит в интерфейс Sequence, но полезна для битовой маски)
    void Set(size_t index, bool value);

    // ------------------- Операции Sequence -----------------
    Sequence<bool>* GetSubsequence(size_t startIndex, size_t endIndex) const override;

    // Вложенный класс-перечислитель
    class Enumerator : public IEnumerator<bool> {
    private:
        const BitSequence* seq;
        size_t currentIndex;
    public:
        explicit Enumerator(const BitSequence* sequence) : seq(sequence), currentIndex(0) {}
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
            return seq->getBitUnchecked(currentIndex - 1);
        }
        bool IsValid() const{
            return currentIndex > 0 && currentIndex <= seq->bitSize;
        }
    };

    IEnumerator<bool>* GetEnumerator() override;

protected:
    // Реализация чисто виртуальных методов базового класса
    Sequence<bool>* instance() override { return this; }
    Sequence<bool>* createEmpty() const override;
    Sequence<bool>* appendImpl(const bool& elem) override;
    Sequence<bool>* prependImpl(const bool& elem) override;
    Sequence<bool>* insertAtImpl(const bool& elem, size_t index) override;
    Sequence<bool>* concatImpl(const Sequence<bool>* other) override;

public:
    // ------------------- Побитовые операции (возвращают новые объекты) ----------------
    BitSequence* And(const BitSequence& other) const;
    BitSequence* Or(const BitSequence& other) const;
    BitSequence* Xor(const BitSequence& other) const;
    BitSequence* Not() const;

    // ------------------- Побитовые операции на месте (мутирующие) --------------------
    void AndInPlace(const BitSequence& other);
    void OrInPlace(const BitSequence& other);
    void XorInPlace(const BitSequence& other);
    void NotInPlace();

    // Изменение размера битовой последовательности (при увеличении новые биты = false)
    void resize(size_t newSize);

    // Операторы сравнения
    bool operator==(const BitSequence& other) const;
    bool operator!=(const BitSequence& other) const { return !(*this == other); }
};

#endif // BITSEQUENCE_H