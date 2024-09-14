#include <iostream>
#include "MyDeque.h"

using namespace std;

int main() {
	MyDeque<int> dq;
	dq.push_back(5);
	dq.push_back(2);
	dq.push_back(4);
	dq.push_back(1);
	dq.push_back(8);
	for (int i : dq)
		cout << i << " ";
	cout << endl;
	int len = dq.size();
	for (int i = 0; i < len; i++)
		cout << dq[i] << " ";
	cout << endl;
	return 0;
}