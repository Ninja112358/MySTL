#ifndef MYDEQUE_H
#define MYDEQUE_H

#define BUFFERSIZE 512	//������涨һ�黺�����Ĵ�С��512
#define MAPSIZE 8		//map�ĳ�ʼ����С

#include <cstdlib>
#include <cstring>
#include <cassert>

#ifdef _DEBUG

//����ģʽ�õ�ͷ�ļ�
#include <iostream>

#endif

template<class Ty>
class MyAllocator {
public:
	MyAllocator() {

	}
	template<class Other>
	MyAllocator(const MyAllocator<Other>&) {}
	Ty* allocate(const size_t count) {
		return (Ty*)malloc(sizeof(Ty) * count);
	}
	void deallocate(Ty* const ptr) {
		free(ptr);
	}
};

template<class T, class Alloc = MyAllocator<T>, size_t buffer_size = BUFFERSIZE>
class MyDeque {
public:
	class iterator {
	public:
		T* cur;		//��ǰԪ�ص�λ��
		T* first;	//�ռ�Ŀ�ͷ
		T* last;	//�ռ�Ľ�β
		T** node;	//ָ��map
		iterator() {
			cur = NULL;
			first = NULL;
			last = NULL;
			node = NULL;
		}
		~iterator() {

		}
		T* operator->() {
			return cur;
		}
		T& operator*() {
			return *cur;
		}


		iterator operator=(const iterator& it) {
			node = it.node;
			first = it.first;
			last = it.last;
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
			return node == it.node ? cur < it.cur : node < it.node;
		}
		bool operator>(const iterator& it) {
			return node == it.node ? cur > it.cur : node > it.node;
		}
		bool operator<=(const iterator& it) {
			return node == it.node ? cur <= it.cur : node <= it.node;
		}
		bool operator>=(const iterator& it) {
			return node == it.node ? cur >= it.cur : node >= it.node;
		}


		size_t operator-(const iterator& it) {
			if (it.node == node)
				return cur - it.cur + 1;
			//������д�ɺ꣬�����ٸ�
			return buffer_size * (node - it.node - 1) + (cur - first + 1) + (it.last - it.cur);
		}

		iterator& operator++() {
			++cur;
			if (cur == last) {
				node++;
				first = *node;
				cur = first;
				//������д�ɺ꣬�����ٸ�
				last = (*node + buffer_size);
			}
			return *this;
		}

		iterator operator++(int) {
			iterator it = *this;
			++*this;
			return it;
		}
		iterator& operator--() {
			if (cur == first) {
				node--;
				first = *node;
				last = (*node + buffer_size);
				cur = last;
			}
			--cur;
			return *this;
		}
		iterator operator--(int) {
			iterator it = *this;
			--*this;
			return it;
		}
		iterator& operator+=(size_t n) {
			size_t offset = (cur - first) + n;		//��ʾ��ǰ������ָ���λ��+n֮���Ԫ�ص�λ��(n����Ϊ����) 
			if (offset >= 0 && offset < buffer_size) {
				//Ŀ��λ����ͬһ��������
				cur += n;
			}
			else {
				//Ŀ��λ�ò���ͬһ�������� 
				//���offset>0˵���������ڵ�ǰnode�ĺ���
				//���offset<0˵���������ڵ�ǰnode��ǰ��
				size_t nodeNum = offset > 0 ? offset / buffer_size : -(-offset - 1) / buffer_size - 1;
				node += nodeNum;
				first = *node;
				last = (*node + buffer_size);
				//�ҵ���ȷ��λ��
				cur = first + (offset - nodeNum * buffer_size);
			}
			return *this;
		}
		iterator operator+(size_t n) {
			iterator it = *this;
			return it += n;
		}
		iterator& operator-=(size_t n) {
			*this += (-n);
			return *this;
		}
		iterator operator-(size_t n) {
			iterator it = *this;
			return it += (-n);
		}
	};
private:
	T** map;			//����map�洢�����ռ�ĵ�ַ
	size_t mapSize;		//�м��������ռ�
	size_t bufferSize;	//һ�黺�����Ĵ�С
	iterator start;		//ʵ��push_front
	iterator finish;	//ʵ��push_back
	Alloc _allocator;

