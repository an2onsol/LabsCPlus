#include <iostream>

template <typename T>
class Deque {
    T** arrayptr;
    struct array {
        int borderElement;
        int arrayIndex;
    };
    const size_t blockCapacity = 10;
    size_t size;
    size_t capacity;
    array head;
    array tail;
public:
    Deque() {
        capacity = blockCapacity;
        size = 0;
        arrayptr = new T * [1];
        arrayptr[0] = new T[blockCapacity];
        head.borderElement = 0;
        head.arrayIndex = 0;
        tail.arrayIndex = 0;
        tail.borderElement = 1;
    }

    Deque(const std::initializer_list<T>& list) {
        capacity = blockCapacity;
        size = 0;
        arrayptr = new T * [1];
        arrayptr[0] = new T[blockCapacity];
        head.borderElement = 0;
        head.arrayIndex = 0;
        tail.arrayIndex = 0;
        tail.borderElement = 1;
        for (auto& it : list) {
            this->push_back(it);
        }
    }

    ~Deque() {
        for (int i = 0; i < capacity / blockCapacity; i++) {
            delete[] arrayptr[i];
        }
        delete[] arrayptr;
    }

    void resize() {
        size_t temp = capacity;
        capacity *= 2;
        T** tempA = new T * [capacity / blockCapacity];
        for (int i = 0; i < temp / blockCapacity; i++) {
            tempA[i] = arrayptr[head.arrayIndex + i];
        }
        for (int i = temp / blockCapacity; i < capacity / blockCapacity; i++) {
            tempA[i] = new T[blockCapacity];
        }
        delete[] arrayptr;
        arrayptr = tempA;
    }

    void push_back(T value) {
        size++;
        arrayptr[tail.arrayIndex][tail.borderElement] = value; 

        if (tail.borderElement < blockCapacity - 1) {
            tail.borderElement++; 
        }
        else if (head.arrayIndex != tail.arrayIndex + 1 && tail.arrayIndex != (capacity / blockCapacity) - 1) {
            tail.arrayIndex++;
            tail.borderElement = 0;
        }
        else if (head.arrayIndex != 0 && tail.arrayIndex == (capacity / blockCapacity) - 1) {
            tail.arrayIndex = 0;
            tail.borderElement = 0;
        }
        else {
            resize();
            tail.arrayIndex++;
            tail.borderElement = 0;
        }
    }

    void pop_back() {
        if (size == 0) {
            throw std::exception("");
        }
        if (tail.borderElement > 0) {
            tail.borderElement -= 1;
        }
        else if (tail.arrayIndex == 0) {
            tail.arrayIndex = (capacity / blockCapacity) - 1;
            tail.borderElement = blockCapacity - 1;
        }
        else {
            tail.arrayIndex--;
            tail.borderElement = blockCapacity - 1;
        }
        size--;
    }
    void push_front(T value) {
        size++;
        arrayptr[head.arrayIndex][head.borderElement] = value;

        if (head.borderElement >= 1) {
            head.borderElement--;
        }
        else if (head.arrayIndex == 0 && tail.arrayIndex != (capacity / blockCapacity) - 1) {
            head.arrayIndex = (capacity / blockCapacity) - 1;
            head.borderElement = blockCapacity - 1;
        }
        else if (head.arrayIndex > 1 && head.arrayIndex - 1 != tail.arrayIndex) {
            head.arrayIndex--;
            head.borderElement = blockCapacity - 1;
        }
        else {
            resize();
            head.arrayIndex = (capacity / blockCapacity) - 1;
            head.borderElement = blockCapacity - 1;
        }
    }

    void pop_front() {
        if (size == 0) {
            throw std::exception("");
        }
        size--;
        if (head.borderElement < blockCapacity - 1) {
            head.borderElement++;
        }
        else if (head.arrayIndex == (capacity / blockCapacity) - 1) {
            head.arrayIndex = 0;
            head.borderElement = 0;
        }
        else {
            head.arrayIndex++;
            head.borderElement = 0;
        }
    }

    void clear() {
        for (int i = 0; i < capacity / blockCapacity; i++) {
            delete[] arrayptr[i];
        }
        delete[] arrayptr;

        capacity = blockCapacity;
        size = 0;
        arrayptr = new T * [1];
        arrayptr[0] = new T[blockCapacity];
        head.borderElement = 0;
        head.arrayIndex = 0;
        tail.arrayIndex = 0;
        tail.borderElement = 1;
    }
    bool empty() const {
        return size > 0;
    }
    size_t Size() const {
        return size;
    }
    class Iterator : std::iterator<std::random_access_iterator_tag, T> {
    private:
        T** data;
        size_t* cap;
        size_t indexA;
        size_t indexE;
        size_t blockCapacity = 10;
    public:
        Iterator(size_t indexA, size_t indexE, T** data, size_t* cap) {
            this->data = data;
            this->indexA = indexA;
            this->cap = cap;
            this->indexE = indexE;
        }
        Iterator(const Iterator& it) {
            this->data = it.data;
            this->indexA = it.indexA;
            this->cap = it.cap;
            this->indexE = it.indexE;
        }
        ~Iterator() {
            data = nullptr;
        }
        Iterator& operator=(const Iterator& it) {
            this->data = it.data;
            this->indexA = it.indexA;
            this->cap = it.cap;
            this->indexE = it.indexE;
            return *this;
        }
        Iterator& operator++() {
            if (indexE + 1 < blockCapacity) {
                indexE++;
            }
            else if (indexA + 1 > (*cap / blockCapacity) - 1) {
                indexA = 0;
                indexE = 0;
            }
            else {
                indexA++;
                indexE = 0;
            }
            return *this;
        }

