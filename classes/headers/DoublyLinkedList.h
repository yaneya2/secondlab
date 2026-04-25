#ifndef SECONDLAB_DOUBLYLINKEDLIST_H
#define SECONDLAB_DOUBLYLINKEDLIST_H

#include <stdexcept>
#include <utility>
#include "IEnumerator.h"

template<typename T>
class DoublyLinkedList {
private:
    struct Node {
        Node *prev;
        Node *next;
        T data;

        Node() : prev(nullptr), next(nullptr), data(T()) {
        }

        explicit Node(const T &value) : prev(nullptr), next(nullptr), data(value) {
        }

        ~Node() = default;
    };

    void swap(DoublyLinkedList &other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(size, other.size);
    }

    class LinkedListEnumerator : public IEnumerator<T> {
    private:
        const DoublyLinkedList<T> &list;
        Node *current;

    public:
        explicit LinkedListEnumerator(const DoublyLinkedList<T> &lst)
            : list(lst), current(nullptr) {
        }

        bool MoveNext() override {
            if (current == nullptr)
                current = list.head;
            else
                current = current->next;
            return current != nullptr;
        }

        T Current() const override {
            if (current == nullptr)
                throw std::out_of_range("Enumerator out of range");
            return current->data;
        }

        void Reset() override {
            current = nullptr;
        }
    };

    Node *head;
    Node *tail;
    size_t size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {
    }

    DoublyLinkedList(const T *items, size_t count) : DoublyLinkedList() {
        if (count == 0) return;

        head = new Node(items[0]);
        tail = head;
        for (size_t i = 1; i < count; ++i) {
            Node *newNode = new Node(items[i]);
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        size = count;
    }

    DoublyLinkedList(const DoublyLinkedList &other) : DoublyLinkedList() {
        if (other.size == 0) return;

        head = new Node(other.head->data);
        tail = head;
        Node *current = other.head->next;
        while (current != nullptr) {
            Node *newNode = new Node(current->data);
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
            current = current->next;
        }
        size = other.size;
    }

    DoublyLinkedList &operator=(const DoublyLinkedList &other) {
        if (this != &other) {
            DoublyLinkedList temp(other);
            swap(temp);
        }
        return *this;
    }

    DoublyLinkedList(DoublyLinkedList &&other) noexcept
        : head(other.head), tail(other.tail), size(other.size) {
        other.head = nullptr;
        other.tail = nullptr;
        other.size = 0;
    }

    DoublyLinkedList &operator=(DoublyLinkedList &&other) noexcept {
        if (this != &other) {
            clear();
            head = other.head;
            tail = other.tail;
            size = other.size;
            other.head = other.tail = nullptr;
            other.size = 0;
        }
        return *this;
    }

    ~DoublyLinkedList() {
        clear();
    }

    T GetFirst() const {
        if (size == 0)
            throw std::out_of_range("list is empty");
        return head->data;
    }

    T GetLast() const {
        if (size == 0)
            throw std::out_of_range("list is empty");
        return tail->data;
    }

    T Get(size_t index) const {
        if (index >= size)
            throw std::out_of_range("IndexOutOfRange");
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
            current = current->next;
        return current->data;
    }

    size_t GetSize() const {
        return size;
    }

    void Del(size_t index) {
        if (index >= size)
            throw std::out_of_range("IndexOutOfRange");

        Node *toDelete = nullptr;
        if (size == 1) {
            toDelete = head;
            head = tail = nullptr;
        } else if (index == 0) {
            toDelete = head;
            head = head->next;
            head->prev = nullptr;
        } else if (index == size - 1) {
            toDelete = tail;
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            Node *current = head;
            for (size_t i = 0; i < index; ++i)
                current = current->next;
            toDelete = current;
            current->prev->next = current->next;
            current->next->prev = current->prev;
        }

        delete toDelete;
        --size;
    }

    void Append(const T &item) {
        Node *newNode = new Node(item);
        if (size == 0) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++size;
    }

    void Prepend(const T &item) {
        Node *newNode = new Node(item);
        if (size == 0) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++size;
    }

    void InsertAt(size_t index, const T &item) {
        if (index > size)
            throw std::out_of_range("IndexOutOfRange");

        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == size) {
            Append(item);
            return;
        }

        Node *newNode = new Node(item);
        Node *current = head;
        for (size_t i = 0; i < index; ++i)
            current = current->next;

        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
        ++size;
    }

    DoublyLinkedList<T> *GetSubList(size_t startIndex, size_t endIndex) const {
        if (startIndex >= size || endIndex >= size || startIndex > endIndex)
            throw std::out_of_range("IndexOutOfRange");

        Node *current = head;
        for (size_t i = 0; i < startIndex; ++i)
            current = current->next;

        auto *newList = new DoublyLinkedList<T>();
        for (size_t i = startIndex; i <= endIndex; ++i) {
            newList->Append(current->data);
            current = current->next;
        }
        return newList;
    }

    DoublyLinkedList<T> *Concat(const DoublyLinkedList<T> &list) {
        auto *newList = new DoublyLinkedList<T>(*this);
        Node *current = list.head;
        for (size_t i = 0; i < list.size; ++i) {
            newList->Append(current->data);
            current = current->next;
        }
        return newList;
    }

    IEnumerator<T> *GetEnumerator() const {
        return new LinkedListEnumerator(this);
    }

private:
    void clear() noexcept {
        Node *current = head;
        while (current != nullptr) {
            Node *next = current->next;
            delete current;
            current = next;
        }
        head = tail = nullptr;
        size = 0;
    }
};

#endif //SECONDLAB_DOUBLYLINKEDLIST_H
