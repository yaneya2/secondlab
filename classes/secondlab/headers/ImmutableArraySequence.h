#ifndef IMMUTABLE_ARRAY_SEQUENCE_H
#define IMMUTABLE_ARRAY_SEQUENCE_H

#include "ArraySequence.h"

template<typename T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    ImmutableArraySequence() : ArraySequence<T>() {}

    ImmutableArraySequence(const T *items, size_t count) : ArraySequence<T>(items, count) {}

    ImmutableArraySequence(const ImmutableArraySequence<T> &other) : ArraySequence<T>(other) {}

    ImmutableArraySequence(DynamicArray<T> *arr, size_t size) : ArraySequence<T>(arr, size) {}

protected:
    Sequence<T> *instance() override {
        return new ImmutableArraySequence<T>(*this);
    }
};

#endif // IMMUTABLE_ARRAY_SEQUENCE_H