        Iterator operator++(int) {
            Iterator temp = *this;
            if (indexE + 1 < blockCapacity) {
                indexE++;
            }
            else if (indexA + 1 > (*cap / blockCapacity) - 1) {
                indexA = 0;
                indexE = 0;
            }
            else {
                indexA++;
                indexE = 0;
            }
            return temp;
        }

        Iterator& operator--() {
            if (indexE > 0) {
                indexE--;
            }
            else if (indexA == 0) {
                indexA = (*cap / blockCapacity) - 1;
                indexE = blockCapacity - 1;
            }
            else {
                indexA--;
                indexE = blockCapacity - 1;
            }
            return *this;
        }
        
        Iterator operator--(int) {
            Iterator temp = *this;
            if (indexE > 0) {
                indexE--;
            }
            else if (indexA == 0) {
                indexA = (*cap / blockCapacity) - 1;
                indexE = blockCapacity - 1;
            }
            else {
                indexA--;
                indexE = blockCapacity - 1;
            }
            return temp;
        }

        Iterator operator+(int value) {
            int temp1;
            int temp2;
            temp1 = (indexA + (value / blockCapacity)) % (*cap / blockCapacity);
            temp1 = (temp1 + (indexE + value % blockCapacity) / blockCapacity) % (*cap / blockCapacity);
            temp2 = (indexE + value % blockCapacity) % blockCapacity;
            return Iterator(temp1, temp2, data, (size_t*)&cap);
        }

        Iterator operator-(int value) {
            int temp1;
            int temp2;
            if (indexA * blockCapacity + indexE < value) {
                int temp = *cap - blockCapacity;
                value -= indexA * blockCapacity + indexE;
                temp -= value;
                temp1 = temp / blockCapacity;
                temp2 = temp % blockCapacity;
            }
            else {
                int temp = indexA * blockCapacity + indexE;
                temp -= value;
                temp1 = temp / blockCapacity;
                temp2 = temp % blockCapacity;
            }
            return Iterator(temp1, temp2, data, (size_t*)&cap);
        }

        Iterator operator-=(int value) {
            this->index -= value;
            return *this;
        }

        bool operator==(const Iterator& it) {
            return (this->indexA == it.indexA && this->indexE == it.indexE && this->cap == it.cap && this->data);
        }
        bool operator!=(const Iterator& it) {
            return !(this->indexA == it.indexA && this->indexE == it.indexE && this->cap == it.cap && this->data);
        }
        bool operator<(const Iterator& it) {
            return (this->data[indexA][indexE] < it.data[it.indexA][it.indexE]);
        }
        bool operator<=(const Iterator& it) {
            return (this->data[indexA][indexE] <= it.data[it.indexA][it.indexE]);
        }
        bool operator>(const Iterator& it) {
            return (this->data[indexA][indexE] > it.data[it.indexA][it.indexE]);
        }
        bool operator>=(const Iterator& it) {
            return (this->data[indexA][indexE] >= it.data[it.indexA][it.indexE]);
        }

        T operator*() {
            return data[indexA][indexE];
        }
        T* operator->() {
            return *(data[indexA][indexE]);
        }
        T operator[](size_t index) {
            return *(*this + index);
        }
    };
    Iterator begin() {
            size_t temp1 = head.arrayIndex;
            size_t temp2 = head.borderElement;
            if (head.borderElement + 1 < blockCapacity) {
                temp2++;
            }
            else if (head.arrayIndex + 1 > (capacity / blockCapacity) - 1) {
                temp1 = 0;
                temp2 = 0;
            }
            else {
                temp1++;
                temp2 = 0;
            }
            return Iterator(temp1, temp2, arrayptr, (size_t*)&capacity);
    }
    Iterator end() {
        return Iterator(tail.arrayIndex, tail.borderElement, arrayptr, (size_t*)&capacity);
    }
};

int main() {
    Deque<int> deq = { 1, 2, 3, 4, 5, 6, 7 };
    Deque<int>::Iterator it = deq.begin();
    std::cout << it[2] << std::endl;
    deq.pop_back();
    deq.push_back(44);
    deq.pop_front();
    deq.push_front(55);
    for (int i = 0; i < deq.Size(); i++) {
        std::cout << it[i] << " ";
    }
}
