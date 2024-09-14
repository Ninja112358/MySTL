#include <iostream>
#include "MyPriorityQueue.h"

using namespace std;

int main() {
	//大根堆
	cout << "大根堆:" << endl;
	MyPriorityQueue<int, MyVector<int>,greater<int>> max_q;
	max_q.push(5);
	max_q.push(8);
	max_q.push(2);
	max_q.push(1);
	max_q.push(4);
	while (!max_q.empty()) {
		cout << max_q.top() << endl;
		max_q.pop();
	}
	//小根堆的演示
	cout << "小根堆:" << endl;
	MyPriorityQueue<int, MyVector<int>, less<int>> min_q;
	min_q.push(5);
	min_q.push(8);
	min_q.push(2);
	min_q.push(1);
	min_q.push(4);
	while (!min_q.empty()) {
		cout << min_q.top() << endl;
		min_q.pop();
	}
	return 0;
}