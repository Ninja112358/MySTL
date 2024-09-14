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
	//������Ľṹ
	typedef struct NODE {
		T value;
		NODE* next;
	}Node;
	//�����±����Node�ĵ�ַ���������ĸ����±�����ɾ���Ȳ���
	Node* query(size_t index) {
		Node* node = head->next;
		for (size_t i = 0; i < index && node != NULL; node = node->next, i++);
		return node;
	}
private:
	Node* head;		//�����ͷָ��
	Node* tail;		//�����βָ��
	size_t length;		//�洢����ĳ���
public:
	//��װһ��������ĵ�����
	//���������ڲ�����O(1)��ʱ�临�Ӷ���ǰ��,��������ĵ�����ֻ�ṩ++����
	class iterator {
	public:
		Node* node;
		//ǰ����
		Node* operator++() {
			node = node->next;
			return node;
		}
		//������(���ж��⿪��,���Ի���ʹ��ǰ�����ȽϺ�)
		Node* operator++(int) {
			Node* temp = node;
			node = node->next;
			return temp;
		}
		//��ֵ
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
		//�ӷ�(�����Խ������Զ���������һ��Ԫ�ص�λ��)
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
	//ʵ��һ�¿������캯��,ʵ�����,����������Դ���ǳ��������
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
	bool empty();									//�����п�
	size_t size();									//����ĳ���
	void push_head(T value);						//�����ͷ��
	void push_back(T value);						//�����β��
	void insert(Node* node, T value);				//����Ĳ���(��node�������)
	Status insert(size_t index, T value);			//����Ĳ���(��indexλ�ò���)
	Status pop_back();								//�����βɾ
	void erase(Node* node);							//�����ɾ��(ɾ��nodeλ�õ�ֵ),����Ҫǰ��������ɾ����λ��Ԫ��,ȱ���ǲ���βɾ,����Ҫ���ж�
	Status erase(size_t index);						//�����ɾ��(ɾ��indexλ�õ�ֵ)
	T& operator[](size_t index);					//����Ĳ��Һ͸���
	void clear();									//��������
	MySingleList<T> merge(MySingleList<T>& list);	//������������ϲ�
	size_t FindIndex(T value);						//����ֵ���ҵ�һ�γ��ֵ��±�

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
//������������ĺϲ�:�����Լ���װ�˵�����������ֱ���õ�����ʵ�ֺϲ���
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