#ifndef MYSINGLELIST_H
#define MYSINGLELIST_H

#define OK 1
#define ERROR 0

#ifndef OVERFLOW
#define OVERFLOW 3
#endif

typedef int Status;

template<class T>
class MySingleList {
private:
	//链表结点的结构
	typedef struct NODE {
		T value;
		NODE* next;
	}Node;
	//根据下标查找Node的地址，方便后面的根据下标插入和删除等操作
	Node* query(size_t index) {
		Node* node = head->next;
		for (size_t i = 0; i < index && node != NULL; node = node->next, i++);
		return node;
	}
private:
	Node* head;		//链表的头指针
	Node* tail;		//链表的尾指针
	size_t length;		//存储链表的长度
public:
	//封装一个单链表的迭代器
	//单链表由于不能以O(1)的时间复杂度往前推,所以这里的迭代器只提供++操作
	class iterator {
	public:
		Node* node;
		//前自增
		Node* operator++() {
			node = node->next;
			return node;
		}
		//后自增(会有额外开销,所以还是使用前自增比较好)
		Node* operator++(int) {
			Node* temp = node;
			node = node->next;
			return temp;
		}
		//赋值
		void operator=(iterator iterator) {
			node = iterator.node;
		}
		bool operator==(iterator iterator) {
			if (node == iterator.node)
				return true;
			return false;
		}
		bool operator!=(iterator iterator) {
			if (node == iterator.node)
				return false;
			return true;
		}
		//加法(如果加越界了永远保持在最后一个元素的位置)
		iterator operator+(size_t num) {
			iterator iterator;
			for (size_t i = 0; i < num && node->next != NULL; i++)
				node = node->next;
			iterator.node = node;
			return iterator;
		}
		T operator*() {
			return node->value;
		}
	};
	MySingleList();
	//实现一下拷贝构造函数,实现深拷贝,解决编译器自带的浅拷贝问题
	MySingleList(const MySingleList<T>& list) {
		head = new Node;
		head->next = NULL;
		tail = NULL;
		length = 0;
		for (Node* temp = list.head->next; temp != NULL; temp = temp->next)
			push_back(temp->value);
	}
	~MySingleList();

	void operator=(MySingleList<T> list) {
		clear();
		for (Node* temp = list.head->next; temp != NULL; temp = temp->next)
			push_back(temp->value);
	}
	bool empty();									//链表判空
	size_t size();									//链表的长度
	void push_head(T value);						//链表的头插
	void push_back(T value);						//链表的尾插
	void insert(Node* node, T value);				//链表的插入(在node后面插入)
	Status insert(size_t index, T value);			//链表的插入(在index位置插入)
	Status pop_back();								//链表的尾删
	void erase(Node* node);							//链表的删除(删除node位置的值),不需要前驱结点进行删除此位置元素,缺点是不能尾删,所以要加判断
	Status erase(size_t index);						//链表的删除(删除index位置的值)
	T& operator[](size_t index);					//链表的查找和更改
	void clear();									//链表的清空
	MySingleList<T> merge(MySingleList<T>& list);	//两个有序链表合并
	size_t FindIndex(T value);						//根据值查找第一次出现的下标

	iterator begin() {
		iterator it;
		it.node = head->next;
		return it;
	}
	iterator end() {
		iterator it;
		if (tail != NULL)
			it.node = tail->next;
		else
			it.node = NULL;
		return it;
	}
};

template<class T>
bool MySingleList<T>::empty(){
	if(head->next == NULL)
		return true;
	return false;
}

template<class T>
size_t MySingleList<T>::size(){
	return length;
}

template<class T>
MySingleList<T>::MySingleList() {
	head = new Node;
	head->next = NULL;
	tail = NULL;
	length = 0;
}

template<class T>
void MySingleList<T>::push_head(T value) {
	Node* node = new Node;
	node->value = value;
	if (head->next == NULL)
		tail = node;
	node->next = head->next;
	head->next = node;
	length++;
}
template<class T>
void MySingleList<T>::push_back(T value) {
	if (tail == NULL)
		push_head(value);
	else {
		Node* node = new Node;
		node->value = value;
		node->next = tail->next;
		tail->next = node;
		tail = node;
		length++;
	}
}
template<class T>
void MySingleList<T>::insert(Node* node, T value){
	Node* temp = new Node;
	temp->value = value;
	temp->next = node->next;
	node->next = temp;
}
template<class T>
Status MySingleList<T>::insert(size_t index, T value){
	if (index < 0 || index > length)
		return ERROR;
	if (index == 0)
		push_head(value);
	else if (index == length)
		push_back(value);
	else {
		Node* node = query(index - 1);
		insert(node, value); 
		length++;
	}
	return OK;
}
template<class T>
Status MySingleList<T>::pop_back(){
	if (length <= 0)
		return ERROR;
	if (length == 1) {
		clear();
	}
	else {
		Node* node = query(length - 2);
		tail = node;
		Node* temp = node->next;
		node->next = node->next->next;
		free(temp);
		temp = NULL;
		length--;
	}
	return OK;
}
template<class T>
void MySingleList<T>::erase(Node* node){
	if (node->next == NULL)
		pop_back();
	else{
		if (node->next->next == NULL)
			tail = node;
		node->value = node->next->value;
		Node* temp = node->next;
		node->next = node->next->next;
		free(temp);
		temp = NULL;
		length--;
	}
	
}
template<class T>
Status MySingleList<T>::erase(size_t index){
	if (index < 0 || index >= length)
		return ERROR;

	if (index >= length - 1)
		pop_back();
	else {
		Node* node = query(index);
		erase(node);
	}
	return OK;
}

template<class T>
T& MySingleList<T>::operator[](size_t index){
	return query(index)->value;
}
template<class T>
void MySingleList<T>::clear() {
	Node* i = head;
	Node* temp;
	while (i != NULL) {
		temp = i;
		i = i->next;
		free(temp);
		temp = NULL;
	}
	head = new Node;
	head->next = NULL;
	length = 0;
	tail = NULL;
}

template<class T>
MySingleList<T>::~MySingleList(){
	Node* i = head;
	Node* temp;
	while (i != NULL) {
		temp = i;
		i = i->next;
		free(temp);
		temp = NULL;
	}
}

template<class T>
size_t MySingleList<T>::FindIndex(T value){
	size_t index = 0;
	for (Node* node = head->next; node != NULL; node = node->next,index++)
		if (node->value == value)
			return index;
	return -1;
}
//两个有序链表的合并:由于自己封装了迭代器，所以直接用迭代器实现合并了
template<class T>
MySingleList<T> MySingleList<T>::merge(MySingleList<T> &list) {
	MySingleList<T> L;
	MySingleList<T>::iterator it1 = begin();
	MySingleList<T>::iterator it2 = list.begin();
	while (it1 != end() && it2 != list.end()) {
		if (*it1 < *it2) {
			L.push_back(*it1);
			++it1;
		}
		else {
			L.push_back(*it2);
			++it2;
		}
	}
	while (it1 != end()) {
		L.push_back(*it1);
		++it1;
	}
	while (it2 != list.end()) {
		L.push_back(*it2);
		++it2;
	}
	return L;
}
#endif