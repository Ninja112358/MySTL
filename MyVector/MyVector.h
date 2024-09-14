#ifndef MYVECTOR_H
#define MYVECTOR_H
#include <cstdlib>
#include <cstring>

#define OK 1
#define ERROR 0

#ifndef OVERFLOW
#define OVERFLOW 3
#endif

#define VECTOR_MAX_SIZE 16

//后面需要用于内存分配的算法:
//求一个数的最临近的比这个数大的2的幂次
//如果这个数就是2的幂次就返回这个数*2的值
size_t FindNextPow2(size_t num) {
	//判断这个数是不是二的幂次
	if ((num & (num - 1)) == 0)
		return num * 2;
	unsigned short ans = 1;
	for (int i = 0; i < 6; i++) {
		num |= num >> ans;
		ans <<= 1;
	}
	return num + 1;
}

typedef int Status;

template<class T>
class MyVector {
private:
	T* elem;
	size_t length;
	bool realloc_vector();
	size_t max_size;
public:
	class iterator {
	public:
		T* cur;
		iterator() {
			cur = NULL;
		}
		~iterator() {

		}
		iterator& operator++() {
			cur++;
			return *this;
		}
		//后自增(会有额外开销,所以还是使用前自增比较好)
		iterator operator++(int) {
			iterator temp = *this;
			++*this;
			return temp;
		}
		iterator& operator+=(int n) {
			cur += n;
			return *this;
		}
		iterator& operator--() {
			cur--;
			return *this;
		}
		//后自增(会有额外开销,所以还是使用前自增比较好)
		iterator operator--(int) {
			iterator temp = *this;
			--*this;
			return temp;
		}
		iterator& operator-=(int n) {
			cur -= n;
			return *this;
		}
		iterator operator-(int n) {
			iterator it = *this;
			return it -= n;
		}
		iterator operator+(int n) {
			iterator it = *this;
			return it += n;
		}
		T operator*() {
			return *cur;
		}
		iterator operator=(const iterator& it) {
			cur = it.cur;
			return *this;
		}
		bool operator==(const iterator& it) {
			return cur == it.cur;
		}
		bool operator!=(const iterator& it) {
			return cur != it.cur;
		}
		bool operator<(const iterator& it) {
			return cur < it.cur;
		}
		bool operator>(const iterator& it) {
			return cur > it.cur;
		}
		bool operator<=(const iterator& it) {
			return cur <= it.cur;
		}
		bool operator>=(const iterator& it) {
			return cur >= it.cur;
		}

	};
	MyVector();
	MyVector(size_t n);
	~MyVector();

	Status push_back(T value);		//顺序表的尾插
	Status pop_back();				//顺序表的尾删 
	Status clear();					//顺序表的清空
	size_t size();						//查看顺序表的长度
	bool empty();
	T& operator[](size_t index);		//赋值和查找操作
	MyVector<T>& operator=(MyVector<T>& v);	//顺序表与顺序表之间的赋值
	iterator begin() {
		iterator it;
		it.cur = elem;
		return it;
	}
	iterator end() {
		iterator it;
		it.cur = elem + length;
		return it;
	}
	Status erase(size_t index);			//顺序表的删除:下标
	Status insert(size_t index, T value);	//顺序表的插入:下标

	//一些关于顺序表的算法
	void reverse();		//顺序表的翻转
	size_t find(T value);	//根据值找下标
};

template<class T>
bool MyVector<T>::realloc_vector() {
	max_size = FindNextPow2(max_size);
	T* temp = (T*)realloc(elem, sizeof(T) * max_size);
	if (temp == NULL)
		return false;
	elem = temp;
	temp = NULL;
	return true;
}

template<class T>
MyVector<T>::MyVector() {
	max_size = VECTOR_MAX_SIZE;
	elem = (T*)malloc(sizeof(T) * max_size);
	length = 0;
	if (elem == NULL)
		return;
}

template<class T>
MyVector<T>::MyVector(size_t n) {
	elem = (T*)malloc(sizeof(T) * n);
	length = n;
}

template<class T>
MyVector<T>::~MyVector() {
	free(elem);
	elem = NULL;
	length = 0;
}

template<class T>
Status MyVector<T>::push_back(T value) {
	if (length >= max_size)
		realloc_vector();
	if (elem == NULL)
		return ERROR;
	elem[length] = value;
	length++;
	return OK;
}
template<class T>
Status MyVector<T>::pop_back() {
	if (length - 1 < 0)
		return ERROR;
	length--;
	return OK;

}
template<class T>
Status MyVector<T>::clear() {
	free(elem);
	max_size = VECTOR_MAX_SIZE;
	elem = (T*)malloc(sizeof(T) * max_size);
	if (elem == NULL)
		return ERROR;
	length = 0;
}

template<class T>
Status MyVector<T>::erase(size_t index) {
	if (index < 0 || index >= length)
		return ERROR;
	memmove(elem + index, elem + index + 1, sizeof(T) * (length - index - 1));
	length--;
	return OK;
}

template<class T>
size_t MyVector<T>::size() {
	return length;
}

template<class T>
bool MyVector<T>::empty() {
	return length == 0;
}

template<class T>
T& MyVector<T>::operator[](size_t index) {
	return elem[index];
}

template<class T>
MyVector<T>& MyVector<T>::operator=(MyVector<T>& v) {
	free(elem);
	elem = (T*)malloc(v.max_size * sizeof(T));
	memcpy(elem, v.elem, sizeof(T) * v.length);
	length = v.length;
	return *this;
}

template<class T>
Status MyVector<T>::insert(size_t index, T value) {
	if (index < 0 || index > length)
		return ERROR;
	if (length >= max_size)
		realloc_vector();
	if (index != length) {
		memmove(elem + index + 1, elem + index, sizeof(T) * (length - index + 1));
		elem[index] = value;
		length++;
	}
	else
		push_back(value);
	return OK;
}

template<class T>
void MyVector<T>::reverse() {
	for (int i = 0; i < length / 2; i++) {
		T temp = elem[i];
		elem[i] = elem[length - i - 1];
		elem[length - i - 1] = temp;
	}
}

template<class T>
size_t MyVector<T>::find(T value) {
	for (int i = 0; i < length; i++)
		if (elem[i] == value)
			return i;
	return -1;
}


#endif

