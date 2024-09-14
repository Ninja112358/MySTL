#ifndef MYSTRING_H
#define MYSTRING_H

#define STRING_MAX_SIZE 16

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <ctype.h>



class MyString {
	friend std::istream& getline(std::istream& cin, MyString& str, char end);
	friend std::ostream& operator<<(std::ostream& cout, MyString str);
	friend std::istream& operator>>(std::istream& cin, MyString& str);
private:
	char* str_data;
	size_t str_size;
	size_t max_size;
	bool realloc_str();
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
public:
	class iterator {
	public:
		char* cur;
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
		char operator*() {
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
	MyString();
	MyString(const MyString& str) {
		str_data = (char*)malloc(sizeof(char) * str.max_size);
		if (str_data == NULL)
			throw "OVERFLOW!!!!!";
		memcpy(str_data, str.str_data, sizeof(char) * (str.str_size + 1));
		str_size = str.str_size;
		max_size = str.max_size;
	}
	MyString(const char* str) {
		size_t str_len = strlen(str);
		size_t str_max_size = FindNextPow2(str_len + 1);
		str_data = (char*)malloc(sizeof(char) * str_max_size);
		if (str_data == NULL)
			throw "OVERFLOW!!!!!";
		memcpy(str_data, str, sizeof(char) * (str_len + 1));
		str_size = str_len;
		max_size = str_max_size;
	}
	MyString(const char ch) {
		str_size = 1;
		max_size = STRING_MAX_SIZE;
		str_data = (char*)malloc(sizeof(char) * max_size);
		if (str_data == NULL)
			throw "OVERFLOW!!!!!";
		*str_data = ch;
	}
	~MyString();

	void push_back(char ch);
	void pop_back();
	void clear();
	bool empty();
	size_t length();
	size_t size();
	char* data();		//返回char*类型的字符串
	char& front();		//返回头部
	char& back();		//返回尾部
	char& at(size_t index);
	void insert(size_t index, const MyString& str);		//在index位置插入一个字符串
	void insert(size_t index, const char* str);			//同上,但是类型是char*
	void erase(size_t index, size_t n);					//删除从index位置开始的n个字符,和stl的erase一样
	void erase(size_t index);							//删除index位置之后的所有字符,和stl的也一样
	size_t find(const MyString& str, size_t index = 0);	//查找字符串str第一次出现的位置
	size_t find(const char* str, size_t index = 0);		//同上,但是类型是char*
	void replace(size_t index, size_t n, const MyString& str);	//替换从index位置开始的n个字符为str,和stl的replace一样
	void replace(size_t index, size_t n, const char* str);		//同上,但是类型是char*


	iterator begin() {
		iterator it;
		it.cur = str_data;
		return it;
	}
	iterator end() {
		iterator it;
		it.cur = str_data + str_size;
		return it;
	}
	//为了贴合stl，这里采用的截取字符方法和stl中的一模一样
	MyString substr(size_t startIndex, size_t counts);
	//这里的内存分配机制完全贴合与2的幂次，这里只是当前分配一次内存
	//都后面扩容的时候自动会将内存空间自动变成与他临近的2的幂次
	void reserve(size_t size) {
		//当分配内存比实际内容容量小，那么直接退出
		if (size <= str_size + 1)
			return;
		max_size = size;
		char* temp = (char*)realloc(str_data, sizeof(char) * max_size);
		if (temp == NULL)
			return;
		free(str_data);
		str_data = temp;
		temp = NULL;
	}
	size_t capacity();		//返回当前分配的物理真实内存

	//运算符重载
	char& operator[](size_t index);
	MyString operator=(const MyString& str);
	MyString operator=(const char* str);
	MyString operator=(const char ch);

	MyString operator+=(const char* str);
	MyString operator+=(const MyString& str);
	MyString operator+=(const char ch);

	MyString operator+(const char* str);
	MyString operator+(const MyString& str);
	MyString operator+(const char ch);

	bool operator==(const MyString& str);
	bool operator==(const char* str);
	bool operator!=(const MyString& str);
	bool operator!=(const char* str);
	bool operator<(const MyString& str);
	bool operator<(const char* str);
	bool operator>(const MyString& str);
	bool operator>(const char* str);
	bool operator<=(const MyString& str);
	bool operator<=(const char* str);
	bool operator>=(const MyString& str);
	bool operator>=(const char* str);

};

std::ostream& operator<<(std::ostream& cout, MyString str) {
	std::cout << str.str_data;
	return cout;
}
std::istream& operator>>(std::istream& cin, MyString& str) {
	str.clear();
	for (int c = cin.get(); c != EOF && isspace(c); c = cin.get())
		str.push_back(c);
	return cin;
}

bool MyString::realloc_str() {
	max_size = FindNextPow2(max_size);
	char* temp = (char*)realloc(str_data, sizeof(char) * max_size);
	if (temp == NULL)
		return false;
	str_data = temp;
	temp = NULL;
	return true;
}

MyString::MyString() {
	max_size = STRING_MAX_SIZE;
	str_size = 0;
	str_data = (char*)malloc(sizeof(char) * max_size);
	if (str_data == NULL) {
		return;
	}
	*str_data = '\0';
}

MyString::~MyString() {
	free(str_data);
	str_data = NULL;
	str_size = 0;
}

void MyString::push_back(char ch) {
	if (str_size + 1 >= max_size)
		realloc_str();
	str_data[str_size] = ch;
	str_data[str_size + 1] = '\0';
	str_size++;
}

void MyString::pop_back() {
	if (str_size == 0)
		return;
	str_size--;
	str_data[str_size] = '\0';
}

void MyString::clear() {
	free(str_data);
	max_size = STRING_MAX_SIZE;
	str_size = 0;
	str_data = (char*)malloc(sizeof(char) * max_size);
	if (str_data == NULL)
		return;
	*str_data = '\0';
}

bool MyString::empty() {
	return str_size == 0;
}

size_t MyString::length() {
	return str_size;
}
size_t MyString::size() {
	return str_size;
}

char* MyString::data() {
	return str_data;
}

char& MyString::front() {
	if (str_size == 0)
		throw "EMPTY!!!";
	return str_data[0];
}

char& MyString::back() {
	if (str_size == 0)
		throw "EMPTY!!!";
	return str_data[str_size - 1];
}

char& MyString::at(size_t index) {
	if (str_size == 0)
		throw "EMPTY!!!";
	return str_data[index];
}
void MyString::insert(size_t index, const MyString& str) {
	if (index < 0 || index > str_size)
		throw "INDEX ERROR!!!";
	while (str.str_size + str_size + 1 >= max_size)
		realloc_str();
	memmove(str_data + index + str.str_size, str_data + index, sizeof(char) * (str_size - index + 1));
	memcpy(str_data + index, str.str_data, sizeof(char) * str.str_size);
	str_size += str.str_size;
}
void MyString::insert(size_t index, const char* str) {
	if (index < 0 || index > str_size)
		throw "INDEX ERROR!!!";
	size_t size = strlen(str);
	while (size + str_size + 1 >= max_size)
		realloc_str();
	memmove(str_data + index + size, str_data + index, sizeof(char) * (str_size - index + 1));
	memcpy(str_data + index, str, sizeof(char) * size);
	str_size += size;
}
void MyString::erase(size_t index, size_t n) {
	if (index < 0 || index >= str_size || str_size - n < 0)
		throw "INDEX ERROR!!!";
	memmove(str_data + index, str_data + index + n, sizeof(char) * (str_size - index - n + 1));
	str_size -= n;
}
void MyString::erase(size_t index) {
	erase(index, str_size - index);
}
size_t MyString::find(const MyString& str, size_t index) {
	if (str_size < str.str_size || str.str_size < 1)
		return -1;
	size_t x = 0;
	size_t y = 0;
	size_t* next;
	if (str.str_size == 1) {
		next = new size_t[1];
		next[0] = -1;
	}
	else {
		next = new size_t[str.str_size];
		next[0] = -1;
		next[1] = 0;
	}
	size_t i = 2;
	size_t cn = 0;
	while (i < str.str_size) {
		if (str.str_data[i - 1] == str.str_data[cn])
			next[++i] = ++cn;
		else if (cn > 0)
			cn = next[cn];
		else
			next[i++] = 0;
	}
	while (x < str_size && y < str.str_size) {
		if (str_data[x] == str.str_data[y]) {
			x++;
			y++;
		}
		else if (next[y] == -1)
			x++;
		else
			y = next[y];
	}
	return y == str.str_size ? x - y : -1;
}
size_t MyString::find(const char* str, size_t index) {
	size_t size = strlen(str);
	if (str_size < size || size < 1)
		return -1;
	size_t x = 0;
	size_t y = 0;
	size_t* next;
	if (size == 1) {
		next = new size_t[1];
		next[0] = -1;
	}
	else {
		next = new size_t[size];
		next[0] = -1;
		next[1] = 0;
	}
	size_t i = 2;
	size_t cn = 0;
	while (i < size) {
		if (str[i - 1] == str[cn])
			next[++i] = ++cn;
		else if (cn > 0)
			cn = next[cn];
		else
			next[i++] = 0;
	}
	while (x < str_size && y < size) {
		if (str_data[x] == str[y]) {
			x++;
			y++;
		}
		else if (next[y] == -1)
			x++;
		else
			y = next[y];
	}
	return y == size ? x - y : -1;
}
void MyString::replace(size_t index, size_t n, const MyString& str) {
	erase(index, n);
	insert(index, str);
}
void MyString::replace(size_t index, size_t n, const char* str) {
	erase(index, n);
	insert(index, str);
}

MyString MyString::substr(size_t startIndex, size_t counts) {
	MyString str;
	str.str_size = counts;
	str.max_size = max_size;
	memcpy(str.str_data, str_data + startIndex, sizeof(char) * counts);
	str.str_data[counts] = '\0';
	return str;
}

size_t MyString::capacity() {
	return max_size;
}
std::istream& getline(std::istream& cin, MyString& str, char cend = '\n') {
	str.clear();
	while (true) {
		size_t begin = str.str_size;
		str.str_size = str.max_size - 1;
		size_t length = cin.get(&str.str_data[begin], str.max_size - begin, cend).gcount();
		int c = cin.get();
		if (c != cend && c != EOF)
			str.push_back(c);//扩容
		else {
			str.str_size = begin + length;
			break;
		}
	}
	return cin;
}

char& MyString::operator[](size_t index) {
	return str_data[index];
}
MyString MyString::operator=(const MyString& str) {
	free(str_data);
	str_data = (char*)malloc(sizeof(char) * str.max_size);
	if (str_data == NULL)
		throw "OVERFLOW!!!!!";
	memcpy(str_data, str.str_data, sizeof(char) * (str.str_size + 1));
	str_size = str.str_size;
	max_size = str.max_size;
	return *this;
}
MyString MyString::operator=(const char* str) {
	free(str_data);
	size_t str_len = strlen(str);
	size_t str_max_size = FindNextPow2(str_len + 1);
	str_data = (char*)malloc(sizeof(char) * str_max_size);
	if (str_data == NULL)
		throw "OVERFLOW!!!!!";
	memcpy(str_data, str, sizeof(char) * (str_len + 1));
	str_size = str_len;
	max_size = str_max_size;
	return *this;
}

MyString MyString::operator=(const char ch) {
	free(str_data);
	str_size = 1;
	max_size = STRING_MAX_SIZE;
	str_data = (char*)malloc(sizeof(char) * max_size);
	if (str_data == NULL)
		throw "OVERFLOW!!!!!";
	*str_data = ch;
	return *this;
}

MyString MyString::operator+=(const char* str) {
	size_t str_len = strlen(str) + str_size;
	size_t str_max_size = FindNextPow2(str_len + 1);
	char* temp;
	if (str_max_size > max_size) {
		//这个temp是考虑了内存如果为空导致数据丢失的问题
		temp = (char*)realloc(str_data, sizeof(char) * str_max_size);
		if (temp == NULL)
			throw "error!!!!";
		str_data = temp;
		temp = NULL;
		max_size = str_max_size;
	}
	memcpy(str_data + str_size, str, sizeof(char) * (str_len - str_size + 1));
	str_size = str_len;
	return *this;
}

MyString MyString::operator+=(const MyString& str) {
	size_t str_len = str.str_size + str_size;
	size_t str_max_size = FindNextPow2(str_len + 1);
	if (str_max_size > max_size) {
		//这个temp是考虑了内存如果为空导致数据丢失的问题
		char* temp = (char*)realloc(str_data, sizeof(char) * str_max_size);
		if (temp == NULL)
			throw "error!!!!";
		str_data = temp;
		temp = NULL;
		max_size = str_max_size;
	}
	memcpy(str_data + str_size, str.str_data, sizeof(char) * (str.str_size + 1));
	str_size = str_len;
	return *this;
}

MyString MyString::operator+=(const char ch) {
	push_back(ch);
	return *this;
}

MyString MyString::operator+(const char* str) {
	MyString ans = *this;
	ans += str;
	return ans;
}

MyString MyString::operator+(const MyString& str) {
	MyString ans = *this;
	ans += str;
	return ans;
}

MyString MyString::operator+(const char ch) {
	MyString ans = *this;
	ans += ch;
	return ans;
}

bool MyString::operator==(const MyString& str) {
	if (str_size != str.str_size)
		return false;
	return memcmp(str_data, str.str_data, sizeof(char) * str_size) == 0;
}
bool MyString::operator==(const char* str) {
	size_t n = strlen(str);
	if (str_size != n)
		return false;
	return memcmp(str_data, str, sizeof(char) * str_size) == 0;
}

bool MyString::operator!=(const MyString& str) {
	if (str_size != str.str_size)
		return false;
	return memcmp(str_data, str.str_data, sizeof(char) * str_size) != 0;
}

bool MyString::operator!=(const char* str) {
	size_t n = strlen(str);
	if (str_size != n)
		return false;
	return memcmp(str_data, str, sizeof(char) * str_size) != 0;
}

bool MyString::operator<(const MyString& str) {
	short int cmp = memcmp(str_data, str.str_data, sizeof(char) * (str_size < str.str_size ? str_size : str.str_size));
	if (str_size == str.str_size)
		return cmp < 0;
	else if (str_size < str.str_size)
		return cmp == 0 ? true : cmp < 0;
	else
		return cmp == 0 ? false : cmp < 0;
}

bool MyString::operator<(const char* str) {
	size_t n = strlen(str);
	short int cmp = memcmp(str_data, str, sizeof(char) * (str_size < n ? str_size : n));
	if (str_size == n)
		return cmp < 0;
	else if (str_size < n)
		return cmp == 0 ? true : cmp < 0;
	else
		return cmp == 0 ? false : cmp < 0;
}

bool MyString::operator>(const MyString& str) {
	short int cmp = memcmp(str_data, str.str_data, sizeof(char) * (str_size < str.str_size ? str_size : str.str_size));
	if (str_size == str.str_size)
		return cmp > 0;
	else if (str_size < str.str_size)
		return cmp == 0 ? false : cmp > 0;
	else
		return cmp == 0 ? true : cmp > 0;
}
bool MyString::operator>(const char* str) {
	size_t n = strlen(str);
	short int cmp = memcmp(str_data, str, sizeof(char) * (str_size < n ? str_size : n));
	if (str_size == n)
		return cmp > 0;
	else if (str_size < n)
		return cmp == 0 ? false : cmp > 0;
	else
		return cmp == 0 ? true : cmp > 0;
}
bool MyString::operator<=(const MyString& str) {
	short int cmp = memcmp(str_data, str.str_data, sizeof(char) * (str_size < str.str_size ? str_size : str.str_size));
	if (str_size == str.str_size)
		return cmp <= 0;
	else if (str_size < str.str_size)
		return cmp == 0 ? true : cmp <= 0;
	else
		return cmp == 0 ? false : cmp <= 0;
}

bool MyString::operator<=(const char* str) {
	size_t n = strlen(str);
	short int cmp = memcmp(str_data, str, sizeof(char) * (str_size < n ? str_size : n));
	if (str_size == n)
		return cmp <= 0;
	else if (str_size < n)
		return cmp == 0 ? true : cmp <= 0;
	else
		return cmp == 0 ? false : cmp <= 0;
}

bool MyString::operator>=(const MyString& str) {
	short int cmp = memcmp(str_data, str.str_data, sizeof(char) * (str_size < str.str_size ? str_size : str.str_size));
	if (str_size == str.str_size)
		return cmp >= 0;
	else if (str_size < str.str_size)
		return cmp == 0 ? false : cmp >= 0;
	else
		return cmp == 0 ? true : cmp >= 0;
}

bool MyString::operator>=(const char* str) {
	size_t n = strlen(str);
	short int cmp = memcmp(str_data, str, sizeof(char) * (str_size < n ? str_size : n));
	if (str_size == n)
		return cmp >= 0;
	else if (str_size < n)
		return cmp == 0 ? false : cmp >= 0;
	else
		return cmp == 0 ? true : cmp >= 0;
}

#endif