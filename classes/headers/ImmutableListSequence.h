#ifndef SECONDLAB_IMMUTABLELISTSEQUENCE_H
#define SECONDLAB_IMMUTABLELISTSEQUENCE_H

#include "ListSequence.h"

template<typename T>
class ImmutableListSequence : public ListSequence<T> {
private:
    Sequence<T>* instance() override {
        return new ImmutableListSequence<T>(*this);
    }

    Sequence<T>* appendImpl(const T& elem) override {
        ImmutableListSequence<T>* copy = new ImmutableListSequence<T>(*this);
        copy->data->Append(elem);
        ++copy->size;
        return copy;
    }

    Sequence<T>* prependImpl(const T& elem) override {
        ImmutableListSequence<T>* copy = new ImmutableListSequence<T>(*this);
        copy->data->Prepend(elem);
        ++copy->size;
        return copy;
    }

    Sequence<T>* insertAtImpl(const T& elem, size_t index) override {
        if (index > this->size) throw std::out_of_range("Index out of range");
        ImmutableListSequence<T>* copy = new ImmutableListSequence<T>(*this);
        copy->data->InsertAt(elem, index);
        ++copy->size;
        return copy;
    }

    Sequence<T>* createEmpty() const override {
        return new ImmutableListSequence<T>();
    }

    // Реализация GetSubsequence
    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex < 0 || static_cast<size_t>(endIndex) >= this->size || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence bounds");
        size_t newLen = static_cast<size_t>(endIndex - startIndex + 1);
        T* items = new T[newLen];
        for (size_t i = 0; i < newLen; ++i)
            items[i] = this->data->Get(static_cast<size_t>(startIndex) + i);
        Sequence<T>* result = new ImmutableListSequence<T>(items, newLen);
        delete[] items;
        return result;
    }

public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {}
    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}
    ImmutableListSequence(LinkedList<T>* list, size_t sz) : ListSequence<T>(list, sz) {}
};

#endif //SECONDLAB_IMMUTABLELISTSEQUENCE_H