#ifndef MYQUEUE_H
#define MYQUEUE_H

#include "MyDeque.h"

template<class T>
class MyQueue {
private:
	MyDeque<T> my_deque;
public:
	MyQueue() {

	}
	//解决浅拷贝问题
	MyQueue(const MyQueue<T>& q) {
		my_deque = q.my_deque;
	}
	~MyQueue() {

	}
	void operator=(const MyQueue<T>& q) {
		my_deque = q.my_deque;
	}
	void push(T value) {
		my_deque.push_back(value);
	}
	const T& front() {
		return my_deque.front();
	}
	void pop() {
		my_deque.pop_front();
	}
	int size() {
		return my_deque.size();
	}
	bool empty() {
		return my_deque.empty();
	}
	const T& operator[](size_t index) {
		return my_deque[index];
	}
};

#endif
