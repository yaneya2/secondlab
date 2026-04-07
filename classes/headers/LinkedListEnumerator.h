#ifndef SECONDLAB_LINKEDLISTITERATOR_H
#define SECONDLAB_LINKEDLISTITERATOR_H

#include "LinkedList.h"
#include "IEnumerator.h"

template<typename T>
class LinkedListEnumerator : public IEnumerator<T> {
private:
    const LinkedList<T>* list;
    LinkedList<T>::Node* current;
public:
    LinkedListEnumerator(const LinkedList<T>* lst) : list(lst), current(nullptr) {}

    bool MoveNext() override {
        if (current == nullptr)
            current = list->head;
        else
            current = current->next;
        return current != nullptr;
    }

    T Current() const override {
        if (current == nullptr) throw std::out_of_range("Enumerator out of range");
        return current->data;
    }

    void Reset() override { current = nullptr; }
};

#endif //SECONDLAB_LINKEDLISTITERATOR_H