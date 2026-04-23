#ifndef SECONDLAB_SEGMENTEDDEQUE_H
#define SECONDLAB_SEGMENTEDDEQUE_H

#include "DoublyLinkedList.h"
#include "Segment.h"
#include <iostream>

template<typename T>
class SegmentedDeque : public Sequence<T> {
private:
    DoublyLinkedList<Segment<T> *> *segments;
    size_t segmentLength;
    size_t length;

    class SegmentedDequeIterator : public IEnumerator<T> {
    private:
        DoublyLinkedList<Segment<T> *> *segmentsList;
        IEnumerator<Segment<T> *> *segListEnum;
        IEnumerator<T> *segEnum;
        T currentValue;
        bool hasCurrent;

    public:
        explicit SegmentedDequeIterator(DoublyLinkedList<Segment<T> *> *list)
            : segmentsList(list), segListEnum(nullptr), segEnum(nullptr), hasCurrent(false) {
        }

        ~SegmentedDequeIterator() {
            delete segListEnum;
            delete segEnum;
        }

        bool MoveNext() override {
            if (!segListEnum) {
                segListEnum = segmentsList->GetEnumerator();
            }

            if (segEnum) {
                if (segEnum->MoveNext()) {
                    currentValue = segEnum->Current();
                    hasCurrent = true;
                    return true;
                }
                delete segEnum;
                segEnum = nullptr;
            }

            while (segListEnum->MoveNext()) {
                Segment<T> *seg = segListEnum->Current();
                segEnum = seg->GetEnumerator();
                if (segEnum->MoveNext()) {
                    currentValue = segEnum->Current();
                    hasCurrent = true;
                    return true;
                }
                delete segEnum;
                segEnum = nullptr;
            }

            hasCurrent = false;
            return false;
        }

        T Current() const override {
            if (!hasCurrent) {
                throw std::out_of_range("SegmentedDequeIterator: iterator is not positioned on a valid element");
            }
            return currentValue;
        }

        void Reset() override {
            delete segListEnum;
            delete segEnum;
            segListEnum = nullptr;
            segEnum = nullptr;
            hasCurrent = false;
        }
    };

public:
    explicit SegmentedDeque(size_t segLen) : segmentLength(segLen), length(0) {
        if (segmentLength < 2) {
            throw std::invalid_argument("SegmentedDeque: segmentLength must be greater than 2");
        }
        segments = new DoublyLinkedList<Segment<T> *>();
        auto *initialSegment = new Segment<T>(segLen);
        size_t mid = segLen / 2;
        initialSegment->SetHead(mid);
        initialSegment->SetTail(mid);
        segments->Append(initialSegment);
    }

    IEnumerator<T> *GetEnumerator() const override {
        return new SegmentedDequeIterator(segments);
    }

    Sequence<T> *appendImpl(const T &item) override {
        Segment<T> *lastSeg = segments->GetLast();
        if (lastSeg->IsTailNotAtSize() || lastSeg->IsEmpty()) {
            lastSeg->Append(item);
        } else {
            auto *newSeg = new Segment<T>(segmentLength);
            newSeg->SetHead(0);
            newSeg->SetTail(0);
            newSeg->Append(item);
            segments->Append(newSeg);
        }
        length++;
        return this;
    }

    Sequence<T> *prependImpl(const T &item) {
        Segment<T> *firstSeg = segments->GetFirst();
        if (firstSeg->IsHeadNotZero() || firstSeg->IsEmpty()) {
            firstSeg->Prepend(item);
        } else {
            auto *newSeg = new Segment<T>(segmentLength);
            newSeg->SetHead(segmentLength - 1);
            newSeg->SetTail(segmentLength - 1);
            newSeg->Prepend(item);
            segments->Prepend(newSeg);
        }
        length++;
        return this;
    }

    size_t GetLength() const override {
        return segments->GetSize();
    }

    size_t GetCountElements() const {
        size_t count = 0;
        for (size_t i = 0; i < segments->GetSize(); i++) {
            count += segments->Get(i)->GetSize();
        }
        return count;
    }

    bool IsEmpty() const {
        return length == 0;
    }

    T GetFirst() const override {
        if (IsEmpty()) {
            throw std::out_of_range("SegmentedDeque::PeekFirst: deque is empty");
        }
        Segment<T> *firstSeg = segments->GetFirst();
        return firstSeg->peekFirst();
    }

    T GetLast() const {
        if (IsEmpty()) {
            throw std::out_of_range("SegmentedDeque::PeekLast: deque is empty");
        }
        Segment<T> *lastSeg = segments->GetLast();
        return lastSeg->peekLast();
    }


