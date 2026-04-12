#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <stdexcept>
#include "Option.h"
#include "IEnumerator.h"

template<typename T>
class MutableArraySequence;

template<typename T>
class Sequence {
protected:
    virtual Sequence<T>* instance() = 0;

    virtual Sequence<T>* appendImpl(const T& elem) = 0;
    virtual Sequence<T>* prependImpl(const T& elem) = 0;
    virtual Sequence<T>* insertAtImpl(const T& elem, size_t index) = 0;
    virtual Sequence<T>* concatImpl(const Sequence<T>* other) = 0;//чтобы UI нормально работад

    virtual Sequence<T>* createEmpty() const = 0;

public:
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual size_t GetLength() const = 0;
    virtual T Get(size_t index) const = 0;
    virtual void Del(size_t index) = 0;

    virtual Sequence<T>* GetSubsequence(size_t startIndex, size_t endIndex) const = 0;
    virtual IEnumerator<T>* GetEnumerator() const = 0;

    Sequence<T>* Append(const T& elem) {
        return instance()->appendImpl(elem);
    }
    Sequence<T>* Prepend(const T& elem) {
        return instance()->prependImpl(elem);
    }
    Sequence<T>* InsertAt(const T& elem, size_t index) {
        return instance()->insertAtImpl(elem, index);
    }
    Sequence<T>* Concat(const Sequence<T>* list) {
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

    // //еще раз проверить
    // static Sequence<Sequence<void*>*>* zipN(const Sequence<Sequence<void*>*>* lists) {
    //     Sequence<Sequence<void*>*>* temp = lists->GetSubsequence(0, 0);
    //     Sequence<Sequence<void*>*>* result = temp->createEmpty();
    //     if (lists->GetLength() == 0) {
    //         delete temp;
    //         return result;
    //     }
    //
    //     size_t num_lists = lists->GetLength();
    //     size_t len = lists->Get(0)->GetLength();
    //
    //     for (size_t i = 1; i < num_lists; i++) {
    //         size_t current_len = lists->Get(i)->GetLength();
    //         if (current_len < len) {
    //             len = current_len;
    //         }
    //     }
    //
    //     for (size_t col = 0; col < len; col++) {
    //         Sequence<void*>* column_temp = lists->Get(0)->GetSubsequence(0, 0);
    //         Sequence<void*>* column = column_temp->createEmpty();
    //         delete column_temp;
    //
    //         for (size_t row = 0; row < num_lists; row++) {
    //             void* value = lists->Get(row)->Get(col);
    //             column = column->Append(value);
    //         }
    //         result = result->Append(column);
    //     }
    //     return result;
    // }
    //
    // static Sequence<Sequence<void*>*>* unzip(const Sequence<Sequence<void*>*>* tuples) {
    //     Sequence<Sequence<void*>*>* temp = tuples->GetSubsequence(0, 0);
    //     Sequence<Sequence<void*>*>* result = temp->createEmpty();
    //
    //     if (tuples->GetLength() == 0) {
    //         delete temp;
    //         return result;
    //     }
    //
    //     size_t num_tuples = tuples->GetLength();
    //
    //     size_t num_fields = tuples->Get(0)->GetLength();
    //
    //     for (size_t field = 0; field < num_fields; field++) {
    //         Sequence<void*>* column_temp = tuples->Get(0)->GetSubsequence(0, 0);
    //         Sequence<void*>* column = column_temp->createEmpty();
    //         delete column_temp;
    //
    //         for (size_t tuple_idx = 0; tuple_idx < num_tuples; tuple_idx++) {
    //             void* value = tuples->Get(tuple_idx)->Get(field);
    //             column = column->Append(value);
    //         }
    //         result = result->Append(column);
    //     }
    //
    //     delete temp;
    //     return result;
    // }
};

#endif