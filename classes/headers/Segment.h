#ifndef SECONDLAB_SEGMENT_H
#define SECONDLAB_SEGMENT_H

#include "DynamicArray.h"
#include <stdexcept>

template<class T>
class Segment {
private:
    DynamicArray<T> *data;
    size_t head;
    size_t tail;
    size_t size;

    class SegmentEnumerator : public IEnumerator<T> {
    private:
        const Segment<T> *segment;
        size_t currentIndex;

    public:
        explicit SegmentEnumerator(const Segment<T> *seg)
            : segment(seg), currentIndex(seg->head) {
        }

        bool MoveNext() override {
            if (segment->IsEmpty()) return false;
            if (currentIndex <= segment->tail) {
                ++currentIndex;
                return true;
            }
            return false;
        }

        T Current() const override {
            if (currentIndex < segment->head || currentIndex > segment->tail + 1 || segment->IsEmpty()) {
                throw std::out_of_range("SegmentEnumerator out of range");
            }
            return segment->Get(currentIndex - 1);
        }

        void Reset() override {
            currentIndex = segment->head - 1;
        }
    };

public:
    explicit Segment(size_t capacity)
        : data(new DynamicArray<T>(capacity)), head(0), tail(0), size(0) {
    }

    Segment(size_t capacity, size_t headIdx, size_t tailIdx)
        : data(new DynamicArray<T>(capacity)), head(headIdx), tail(tailIdx), size(0) {
    }

    ~Segment() {
        delete data;
    }

    Segment(const Segment &other)
        : data(new DynamicArray<T>(*other.data)), head(other.head), tail(other.tail), size(other.size) {
    }

    Segment &operator=(const Segment &other) {
        if (this != &other) {
            delete data;
            data = new DynamicArray<T>(*other.data);
            head = other.head;
            tail = other.tail;
            size = other.size;
        }
        return *this;
    }

    Segment(Segment &&other) noexcept
        : data(other.data), head(other.head), tail(other.tail) {
        other.data = nullptr;
        other.head = 0;
        other.tail = 0;
        other.size = 0;
    }

    Segment &operator=(Segment &&other) noexcept {
        if (this != &other) {
            delete data;
            data = other.data;
            head = other.head;
            tail = other.tail;
            other.data = nullptr;
            other.head = 0;
            other.tail = 0;
            other.size = 0;
        }
        return *this;
    }

    size_t GetHead() const {
        return head;
    }

    size_t GetTail() const {
        return tail;
    }

    T Get(size_t index) const {
        return data->Get(index);
    }

    size_t GetSize() const {
        return size;
    }

    void SetHead(size_t newHead) {
        if (newHead >= data->GetSize()) {
            throw std::out_of_range("Segment SetHead: index out of range");
        }
        head = newHead;
    }

    void SetTail(size_t newTail) {
        if (newTail >= data->GetSize()) {
            throw std::out_of_range("Segment SetTail: index out of range");
        }
        tail = newTail;
    }

    bool IsHeadNotZero() const {
        return head != 0;
    }

    bool IsTailNotAtSize() const {
        return tail != data->GetSize() - 1;
    }

    bool IsEmpty() const {
        return size == 0;
    }

    bool Append(const T &value) {
        if (tail >= data->GetSize()) {
            return false;
        }
        if (GetSize() == 0) {
            data->Set(tail, value);
            size++;
            return true;
        }
        tail++;
        size++;
        data->Set(tail, value);
        return true;
    }

    bool Prepend(const T &value) {
        if (head == 0) {
            return false;
        }
        if (GetSize() == 0) {
            data->Set(head, value);
            size++;
            return true;
        }
        head--;
        size++;
        data->Set(head, value);
        return true;
    }

    T peekFirst() const {
        return data->Get(head);
    }

    T peekLast() const {
        return data->Get(tail);
    }

    bool PopFirst(T &outValue) {
        if (head >= tail || head >= data->GetSize()) {
            return false;
        }
        outValue = data->Get(head);
        ++head;
        return true;
    }

    bool PopLast(T &outValue) {
        if (tail <= head || tail == 0) {
            return false;
        }
        --tail;
        outValue = data->Get(tail);
        return true;
    }

    IEnumerator<T> *GetEnumerator() const {
        return new SegmentEnumerator(this);
    }
};

#endif
