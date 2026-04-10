#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdexcept>
#include "Option.h"
#include "IEnumerator.h"

template<typename T>
class Sequence {
protected:
    virtual Sequence<T>* instance() = 0;

    virtual Sequence<T>* appendImpl(const T& elem) = 0;
    virtual Sequence<T>* prependImpl(const T& elem) = 0;
    virtual Sequence<T>* insertAtImpl(const T& elem, size_t index) = 0;
    virtual Sequence<T>* concatImpl(const Sequence<T>* other) = 0;

    virtual Sequence<T>* createEmpty() const = 0;

public:
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual size_t GetLength() const = 0;

    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual IEnumerator<T>* GetEnumerator() = 0;

    Sequence<T>* Append(const T& elem) {
        return instance()->appendImpl(elem);
    }
    Sequence<T>* Prepend(const T& elem) {
        return instance()->prependImpl(elem);
    }
    Sequence<T>* InsertAt(const T& elem, size_t index) {
        return instance()->insertAtImpl(elem, index);
    }
    Sequence<T>* Concat(Sequence<T>* list) {
        return instance()->concatImpl(list);
    }

    template<typename T2>
    Sequence<T>* Map(T2 (*func)(T)) {
        Sequence<T>* result = createEmpty();
        size_t len = GetLength();
        for (size_t i = 0; i < len; ++i) {
            result = result->Append(func(Get(i)));
        }
        return result;
    }

    Sequence<T>* Where(bool (*pred)(T)) {
        Sequence<T>* result = createEmpty();
        size_t len = GetLength();
        for (size_t i = 0; i < len; ++i) {
            T elem = Get(i);
            if (pred(elem))
                result = result->Append(elem);
        }
        return result;
    }

    template<typename T2>
    T Reduce(T2 (*func)(T2, T)) {
        size_t len = GetLength();
        if (len == 0)
            throw std::out_of_range("Cannot reduce empty sequence");
        T acc = Get(0);
        for (size_t i = 1; i < len; ++i)
            acc = func(acc, Get(i));
        return acc;
    }

    Option<T> GetFirst(bool (*pred)(T) = nullptr) {
        size_t len = GetLength();
        if (pred == nullptr) {
            if (len == 0) return Option<T>();
            return Option<T>(Get(0));
        } else {
            for (size_t i = 0; i < len; ++i) {
                T elem = Get(i);
                if (pred(elem))
                    return Option<T>(elem);
            }
            return Option<T>();
        }
    }

    Option<T> GetLast(bool (*pred)(T) = nullptr) {
        size_t len = GetLength();
        if (pred == nullptr) {
            if (len == 0) return Option<T>();
            return Option<T>(Get(len - 1));
        } else {
            for (size_t i = len; i-- > 0; ) {
                T elem = Get(i);
                if (pred(elem))
                    return Option<T>(elem);
            }
            return Option<T>();
        }
    }

    virtual ~Sequence() = default;
};

#endif