    T PopFirst() {
        if (IsEmpty()) {
            throw std::out_of_range("SegmentedDeque::PopFirst: deque is empty");
        }
        Segment<T> *firstSeg = segments->GetFirst();
        T res{};
        firstSeg->PopFirst(res);
        return res;
    }

    int FindSubsequence(const Sequence<T>& subsequence) const {
        size_t subLen = subsequence.GetSize();
        if (subLen == 0) return 0;
        if (subLen > length) return -1;

        IEnumerator<T>* it = this->GetEnumerator();
        size_t currentIdx = 0;
        size_t matchStart = 0;
        size_t matchPos = 0;

        while (it->MoveNext()) {
            if (it->Current() == subsequence.Get(matchPos)) {
                if (matchPos == 0) {
                    matchStart = currentIdx;
                }
                matchPos++;

                if (matchPos == subLen) {
                    delete it;
                    return static_cast<int>(matchStart);
                }
            } else {
                if (matchPos > 0) {
                    matchPos = 0;
                    it->Reset();
                    for (size_t i = 0; i <= matchStart; ++i) {
                        it->MoveNext();
                    }
                    currentIdx = matchStart;
                }
            }
            currentIdx++;
        }

        delete it;
        return -1;
    }

    friend std::ostream &operator<<(std::ostream &os, const SegmentedDeque<T> &deque) {
        os << "{ ";
        for (size_t i = 0; i < deque.segments->GetSize(); ++i) {
            Segment<T> *seg = deque.segments->Get(i);
            size_t cap = deque.segmentLength;
            os << "[";
            for (size_t j = 0; j < cap; ++j) {
                if (j > 0) os << ", ";
                if (j < seg->GetHead() || j > seg->GetTail()) {
                    os << "_";
                } else {
                    os << seg->Get(j);
                }
            }
            os << "]";
            if (i < deque.segments->GetSize() - 1) {
                os << " ";
            }
        }
        os << " }";
        return os;
    }

protected:
    Sequence<T> *instance() override {
        return this;
    }

    Sequence<T> *insertAtImpl(const T &elem, size_t index) override {
        if (index > length) {
            throw std::out_of_range("insertAtImpl: index out of range");
        }

        auto *result = new SegmentedDeque<T>(segmentLength);

        IEnumerator<T> *it = this->GetEnumerator();
        size_t currentIdx = 0;

        while (it->MoveNext()) {
            if (currentIdx == index) {
                result->Append(elem);
            }
            result->Append(it->Current());
            ++currentIdx;
        }

        if (index == length) {
            result->Append(elem);
        }

        delete it;
        return result;
    }

    Sequence<T> *concatImpl(const Sequence<T> &other) override {
        auto *result = new SegmentedDeque<T>(this->segmentLength);

        IEnumerator<T> *iterThis = this->GetEnumerator();
        IEnumerator<T> *iterOther = other.GetEnumerator();

        while (iterThis->MoveNext()) {
            result->Append(iterThis->Current());
        }

        while (iterOther->MoveNext()) {
            result->Append(iterOther->Current());
        }

        delete iterThis;
        delete iterOther;

        return result;
    }

    Sequence<T> *delImpl(size_t index) override {
        if (index >= length) {
            throw std::out_of_range("delImpl: index out of range");
        }

        auto *result = new SegmentedDeque<T>(segmentLength);
        IEnumerator<T> *it = this->GetEnumerator();
        size_t currentIdx = 0;

        while (it->MoveNext()) {
            if (currentIdx != index) {
                result->Append(it->Current());
            }
            ++currentIdx;
        }

        delete it;
        return result;
    }

    Sequence<T> *createEmpty() const override {
        return new SegmentedDeque<T>(segmentLength);
    }

public:
    Sequence<T> *GetSubsequence(size_t startIndex, size_t endIndex) const override {
        if (startIndex > endIndex || endIndex >= length) {
            throw std::out_of_range("GetSubsequence: invalid indices");
        }

        auto *result = new SegmentedDeque<T>(segmentLength);

        IEnumerator<T> *enumerator = this->GetEnumerator();

        size_t currentIndex = 0;
        while (enumerator->MoveNext()) {
            if (currentIndex >= startIndex && currentIndex <= endIndex) {
                result->appendImpl(enumerator->Current());
            }
            if (currentIndex >= endIndex) {
                break;
            }
            ++currentIndex;
        }

        delete enumerator;

        return result;
    }

    ~SegmentedDeque() override {
        delete segments;
    }
};

#endif
