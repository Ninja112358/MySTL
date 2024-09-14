#ifndef MYSTACK_H
#define MYSTACK_H

#include "MyDeque.h"

template<class T>
class MyStack {
private:
	MyDeque<T> my_deque;
public:
	MyStack() {

	}
	//解决浅拷贝问题
	MyStack(const MyStack<T>& stack) {
		my_deque = stack.my_deque;
	}
	~MyStack() {

	}
	void operator=(const MyStack<T>& stack) {
		my_deque = stack.my_deque;
	}
	void push(T value) {
		my_deque.push_back(value);
	}
	const T& top() {
		return my_deque.back();
	}
	void pop() {
		my_deque.pop_back();
	}
	size_t size() {
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
