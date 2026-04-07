#ifndef MUTABLE_ARRAY_SEQUENCE_H
#define MUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template<typename T>
class MutableArraySequence : public ArraySequence<T> {
public:
    MutableArraySequence() : ArraySequence<T>() {}
    MutableArraySequence(const T* items, size_t count) : ArraySequence<T>(items, count) {}
    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}
    MutableArraySequence(DynamicArray<T>* arr, size_t size) : ArraySequence<T>(arr, size) {}
protected:
    Sequence<T>* instance() override {
        return this;
    }

    Sequence<T>* createEmpty() const override {
        return new MutableArraySequence<T>();
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (endIndex >= this->size || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence bounds");
        size_t newLen = endIndex - startIndex + 1;
        T* items = new T[newLen];
        for (size_t i = 0; i < newLen; ++i)
            items[i] = this->data->Get(startIndex + i);
        Sequence<T>* result = new MutableArraySequence<T>(items, newLen);
        delete[] items;
        return result;
    }

    IEnumerator<T>* GetEnumerator() override {
        class ArrayEnumerator : public IEnumerator<T> {
        private:
            const MutableArraySequence<T>* seq;
            size_t index;
            size_t length;
        public:
            ArrayEnumerator(const MutableArraySequence<T>* s)
                : seq(s), index(0), length(s->GetLength()) {}

            bool MoveNext() override {
                if (index < length) {
                    ++index;
                    return index <= length;
                }
                return false;
            }

            T Current() const override {
                if (index == 0 || index > length)
                    throw std::out_of_range("Enumerator out of range");
                return seq->Get(index - 1);
            }

            void Reset() override {
                index = 0;
            }
        };
        return new ArrayEnumerator(this);
    }
};

#endif // MUTABLE_ARRAY_SEQUENCE_H