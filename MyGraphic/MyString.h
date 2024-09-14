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
	char* data();		//����char*���͵��ַ���
	char& front();		//����ͷ��
	char& back();		//����β��
	char& at(size_t index);
	void insert(size_t index, const MyString& str);		//��indexλ�ò���һ���ַ���
	void insert(size_t index, const char* str);			//ͬ��,����������char*
	void erase(size_t index, size_t n);					//ɾ����indexλ�ÿ�ʼ��n���ַ�,��stl��eraseһ��
	void erase(size_t index);							//ɾ��indexλ��֮��������ַ�,��stl��Ҳһ��
	size_t find(const MyString& str, size_t index = 0);	//�����ַ���str��һ�γ��ֵ�λ��
	size_t find(const char* str, size_t index = 0);		//ͬ��,����������char*
	void replace(size_t index, size_t n, const MyString& str);	//�滻��indexλ�ÿ�ʼ��n���ַ�Ϊstr,��stl��replaceһ��
	void replace(size_t index, size_t n, const char* str);		//ͬ��,����������char*


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
	//Ϊ������stl��������õĽ�ȡ�ַ�������stl�е�һģһ��
	MyString substr(size_t startIndex, size_t counts);
	//������ڴ���������ȫ������2���ݴΣ�����ֻ�ǵ�ǰ����һ���ڴ�
	//���������ݵ�ʱ���Զ��Ὣ�ڴ�ռ��Զ���������ٽ���2���ݴ�
	void reserve(size_t size) {
		//�������ڴ��ʵ����������С����ôֱ���˳�
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
	size_t capacity();		//���ص�ǰ�����������ʵ�ڴ�

	//���������
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
			str.push_back(c);//����
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
		//���temp�ǿ������ڴ����Ϊ�յ������ݶ�ʧ������
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
		//���temp�ǿ������ڴ����Ϊ�յ������ݶ�ʧ������
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