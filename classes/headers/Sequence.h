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
    virtual Sequence<T>* concatImpl(const Sequence<T>& other) = 0;
    virtual Sequence<T>* delImpl(size_t index) = 0;

    virtual Sequence<T>* createEmpty() const = 0;

public:
    virtual T GetFirst() const = 0;
    virtual T GetLast() const = 0;
    virtual size_t GetLength() const = 0;

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
    Sequence<T>* Concat(const Sequence<T>& list) {
        return instance()->concatImpl(list);
    }
    Sequence<T>* Del(size_t index) {
        return instance()->delImpl(index);
    }

    template<typename T2>
    Sequence<T>* Map(T2 (&func)(T)) {
        Sequence<T>* result = createEmpty();
        auto enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            result = result->Append(func(enumerator->Current()));
        }
        return result;
    }

    Sequence<T>* Where(bool (&func)(T)) {
        Sequence<T>* result = createEmpty();
        auto enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            if (func(enumerator->Current())) {
                result = result->Append(enumerator->Current());
            }
        }
        return result;
    }

    template<typename T2>
    T Reduce(T2 (&func)(T2, T)) {
        size_t len = GetLength();
        if (len == 0)
            throw std::out_of_range("Cannot reduce empty sequence");
        auto enumerator = this->GetEnumerator();
        enumerator->MoveNext();
        T acc = enumerator->Current();
        while (enumerator->MoveNext()) {
            acc = func(acc, enumerator->Current());
        }
        return acc;
    }

    Option<T> GetFirstP(bool (&func)(T)) {
        auto enumerator = this->GetEnumerator();
        while (enumerator->MoveNext()) {
            T elem = enumerator->Current();
            if (func(elem)) return Option<T>(elem);
        }
        return Option<T>();
    }

    Option<T> GetLastP(bool (&func)(T)) {
        auto enumerator = this->GetEnumerator();
        T res{};
        bool flag = false;
        while (enumerator->MoveNext()) {
            T elem = enumerator->Current();
            if (func(elem)) {
                res = elem;
                flag = true;
            }
        }
        if (flag)
            return Option<T>(res);
        return Option<T>();
    }

    virtual ~Sequence() = default;

    // static Sequence<Sequence<T>*>* zip(const Sequence<Sequence<T>*>& lists) {  // Ссылка!
    //     if (lists.GetLength() == 0) {  // GetLength() вместо nullptr!
    //         return lists.createEmpty();
    //     }
    //     Sequence<Sequence<T>*>* result = lists.GetSubsequence(0, 0);
    //
    //     size_t len_new_lists = lists.GetLength();
    //     auto enumerator = lists.GetEnumerator();
    //     enumerator->MoveNext();
    //     size_t count_new_lists = enumerator->Current()->GetLength();
    //     while (enumerator->MoveNext()) {
    //         count_new_lists = std::min(count_new_lists, enumerator->Current()->GetLength());
    //     }
    //
    //     for (size_t col = 0; col < count_new_lists; col++) {
    //         Sequence<T>* temp = lists.GetSubsequence(0, 0);
    //         auto enumerator_row = lists.GetEnumerator();
    //         size_t row = 0;
    //         while (enumerator_row->MoveNext() && row < len_new_lists) {
    //             T value = lists.Get(row)->Get(col);
    //             temp = temp->Append(value);
    //             row++;
    //         }
    //         result = result->Append(temp);
    //     }
    //     return result;
    // }
    //
    // static Sequence<Sequence<void*>*>* unzip(const Sequence<Sequence<void*>*>& tuples) {  // Ссылка!
    //     if (tuples.GetLength() == 0) {
    //         return tuples.createEmpty();
    //     }
    //
    //     size_t num_tuples = tuples.GetLength();
    //     size_t num_fields = tuples.GetFirst()->GetLength();  // Используем GetFirst()
    //
    //     Sequence<Sequence<void*>*>* result = tuples.createEmpty();
    //
    //     for (size_t field = 0; field < num_fields; field++) {
    //         Sequence<void*>* column = tuples.GetFirst()->GetSubsequence(0, 0)->createEmpty();
    //
    //         for (size_t tuple_idx = 0; tuple_idx < num_tuples; tuple_idx++) {
    //             void* value = tuples.Get(tuple_idx)->Get(field);
    //             column = column->Append(value);
    //         }
    //         result = result->Append(column);
    //     }
    //     return result;
    // }
};

#endif