	bool realloc_map();
	//�����±��õ�����:
	iterator find_iterator(size_t index) {
		assert(index >= 0 && index < size());
		return (start + index);
	}
public:
	MyDeque();
	~MyDeque();
	MyDeque(const MyDeque<T>& q) {
		//��д�������캯���������
		//���ں���д��operator=��������ֱ�Ӽ���
		*this = q;
	}
	void push_back(T value);			//β��
	void push_front(T value);			//ͷ��
	void pop_back();					//βɾ
	void pop_front();					//ͷɾ
	void insert(iterator it, T value);	//���ݵ���������
	void erase(iterator it);			//���ݵ�����ɾ��
	void insert(size_t index, T value);	//�����±����
	void erase(size_t index);			//�����±�ɾ��
	const T& back();					//�������һ��Ԫ�ص���һ��λ�õĵ�����
	const T& front();					//���ص�һ��Ԫ�ص�����
	bool empty();						//�пղ���
	void operator=(const MyDeque<T>& q);//���ظ�ֵ�����
	T& operator[](size_t index);			//�����±������(���Ҳ���)
#ifdef _DEBUG
	//�����õĺ���
	void debugPrint() {
		int indexStart = start.node - map;
		int indexFinish = finish.node - map;
		for (int i = indexStart; i <= indexFinish; i++) {
			if (i == indexStart)
				std::cout << "start:" << "\t";
			else if (i == indexFinish)
				std::cout << "finish:" << "\t";
			else
				std::cout << "node:" << "\t";
			for (int j = 0; j < buffer_size; j++)
				std::cout << (*(map[i] + j) < 0 ? -1 : *(map[i] + j)) << "\t";
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

#endif

	size_t cur_map_size() {
		return finish.node - start.node + 1;
	}
	iterator begin() {
		return start;
	}
	iterator end() {
		//���ڵ�����ָ��������һ��Ԫ�ص���һ��λ��,���������װ��finish��curָ��������һ��Ԫ�ص�λ��
		//���������curҪ++ 
		iterator it;
		it = finish;
		it.cur++;
		return it;
	}
	size_t size() {
		return finish - start;
	}
};

template<class T, class Alloc, size_t buffer_size>
MyDeque<T, Alloc, buffer_size>::MyDeque() {
	bufferSize = buffer_size;
	mapSize = MAPSIZE;
	//�����ʼ��ʱ��start��finish��ָ����Ψһ�Ŀռ�
	//cur�±궼ָ��������ռ���м�λ��
	map = (T**)calloc(mapSize, sizeof(T*));

	assert(map != NULL);
	if (map == NULL)
		return;

	//map[mapSize / 2] = (T*)malloc(sizeof(T) * bufferSize);
	map[mapSize / 2] = _allocator.allocate(bufferSize);

	assert(map[mapSize / 2] != NULL);
	start.node = &map[mapSize / 2];
	finish.node = &map[mapSize / 2];
	size_t index = bufferSize / 2;
	start.cur = (map[mapSize / 2] + index);
	finish.cur = (map[mapSize / 2] + index - 1);
	start.first = map[mapSize / 2];
	finish.first = map[mapSize / 2];
	//�����last�����һ��Ԫ�ص���һ����ַ
	start.last = (map[mapSize / 2] + bufferSize);
	finish.last = (map[mapSize / 2] + bufferSize);
}
template<class T, class Alloc, size_t buffer_size>
MyDeque<T, Alloc, buffer_size>::~MyDeque() {
	size_t indexStart = start.node - map;
	size_t indexFinish = finish.node - map;
	for (size_t i = indexStart; i <= indexFinish; i++) {
		_allocator.deallocate(map[i]);
		//free(map[i]);
		map[i] = NULL;
	}
	free(map);
	map = NULL;
}

template<class T, class Alloc, size_t buffer_size>
bool MyDeque<T, Alloc, buffer_size>::realloc_map() {
	//ÿһ����������һ�ε�����8 16 32 64
	T** map_temp = (T**)calloc(mapSize * 2, sizeof(T*));
	if (map_temp == NULL)
		return false;
	size_t temp = mapSize;
	size_t indexStart = start.node - map;
	size_t indexFinish = finish.node - map;
	mapSize *= 2;
	memcpy(&map_temp[temp / 2], map, sizeof(T*) * temp);
	free(map);
	map = map_temp;
	map_temp = NULL;
	start.node = &map[temp / 2 + indexStart];
	finish.node = &map[temp / 2 + indexFinish];
	return true;
}

template<class T, class Alloc, size_t buffer_size>
const T& MyDeque<T, Alloc, buffer_size>::back() {
	assert(!empty());
	return *finish.cur;
}

template<class T, class Alloc, size_t buffer_size>
const T& MyDeque<T, Alloc, buffer_size>::front() {
	assert(!empty());
	return *start.cur;
}

template<class T, class Alloc, size_t buffer_size>
bool MyDeque<T, Alloc, buffer_size>::empty() {
	return finish.node == start.node ? finish < start : false;
}

template<class T, class Alloc, size_t buffer_size>
T& MyDeque<T, Alloc, buffer_size>::operator[](size_t index) {
	assert(index >= 0 && index < size());
	////�����һ��ʼû��ʵ�ֵ�������ʱ���뵽�ķ�������������Ч��̫�ͣ�ֱ���õ����������滻��
	//if (start.last - start.cur > index) {
	//	//���index��start����
	//	return *(start.cur + index);
	//}
	//else if (bufferSize * (finish.node - start.node - 1) + (start.last - start.cur) > index) {
	//	//���index���м��node����
	//	index -= (start.last - start.cur);
	//	int indexStart = start.node - map;
	//	//index / bufferSize����˼���ҵ�indexλ�õ�ֵ�ڵڼ���node���棬��һ����Ϊ��������start��
	//	//index % bufferSize����˼��index�ڴ�node������±�λ��
	//	return *(map[indexStart + index / bufferSize + 1] + index % bufferSize);
	//}
	//else {
	//	//���index��finish����
	//	//index�ȼ�ȥstart������node�Ĵ�С��ʣ�µ�ֵ������finish�е�����λ��
	//	index -= (bufferSize * (finish.node - start.node - 1) + (start.last - start.cur));
	//	return *(finish.first + index);
	//}

	//return *(start + index);

	return *(find_iterator(index));
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::operator=(const MyDeque<T>& q) {
	bufferSize = q.bufferSize;
	mapSize = q.mapSize;
	size_t q_indexStart = q.start.node - q.map;
	size_t q_indexFinish = q.finish.node - q.map;
	map = (T**)calloc(mapSize, sizeof(T*));
	for (size_t i = q_indexStart; i <= q_indexFinish; i++) {
		//map[i] = (T*)malloc(sizeof(T) * bufferSize);
		map[i] = _allocator.allocate(bufferSize);
		assert(map[i] != NULL);
		memcpy(map[i], q.map[i], sizeof(T) * bufferSize);
	}
	start.first = map[q_indexStart];
	start.last = map[q_indexStart] + bufferSize;
	start.node = &map[q_indexStart];
	start.cur = map[q_indexStart] + (q.start.cur - q.start.first);
	finish.first = map[q_indexFinish];
	finish.last = map[q_indexFinish] + bufferSize;
	finish.node = &map[q_indexFinish];
	finish.cur = map[q_indexFinish] + (q.finish.cur - q.finish.first);
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::push_back(T value) {
	if (finish.cur != finish.last - 1) {
		finish.cur++;
		*finish.cur = value;
	}
	else {
		if ((finish.node - map) == mapSize - 1)
			realloc_map();
		//��finish��nodeָ��map��һ��λ��
		finish.node++;
		//�ڴ�λ���Ͻ��и�ֵ����
		size_t indexFinish = finish.node - map;
		//map[indexFinish] = (T*)malloc(sizeof(T) * bufferSize);
		map[indexFinish] = _allocator.allocate(bufferSize);
		finish.node = &map[indexFinish];
		assert(*finish.node != NULL);
		finish.cur = *finish.node;
		*finish.cur = value;
		finish.first = *finish.node;
		finish.last = *finish.node + bufferSize;
	}
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::push_front(T value) {
	if (start.cur != start.first) {
		start.cur--;
		*start.cur = value;
	}
	else {
		if ((start.node - map) == 0)
			realloc_map();
		//��start��nodeָ��map��һ��λ��
		start.node--;
		//�ڴ�λ���Ͻ��и�ֵ����
		size_t indexStart = start.node - map;
		//map[indexStart] = (T*)malloc(sizeof(T) * bufferSize);
		map[indexStart] = _allocator.allocate(bufferSize);
		start.node = &map[indexStart];
		assert(*start.node != NULL);
		//�����ͷ����ôcur��λ�þ��ǻ��������һ��λ��
		start.cur = *start.node + bufferSize - 1;
		*start.cur = value;
		start.first = *start.node;
		start.last = *start.node + bufferSize;
	}
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::pop_back() {
	assert(!empty());
	if (finish.cur != finish.first)
		finish.cur--;
	else {
		if (start.node == finish.node && start.cur == start.first) {
			pop_front();
		}
		else {
			size_t indexFinish = finish.node - map;
			//free(*finish.node);
			_allocator.deallocate(*finish.node);
			finish.node--;
			//��map�е�Ұָ��ָ���
			map[indexFinish] = NULL;
			finish.first = *finish.node;
			finish.last = *finish.node + bufferSize;
			finish.cur = *finish.node + bufferSize - 1;
		}
	}
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::pop_front() {
	assert(!empty());
	if (start.cur != start.last - 1)
		start.cur++;
	else {
		if (start.node == finish.node && finish.cur == finish.last - 1) {
			pop_back();
		}
		else {
			size_t indexStart = start.node - map;
			//free(*start.node);
			_allocator.deallocate(*start.node);
			start.node++;
			//��map�е�Ұָ��ָ���
			map[indexStart] = NULL;
			start.first = *start.node;
			start.last = *start.node + bufferSize;
			start.cur = start.first;
		}
	}
}
template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::insert(iterator it, T value) {
	if (it.cur == start.cur)
		push_front(value);
	else if (it.cur == finish.cur + 1)
		push_back(value);
	else {
		//���insert���м�
		size_t index = it - start;
		if (index < size() / 2) {
			T temp = front();
			//ʣ�����ݽ���Ų��
			//��������memmove���Ż�ʱ��
			size_t indexStart = start.node - map;
			size_t indexIt = it.node - map;
			if (it.node != start.node) {
				memmove(map[indexStart] + (start.cur - start.first), map[indexStart] + (start.cur - start.first + 1), sizeof(T) * (start.last - start.cur - 1));
				*(map[indexStart] + bufferSize - 1) = *map[indexStart + 1];
			}
			else
				memmove(map[indexStart] + (start.cur - start.first), map[indexStart] + (start.cur - start.first + 1), sizeof(T) * (it.cur - start.cur - 1));
			//�����Ƕ�start��it֮����ӵ�node���д���
			for (size_t map_i = (indexStart + 1); map_i < indexIt; map_i++) {
				memmove(map[map_i], map[map_i] + 1, sizeof(T) * (bufferSize - 1));
				*(map[map_i] + bufferSize - 1) = *map[map_i + 1];
			}
			if (it.node != start.node)
				memmove(map[indexIt], map[indexIt] + 1, sizeof(T) * (it.cur - it.first));
			//��Ԫ�ز��뵽ͷ��
			push_front(temp);
			*(it - 1) = value;
		}
		else {
			T temp = back();
			//ʣ�����ݽ���Ų��
			//��������memmove���Ż�ʱ��
			size_t indexFinish = finish.node - map;
			size_t indexIt = it.node - map;
			if (it.node != finish.node) {
				memmove(map[indexFinish] + 1, map[indexFinish], sizeof(T) * (finish.cur - finish.first));
				*map[indexFinish] = *(map[indexFinish - 1] + bufferSize - 1);
			}
			else
				memmove(map[indexFinish] + (it.cur - finish.first) + 1, map[indexFinish] + (it.cur - finish.first), sizeof(T) * (finish.cur - it.cur));
			//�����Ƕ�it��finish֮����ӵ�node���д���
			for (size_t map_i = (indexFinish - 1); map_i > indexIt; map_i--) {
				memmove(map[map_i] + 1, map[map_i], sizeof(T) * (bufferSize - 1));
				*map[map_i] = *(map[map_i - 1] + bufferSize - 1);
			}
			if (it.node != finish.node)
				memmove(map[indexIt] + (it.cur - it.first) + 1, map[indexIt] + (it.cur - it.first), sizeof(T) * (bufferSize - (it.cur - it.first) - 1));
			//��Ԫ�ز��뵽β��
			push_back(temp);
			*it = value;
		}
	}
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::erase(iterator it) {
	if (it.cur == start.cur)
		pop_front();
	else if (it.cur == finish.cur)
		pop_back();
	else {
		//���erase���м�
		size_t index = it - start;
		size_t indexIt = it.node - map;
		if (index < size() / 2) {
			//�����ǰit�����λ��ʱ
			//itǰ��Ԫ�����������ƶ�һλ
			size_t indexStart = start.node - map;
			if (it.node != start.node) {
				memmove(map[indexIt] + 1, map[indexIt], sizeof(T) * (it.cur - it.first));
				*map[indexIt] = *(map[indexIt - 1] + (bufferSize - 1));
			}
			else
				memmove(map[indexStart] + (start.cur - start.first) + 1, map[indexStart] + (start.cur - start.first), sizeof(T) * (it.cur - start.cur));
			for (size_t map_i = indexIt - 1; map_i > indexStart; map_i--) {
				memmove(map[map_i] + 1, map[map_i], sizeof(T) * (bufferSize - 1));
				*map[map_i] = *(map[map_i - 1] + bufferSize - 1);
			}
			if (it.node != start.node)
				memmove(map[indexStart] + (start.cur - start.first) + 1, map[indexStart] + (start.cur - start.first), sizeof(T) * (start.last - start.cur - 1));
			pop_front();
		}
		else {
			//�����ǰit���Ҳ�λ��ʱ
			//itǰ��Ԫ�����������ƶ�һλ
			size_t indexFinish = finish.node - map;
			size_t indexIt = it.node - map;
			if (it.node != finish.node) {
				memmove(map[indexIt] + (it.cur - it.first), map[indexIt] + (it.cur - it.first) + 1, sizeof(T) * (it.last - it.cur - 1));
				*(map[indexIt] + bufferSize - 1) = *map[indexIt + 1];
			}
			else
				memmove(map[indexFinish] + (it.cur - it.first), map[indexFinish] + (it.cur - it.first) + 1, sizeof(T) * (it.last - it.cur - 1));
			for (size_t map_i = indexIt + 1; map_i < indexFinish; map_i++) {
				memmove(map[map_i], map[map_i] + 1, sizeof(T) * (bufferSize - 1));
				*(map[map_i] + bufferSize - 1) = *map[map_i + 1];
			}
			if (it.node != finish.node)
				memmove(map[indexFinish], map[indexFinish] + 1, sizeof(T) * (finish.cur - finish.first));
			pop_back();
		}
	}
}
template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::insert(size_t index, T value) {
	assert(index >= 0 && index < size());
	insert(find_iterator(index), value);
}

template<class T, class Alloc, size_t buffer_size>
void MyDeque<T, Alloc, buffer_size>::erase(size_t index) {
	assert(index >= 0 && index < size());
	erase(find_iterator(index));
}

#endif