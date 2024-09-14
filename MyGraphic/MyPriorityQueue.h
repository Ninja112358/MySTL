#ifndef MYPRIORITYQUEUE_H
#define MYPRIORITYQUEUE_H

#include "MyVector.h"

template<class T>
struct MyLess {
    bool operator()(const T& Left, const T& Right) {
        return Left < Right;
    }
};

template<class T>
struct MyGreater {
    bool operator()(const T& Left, const T& Right) {
        return Left > Right;
    }
};

template<class T, class Container = MyVector<T>, class Pr = MyLess<T>>
class MyPriorityQueue {
private:
    Pr comp{};
    Container heap;
    void HeapInsert(size_t index);
    void Heapify(size_t index, size_t heapSize);
    //通过第三变量去交换两个变量的值适用于所有数据类型
    //但是如果不通过第三变量，用位运算，加法，乘法等，他的数据类型是有局限性的
    void swap(T& a, T& b) {
        T temp = a;
        a = b;
        b = temp;
    }
public:
    MyPriorityQueue();
    ~MyPriorityQueue();
    void push(T value);
    const T top();
    void pop();
    bool empty();
};

template<class T, class Container, class Pr>
void MyPriorityQueue<T, Container, Pr>::HeapInsert(size_t index) {
    while (comp(heap[index], heap[index == 0 ? 0 : (index - 1) / 2])) {
        swap(heap[index], heap[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

template<class T, class Container, class Pr>
void MyPriorityQueue<T, Container, Pr>::Heapify(size_t index, size_t heapSize) {
    size_t left = 2 * index + 1;
    while (left < heapSize) {
        size_t largest = left + 1 < heapSize && comp(heap[left + 1], heap[left]) ? left + 1 : left;
        if (comp(heap[largest], heap[index])) {
            swap(heap[largest], heap[index]);
            index = largest;
            left = 2 * index + 1;
        }
        else
            return;
    }
}
template<class T, class Container, class Pr>
MyPriorityQueue<T, Container, Pr>::MyPriorityQueue() {

}

template<class T, class Container, class Pr>
MyPriorityQueue<T, Container, Pr>::~MyPriorityQueue() {

}

template<class T, class Container, class Pr>
void MyPriorityQueue<T, Container, Pr>::push(T value) {
    heap.push_back(value);
    HeapInsert(heap.size() - 1);
}

template<class T, class Container, class Pr>
const T MyPriorityQueue<T, Container, Pr>::top() {
    return heap[0];
}

template<class T, class Container, class Pr>
void MyPriorityQueue<T, Container, Pr>::pop() {
    size_t heap_size = heap.size();
    swap(heap[0], heap[heap_size - 1]);
    heap.pop_back();
    Heapify(0, heap.size());
}

template<class T, class Container, class Pr>
bool MyPriorityQueue<T, Container, Pr>::empty() {
    return heap.size() == 0;
}

#endif