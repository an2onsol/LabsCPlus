#include <iostream>

template <typename T>
class Vector {
private:
    size_t s;
    size_t capacity;
    T* curVector;

public:
    Vector() {
        s = 0;
        capacity = 2;
        curVector = new T[capacity];
    }

    Vector(size_t size) {
        s = size;
        capacity = s * 2;
        curVector = new T[capacity];
    }

    Vector(const Vector& v) {
        s = v.s;
        capacity = v.capacity;
        curVector = new T[capacity];
        for (int i = 0; i < s; i++) {
            curVector[i] = v.curVector[i];
        }
    }
    Vector(const std::initializer_list<T>& list) :
        Vector(list.size()) {
        size_t i = 0;
        for (T value : list) {
            curVector[i++] = value;
        }
    }

    ~Vector() {
        delete[] curVector;
    }

    void push_back(T a) {
        
        if (s == capacity) {
            capacity *= 2;
            T* newVector = new T[capacity];

            for (int i = 0; i < s; i++) {
                newVector[i] = curVector[i];
            }
            delete[] curVector;
            curVector = newVector;
        }
        curVector[s] = a;
        s++;
    }

    void pop_back() {
        if (s > 0) {
            s--;
            T* newVector = new T[capacity];
            for (int i = 0; i < s; i++) {
                newVector[i] = curVector[i];

            }
            delete[] curVector;
            curVector = newVector;
        }
        else {
            throw std::out_of_range("");
        }
    }

    bool empty() {
        return s == 0;
    }

    size_t size() {
        return s;
    }

    void resize(size_t n) {
        size_t temp = s;
        s = n;
        capacity = s * 2;
        T* newVector = new T[capacity];
        for (int i = 0; i < n; i++) {
            if (i < temp) {
                newVector[i] = curVector[i];
            }
            else {
                newVector[i] = 0;
            }
        }
        delete[] curVector;
        curVector = newVector;
    }

    Vector& operator=(Vector& v) {
        s = v.s;
        capacity = v.capacity;
        curVector = new T[capacity];
        for (int i = 0; i < s; i++) {
            curVector[i] = v.curVector[i];
        }
        return *this;
    }
    T& operator[](const size_t i) {
        if (i < 0 || i > s) {
            throw std::out_of_range("");
        }
        return curVector[i];
    }
    class Iterator : std::iterator<std::random_access_iterator_tag, T> {
        T* vectorPtr;
    public:
        Iterator() {
            vectorPtr = nullptr;
        }
        Iterator(const Iterator& it) {
            vectorPtr = it.vectorPtr;
        }
        Iterator(T* vptr) {
            vectorPtr = vptr;
        }
        bool operator!=(const Iterator& it) {
            return vectorPtr != it.vectorPtr;
        }
        bool operator==(const Iterator& it) {
            return vectorPtr == it.vectorPtr;
        }
        Iterator& operator=(const Iterator& it) {
            vectorPtr = it.vectorPtr;
            return *this;
        }
        Iterator operator+=(int value) {
            vectorPtr += value;
            return *this;
        }
        Iterator operator-=(int value) {
            vectorPtr -= value;
            return *this;
        }
        Iterator operator+(int value) {
            Iterator it = *this;
            it.vectorPtr += value;
            return it;
        }
        Iterator operator-(int value) {
            Iterator it = *this;
            it.vectorPtr -= value;
            return it;
        }
        Iterator& operator++() {
            vectorPtr++;
            return *this;
        }
        Iterator& operator--() {
            vectorPtr--;
            return *this;
        }
        T operator*() {
            return *vectorPtr;
        }
        T* operator->() {
            return vectorPtr;
        }
        T operator[](int value) {
            return *(vectorPtr + value);
        }
        bool operator<(const Iterator& it) {
            return *vectorPtr < *it.vectorPtr;
        }
        bool operator>(const Iterator& it) {
            return *vectorPtr > *it.vectorPtr;
        }
        bool operator<=(const Iterator& it) {
            return *vectorPtr <= *it.vectorPtr;
        }
        bool operator>=(const Iterator& it) {
            return *vectorPtr >= *it.vectorPtr;
        }
        
    };

    Iterator begin() {
        return Iterator(curVector);
    }
    Iterator end() {
        return Iterator(curVector + s);
    }
};
int main()
{
    Vector<int> V;
    V.push_back(1);
    V.push_back(2);
    V.pop_back();
    V.push_back(3);
    for (int i = 0; i < V.size(); i++) {
        std::cout << V[i];
    }
    return 0;
}

