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

//������Ҫ�����ڴ������㷨:
//��һ���������ٽ��ı���������2���ݴ�
//������������2���ݴξͷ��������*2��ֵ
size_t FindNextPow2(size_t num) {
	//�ж�������ǲ��Ƕ����ݴ�
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
		//������(���ж��⿪��,���Ի���ʹ��ǰ�����ȽϺ�)
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
		//������(���ж��⿪��,���Ի���ʹ��ǰ�����ȽϺ�)
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

	Status push_back(T value);		//˳����β��
	Status pop_back();				//˳����βɾ 
	Status clear();					//˳�������
	size_t size();						//�鿴˳���ĳ���
	bool empty();
	T& operator[](size_t index);		//��ֵ�Ͳ��Ҳ���
	MyVector<T>& operator=(MyVector<T>& v);	//˳�����˳���֮��ĸ�ֵ
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
	Status erase(size_t index);			//˳����ɾ��:�±�
	Status insert(size_t index, T value);	//˳���Ĳ���:�±�

	//һЩ����˳�����㷨
	void reverse();		//˳���ķ�ת
	size_t find(T value);	//����ֵ���±�
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

