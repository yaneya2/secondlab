// ImmutableArraySequence.h
#ifndef IMMUTABLE_ARRAY_SEQUENCE_H
#define IMMUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"
#include "MutableArraySequence.h"

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {}
    ImmutableArraySequence(const T* items, size_t count) : ArraySequence<T>(items, count) {}
    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {}
    ImmutableArraySequence(DynamicArray<T>* arr, size_t size) : ArraySequence<T>(arr, size) {}
private:
    Sequence<T>* instance() override {
        return new ImmutableArraySequence<T>(*this);
    }

    Sequence<T>* createEmpty() const override {
        return new ImmutableArraySequence<T>();
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (endIndex >= this->size || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence bounds");
        size_t newLen = endIndex - startIndex + 1;
        T* items = new T[newLen];
        for (size_t i = 0; i < newLen; ++i)
            items[i] = this->data->Get(startIndex + i);
        Sequence<T>* result = new ImmutableArraySequence<T>(items, newLen);
        delete[] items;
        return result;
    }

    IEnumerator<T>* GetEnumerator() override {
        class ArrayEnumerator : public IEnumerator<T> {
        private:
            const ImmutableArraySequence<T>* seq;
            size_t index;
            size_t length;
        public:
            ArrayEnumerator(const ImmutableArraySequence<T>* s)
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
            void Reset() override { index = 0; }
        };
        return new ArrayEnumerator(this);
    }
};

#endif // IMMUTABLE_ARRAY_SEQUENCE_H