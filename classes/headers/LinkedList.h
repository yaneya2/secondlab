#ifndef SECONDLAB_LINKEDLIST_H
#define SECONDLAB_LINKEDLIST_H

#include <stdexcept>
//#include <utility>

template<typename T>
class LinkedList {
private:
    struct Node {
        Node* next;
        T data;

        Node() : next(nullptr), data(T()) {}
        Node(const T& data) : data(data), next(nullptr) {}
        ~Node() = default;
    };

    Node* head;
    Node* tail;
    size_t size;

public:
    template<typename U> friend class LinkedListEnumerator;
    void swap(LinkedList& other) noexcept {
        std::swap(head, other.head);
        std::swap(tail, other.tail);
        std::swap(size, other.size);
    }

    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(const T* items, size_t count) : LinkedList() {
        if (count <= 0) {
            return;
        }
        head = new Node(items[0]);
        tail = head;
        for (size_t i = 1; i < count; i++) {
            tail->next = new Node(items[i]);
            tail = tail->next;
        }
        tail->next = nullptr;
        size = count;
    }

    LinkedList(const LinkedList& other) : LinkedList() {
        if (other.size == 0) {
            return;
        }
        head = new Node(other.head->data);
        tail = head;
        Node* current = other.head->next;
        while (current != nullptr) {
            tail->next = new Node(current->data);
            tail = tail->next;
            current = current->next;
        }
        size = other.size;
    }

    LinkedList& operator=(const LinkedList& other) {
        if (this != &other) {
            LinkedList temp(other);
            swap(temp);
        }
        return *this;
    }

    ~LinkedList() {
        Node* current = head;
        while (current != nullptr) {
            Node* next = current->next;
            delete current;
            current = next;
        }
    }

    T GetFirst() const {
        if (size == 0) {
            throw std::out_of_range("list is empty");
        }
        return head->data;
    }

    T GetLast() const {
        if (size == 0) {
            throw std::out_of_range("list is empty");
        }
        return tail->data;
    }

    T Get(size_t index) const {
        if (index >= size) {
            throw std::out_of_range("IndexOutOfRange");
        }
        Node* current = head;
        for (size_t i = 0; i < index; i++) {
            current = current->next;
        }
        return current->data;
    }

    size_t GetSize() const {
        return size;
    }

    LinkedList<T>* GetSubList(size_t startIndex, size_t endIndex) const {
        if (startIndex >= size || endIndex >= size || startIndex > endIndex) {
            throw std::out_of_range("IndexOutOfRange");
        }
        Node* current = head;
        for (size_t i = 0; i < startIndex; i++) {
            current = current->next;
        }
        LinkedList<T>* subList = new LinkedList<T>();
        for (size_t i = startIndex; i <= endIndex; i++) {
            subList->Append(current->data);
            current = current->next;
        }
        return subList;
    }

    void Append(const T& item) {
        Node* newNode = new Node(item);
        if (size == 0) {
            head = newNode;
            tail = newNode;
            size = 1;
            return;
        }
        tail->next = newNode;
        tail = tail->next;
        size++;
    }

    void Prepend(const T& item) {
        Node* newNode = new Node(item);
        if (size == 0) {
            head = newNode;
            tail = newNode;
            size = 1;
            return;
        }
        newNode->next = head;
        head = newNode;
        size++;
    }

    void InsertAt(size_t index, const T& item) {
        if (index > size) {
            throw std::out_of_range("IndexOutOfRange");
        }
        if (index == 0) {
            this->Prepend(item);
            return;
        }//убрать эту ветку
        if (index == size) {
            this->Append(item);
            return;
        }
        Node* newNode = new Node(item);
        Node* prev = head;
        for (size_t i = 0; i < index - 1; i++) {
            prev = prev->next;
        }
        newNode->next = prev->next;
        prev->next = newNode;
        size++;
    }

    LinkedList<T>* Concat(const LinkedList<T> &list) const {
        if (list == nullptr) {
            throw std::invalid_argument("list is nullptr");
        }
        if (list->GetSize() == 0) {
            return this;
        }
        LinkedList<T>* newList = new LinkedList<T>(*this);//использовать нумератор
        Node* current = list->head;
        for (size_t i = 0; i < list->size; i++) {
            newList->Append(current->data);
            current = current->next;
        }
        return newList;
    }
};

#endif // SECONDLAB_LINKEDLIST_H