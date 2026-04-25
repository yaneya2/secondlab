#ifndef SECONDLAB_IMMUTABLELISTSEQUENCE_H
#define SECONDLAB_IMMUTABLELISTSEQUENCE_H

#include "ListSequence.h"

template<typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence() : ListSequence<T>() {}

    ImmutableListSequence(const T *items, size_t count) : ListSequence<T>(items, count) {}

    ImmutableListSequence(const ImmutableListSequence<T> &other) : ListSequence<T>(other) {}

    ImmutableListSequence(LinkedList<T> *list, size_t size) : ListSequence<T>(list, size) {}

protected:
    Sequence<T> *Instance() override {
        return new ImmutableListSequence<T>(*this);
    }
};

#endif //SECONDLAB_IMMUTABLELISTSEQUENCE_H
