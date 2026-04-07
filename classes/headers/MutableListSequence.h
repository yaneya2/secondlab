#ifndef SECONDLAB_MUTABLELISTSEQUNCE_H
#define SECONDLAB_MUTABLELISTSEQUNCE_H
#include "ListSequence.h"
#include "IEnumerator.h"

template<typename T>
class MutableListSequence : public ListSequence<T> {
public:
    MutableListSequence() : ListSequence<T>() {}
    MutableListSequence(const T* items, size_t count) : ListSequence<T>(items, count) {}
    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}
    MutableListSequence(LinkedList<T>* list, size_t sz) : ListSequence<T>(list, sz) {}

protected:
    Sequence<T>* instance() override {
        return this;
    }

    Sequence<T>* createEmpty() const override {
        return new MutableListSequence<T>();
    }

    Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (endIndex >= this->size || startIndex > endIndex)
            throw std::out_of_range("Invalid subsequence bounds");

        LinkedList<T>* sublist = this->data->GetSubList(startIndex, endIndex);
        return new MutableListSequence<T>(new LinkedList<T>(*sublist), sublist->GetSize());
    }

    IEnumerator<T>* GetEnumerator() override {
        // Локальный класс-итератор для списка
        class ListEnumerator : public IEnumerator<T> {
        private:
            const MutableListSequence<T>* seq;
            size_t currentIndex;
            size_t length;

        public:
            ListEnumerator(const MutableListSequence<T>* s)
                : seq(s), currentIndex(0), length(s->GetLength()) {}

            bool MoveNext() override {
                if (currentIndex < length) {
                    ++currentIndex;
                    return currentIndex <= length;
                }
                return false;
            }

            T Current() const override {
                if (currentIndex == 0 || currentIndex > length)
                    throw std::out_of_range("Enumerator out of range");
                return seq->Get(currentIndex - 1);
            }

            void Reset() override {
                currentIndex = 0;
            }
        };
        return new ListEnumerator(this);
    }
};

#endif //SECONDLAB_MUTABLELISTSEQUNCE_H