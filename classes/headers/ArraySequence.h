#ifndef ARRAYSEQUENCE_H
#define ARRAYSEQUENCE_H

#include "Sequence.h"
#include "DynamicArray.h"

template<typename T>
class ArraySequence : public Sequence<T> {
private:
    void ensureCapacity() {
        size_t currentCap = this->data->GetSize();
        if (this->size < currentCap) return;
        size_t newCap = (currentCap == 0) ? 1 : currentCap * 2;
        this->data->Resize(newCap);
    }
protected:
    DynamicArray<T>* data;
    size_t size;      // текущее количество элементов

    // Внутренний конструктор для использования в наследниках (принимает готовый DynamicArray)
    ArraySequence(DynamicArray<T>* arr, size_t sz) : data(arr), size(sz) {}

public:
    ArraySequence() : data(new DynamicArray<T>(0)), size(0) {}
    ArraySequence(const T* items, size_t count) : data(new DynamicArray<T>(items, count)), size(count) {}
    ArraySequence(const ArraySequence<T>& other) : data(new DynamicArray<T>(*(other.data))), size(other.size) {}
    virtual ~ArraySequence() { delete data; }

    T GetFirst() const override {
        if (size == 0) throw std::out_of_range("Sequence is empty");
        return data->Get(0);
    }
    T GetLast() const override {
        if (size == 0) throw std::out_of_range("Sequence is empty");
        return data->Get(size - 1);
    }
    T Get(size_t index) const override {
        if (index >= size) throw std::out_of_range("Index out of range");
        return data->Get(index);
    }
    size_t GetLength() const override {
        return size;
    }


    Sequence<T>* appendImpl(const T& elem) override {
        ensureCapacity();
        data->Set(size,elem);
        size++;
        return this;
    }
    Sequence<T>* prependImpl(const T& elem) override {
        size_t oldSize = size;
        ensureCapacity();

        for (int i = static_cast<int>(oldSize) - 1; i >= 0; i--) {
            data->Set(i + 1, data->Get(i));
        }

        data->Set(0, elem);
        size++;
        return this;
    }
    virtual Sequence<T>* insertAtImpl(const T& elem, size_t index) override {
        size_t oldSize = size;
        ensureCapacity();

        for (int i = static_cast<int>(oldSize) - 1; i >= static_cast<int>(index); i--) {
            data->Set(i + 1, data->Get(i));
        }
        data->Set(index, elem);
        size++;

        return this;
    }
    virtual Sequence<T>* concatImpl(Sequence<T>* other) override {
        size_t oldSize = size;
        size_t addSize = other->GetLength();
        data->Resize(addSize + oldSize);
        for (size_t i = 0; i < addSize; i++) {
            data->Set(i + oldSize, other->Get(i));
        }
        size = addSize + oldSize;

        return this;
    }

    virtual Sequence<T>* createEmpty() const override = 0;
};

#endif // ARRAYSEQUENCE_H