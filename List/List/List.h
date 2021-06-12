#include <iostream>

template<class T>
class List {
private:
    struct Node {
        int x;
        Node* next;
        Node* prev;
    };
    size_t s;
    Node* first;
    Node* last;
public:
    List() {
        first = nullptr;
        last = nullptr;
        s = 0;
    }
    List(const std::initializer_list<int>& list) {
        s = 0;
        first = nullptr;
        last = nullptr;
        for (auto it : list) {
            push_back(it);
        }
    }
    List(List<T>& list) {
        s = 0;
        first = nullptr;
        last = nullptr;
        for (auto it : list) {
            push_back(it);
        }
    }
    List<T>& operator=(List<T>& list) {
        s = 0;
        first = nullptr;
        last = nullptr;
        for (auto it : list) {
            push_back(it);
        }
        return *this;
    }
    ~List() {
        while (first) {
            last = first->next;
            delete first;
            first = last;
        }
    }
    void push_back(T x) {
        Node* temp = new Node;
        temp->next = nullptr;
        temp->x = x;

        if (first != nullptr) {
            temp->prev = last;
            last->next = temp;
            last = temp;
        }
        else {
            temp->prev = nullptr;
            first = temp;
            last = temp;
        }
        s++;
    }
    void push_front(T x) {
        Node* temp = new Node;
        temp->prev = nullptr;
        temp->x = x;

        if (last != nullptr) {
            temp->next = first;
            first->prev = temp;
            first = temp;
        }
        else {
            temp->next = nullptr;
            first = temp;
            last = temp;
        }
        s++;
    }
    void pop_back() {
        Node* temp = last;
        last = last->prev;
        last->next = nullptr;
        delete temp;
        s--;
    }
    void pop_front() {
        Node* temp = first;
        first = first->next;
        first->prev = nullptr;
        delete temp;
        s--;
    }
    bool empty() const {
        return s == 0;
    }
    size_t size() const {
        return s;
    }
    class Iterator : std::iterator<std::bidirectional_iterator_tag, T> {
    private:
        Node* nodePtr;
    public:
        Iterator() {
            nodePtr = nullptr;
        }
        Iterator(const Iterator& it) {
            nodePtr = it.nodePtr;
        }
        Iterator(Node& it) {
            nodePtr = &it;
        }
        bool operator==(const Iterator& it) {
            return nodePtr == it.nodePtr;
        }
        bool operator!=(const Iterator& it) {
            return nodePtr != it.nodePtr;
        }
        Iterator operator=(const Iterator& it) {
            nodePtr = it.nodePtr;
            return *this;
        }
        Iterator& operator--() {
            nodePtr = nodePtr->prev;
            return *this;
        }
        Iterator& operator++() {
            nodePtr = nodePtr->next;
            return *this;
        }
        Iterator operator++(int) {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        Iterator operator--(int) {
            Iterator temp = *this;
            --(*this);
            return temp;
        }
        T* operator->() {
            return nodePtr;
        }
        T operator*() const {
            return nodePtr->x;
        }
        T& operator*() {
            return nodePtr->x;
        }
        ~Iterator() {
            nodePtr = nullptr;
        }
    };
    Iterator begin() {
        return Iterator(*first);
    }
    Iterator end() {
        return Iterator(*last->next);
    }
};


