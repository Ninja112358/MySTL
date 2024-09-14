#include <iostream>
#include "MyVector.h"

using namespace std;

int main() {
	MyVector<int> v;
	v.push_back(2);
	v.push_back(5);
	v.push_back(4);
	v.push_back(8);
	v.erase(4);
	v.reverse();
	for (int i = 0; i < v.size(); i++)
		cout << v[i] << endl;
	return 0;
}

