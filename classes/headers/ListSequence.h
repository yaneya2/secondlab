#ifndef SECONDLAB_LISTSEQUENCE_H
#define SECONDLAB_LISTSEQUENCE_H

#include "Sequence.h"
#include "LinkedList.h"
#include "LinkedListEnumerator.h"

template<class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* data;
    size_t size;      // текущее количество элементов

    // Внутренний конструктор для наследников
    ListSequence(LinkedList<T>* list, size_t size) : data(list), size(size) {}

public:
    // Конструкторы
    ListSequence() : data(new LinkedList<T>()), size(0) {}
    ListSequence(const T* items, size_t count) : data(new LinkedList<T>(items, count)), size(count) {}
    ListSequence(const ListSequence<T>& other) : data(new LinkedList<T>(*(other.data))), size(other.size) {}
    virtual ~ListSequence() { delete data; }

    // Операции чтения
    T GetFirst() const override {
        if (size == 0) throw std::out_of_range("Sequence is empty");
        return data->GetFirst();
    }

    T GetLast() const override {
        if (size == 0) throw std::out_of_range("Sequence is empty");
        return data->GetLast();
    }

    T Get(size_t index) const override {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data->Get(index);
    }

    size_t GetLength() const override {
        return size;
    }

    Sequence<T>* appendImpl(const T& elem) override {
        data->Append(elem);
        ++size;
        return this;
    }

    Sequence<T>* prependImpl(const T& elem) override {
        data->Prepend(elem);
        ++size;
        return this;
    }

    Sequence<T>* insertAtImpl(const T& elem, size_t index) override {
        if (index > size) throw std::out_of_range("Index out of range");
        data->InsertAt(elem, index);
        ++size;
        return this;
    }

    Sequence<T>* concatImpl(Sequence<T>* other) override {
        size_t otherLen = other->GetLength();
        for (size_t i = 0; i < otherLen; ++i) {
            data->Append(other->Get(i));
        }
        size += otherLen;
        return this;
    }

    IEnumerator<T>* GetEnumerator() override {
        return new LinkedListEnumerator<T>(this->data);
    }
};

#endif //SECONDLAB_LISTSEQUENCE_H