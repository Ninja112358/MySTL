#ifndef MYDEQUE_H
#define MYDEQUE_H

#define BUFFERSIZE 512	//在这里规定一块缓冲区的大小是512
#define MAPSIZE 8		//map的初始化大小

#include <cstdlib>
#include <cstring>
#include <cassert>

#ifdef _DEBUG

//调试模式用的头文件
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
		T* cur;		//当前元素的位置
		T* first;	//空间的开头
		T* last;	//空间的结尾
		T** node;	//指向map
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
			//这里先写成宏，后面再改
			return buffer_size * (node - it.node - 1) + (cur - first + 1) + (it.last - it.cur);
		}

		iterator& operator++() {
			++cur;
			if (cur == last) {
				node++;
				first = *node;
				cur = first;
				//这里先写成宏，后面再改
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
			size_t offset = (cur - first) + n;		//表示当前迭代器指向的位置+n之后的元素的位置(n可能为负数) 
			if (offset >= 0 && offset < buffer_size) {
				//目标位置在同一缓冲区内
				cur += n;
			}
			else {
				//目标位置不在同一缓冲区内 
				//如果offset>0说明缓冲区在当前node的后面
				//如果offset<0说明缓冲区在当前node的前面
				size_t nodeNum = offset > 0 ? offset / buffer_size : -(-offset - 1) / buffer_size - 1;
				node += nodeNum;
				first = *node;
				last = (*node + buffer_size);
				//找到正确的位置
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
	T** map;			//利用map存储连续空间的地址
	size_t mapSize;		//有几块连续空间
	size_t bufferSize;	//一块缓冲区的大小
	iterator start;		//实现push_front
	iterator finish;	//实现push_back
	Alloc _allocator;

	bool realloc_map();
	//根据下标获得迭代器:
	iterator find_iterator(size_t index) {
		assert(index >= 0 && index < size());
		return (start + index);
	}
public:
	MyDeque();
	~MyDeque();
	MyDeque(const MyDeque<T>& q) {
		//重写拷贝构造函数进行深拷贝
		//由于后面写了operator=所以这里直接简化了
		*this = q;
	}
	void push_back(T value);			//尾插
	void push_front(T value);			//头插
	void pop_back();					//尾删
	void pop_front();					//头删
	void insert(iterator it, T value);	//根据迭代器插入
	void erase(iterator it);			//根据迭代器删除
	void insert(size_t index, T value);	//根据下标插入
	void erase(size_t index);			//根据下标删除
	const T& back();					//返回最后一个元素的下一个位置的迭代器
	const T& front();					//返回第一个元素迭代器
	bool empty();						//判空操作
	void operator=(const MyDeque<T>& q);//重载赋值运算符
	T& operator[](size_t index);			//重载下标运算符(查找操作)
#ifdef _DEBUG
	//调试用的函数
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
		//由于迭代器指向的是最后一个元素的下一个位置,但是这里封装的finish的cur指向的是最后一个元素的位置
		//所以这里的cur要++ 
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
	//在最初始的时候start和finish都指向那唯一的空间
	//cur下标都指向在这个空间的中间位置
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
	//这里的last是最后一个元素的下一个地址
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
	//每一次扩容是上一次的两倍8 16 32 64
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
	////这个是一开始没有实现迭代器的时候想到的方法，后来发现效率太低，直接用迭代器进行替换了
	//if (start.last - start.cur > index) {
	//	//如果index在start上面
	//	return *(start.cur + index);
	//}
	//else if (bufferSize * (finish.node - start.node - 1) + (start.last - start.cur) > index) {
	//	//如果index在中间的node上面
	//	index -= (start.last - start.cur);
	//	int indexStart = start.node - map;
	//	//index / bufferSize的意思是找到index位置的值在第几个node上面，加一是因为不可能在start上
	//	//index % bufferSize的意思是index在此node上面的下标位置
	//	return *(map[indexStart + index / bufferSize + 1] + index % bufferSize);
	//}
	//else {
	//	//如果index在finish上面
	//	//index先减去start和所有node的大小，剩下的值就是在finish中的坐标位置
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
		//将finish的node指向map下一个位置
		finish.node++;
		//在此位置上进行赋值操作
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
		//将start的node指向map上一个位置
		start.node--;
		//在此位置上进行赋值操作
		size_t indexStart = start.node - map;
		//map[indexStart] = (T*)malloc(sizeof(T) * bufferSize);
		map[indexStart] = _allocator.allocate(bufferSize);
		start.node = &map[indexStart];
		assert(*start.node != NULL);
		//如果是头插那么cur的位置就是缓冲区最后一个位置
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
			//让map中的野指针指向空
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
			//让map中的野指针指向空
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
		//如果insert在中间
		size_t index = it - start;
		if (index < size() / 2) {
			T temp = front();
			//剩下数据进行挪动
			//这里利用memmove会优化时间
			size_t indexStart = start.node - map;
			size_t indexIt = it.node - map;
			if (it.node != start.node) {
				memmove(map[indexStart] + (start.cur - start.first), map[indexStart] + (start.cur - start.first + 1), sizeof(T) * (start.last - start.cur - 1));
				*(map[indexStart] + bufferSize - 1) = *map[indexStart + 1];
			}
			else
				memmove(map[indexStart] + (start.cur - start.first), map[indexStart] + (start.cur - start.first + 1), sizeof(T) * (it.cur - start.cur - 1));
			//这里是对start和it之间夹杂的node进行处理
			for (size_t map_i = (indexStart + 1); map_i < indexIt; map_i++) {
				memmove(map[map_i], map[map_i] + 1, sizeof(T) * (bufferSize - 1));
				*(map[map_i] + bufferSize - 1) = *map[map_i + 1];
			}
			if (it.node != start.node)
				memmove(map[indexIt], map[indexIt] + 1, sizeof(T) * (it.cur - it.first));
			//把元素插入到头部
			push_front(temp);
			*(it - 1) = value;
		}
		else {
			T temp = back();
			//剩下数据进行挪动
			//这里利用memmove会优化时间
			size_t indexFinish = finish.node - map;
			size_t indexIt = it.node - map;
			if (it.node != finish.node) {
				memmove(map[indexFinish] + 1, map[indexFinish], sizeof(T) * (finish.cur - finish.first));
				*map[indexFinish] = *(map[indexFinish - 1] + bufferSize - 1);
			}
			else
				memmove(map[indexFinish] + (it.cur - finish.first) + 1, map[indexFinish] + (it.cur - finish.first), sizeof(T) * (finish.cur - it.cur));
			//这里是对it和finish之间夹杂的node进行处理
			for (size_t map_i = (indexFinish - 1); map_i > indexIt; map_i--) {
				memmove(map[map_i] + 1, map[map_i], sizeof(T) * (bufferSize - 1));
				*map[map_i] = *(map[map_i - 1] + bufferSize - 1);
			}
			if (it.node != finish.node)
				memmove(map[indexIt] + (it.cur - it.first) + 1, map[indexIt] + (it.cur - it.first), sizeof(T) * (bufferSize - (it.cur - it.first) - 1));
			//把元素插入到尾部
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
		//如果erase在中间
		size_t index = it - start;
		size_t indexIt = it.node - map;
		if (index < size() / 2) {
			//如果当前it在左侧位置时
			//it前的元素整体向右移动一位
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
			//如果当前it在右侧位置时
			//it前的元素整体向左移动一位
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