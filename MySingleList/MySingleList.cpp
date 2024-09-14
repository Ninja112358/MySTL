#define _CRT_SECURE_NO_WARNINGS
#include <cstring>
#include <iostream>
#include "MySingleList.h"

using namespace std;

class Student{
	friend ostream& operator<<(ostream& cout, Student& student);
	friend istream& operator>>(istream& cin, Student& student);
private:
	int id;
	char name[20];
	int score;
public:
	Student() {
		this->id = 0;
		strcpy(this->name, "");
		this->score = 0;
	}
	Student(int id, const char* name, int score) {
		this->id = id;
		strcpy(this->name, name);
		this->score = score;
	}
	void setId(int id) {
		this->id = id;
	}
	void setName(char* name) {
		strcpy(this->name, name);
	}
	void setScore(int score) {
		this->score = score;
	}
	int getId() {
		return id;
	}
	char* getName(){
		return name;
	}
	int getScore() {
		return score;
	}
	
};
ostream& operator<<(ostream& cout, Student& student) {
	cout << "学号:" << student.id << endl;
	cout << "姓名:" << student.name << endl;
	cout << "成绩:" << student.score << endl;
	return cout;
}
istream& operator>>(istream& cin, Student& student) {
	cout << "请输入学号:";
	cin >> student.id;
	cout << "请输入姓名:";
	cin >> student.name;
	cout << "请输入成绩:";
	cin >> student.score;
	return cin;
}

int main() {
	MySingleList<Student> list;
	MySingleList<Student>::iterator it;
	Student student;
	Student students[100];
	char name[20];
	int n,num,index;
	int k;		//在用姓名查找学生学号和成绩的时候，可以用于统计重名学生的个数
	cout << "1.根据指定学生个数，逐个输入学生信息" << endl;
	cout << "2.逐个显示学生表中所有学生的相关信息" << endl;
	cout << "3.根据姓名进行查找，返回此学生的学号和成绩(下标是从0开始的)" << endl;
	cout << "4.根据指定的位置可返回相应的学生信息（学号，姓名，成绩）" << endl;
	cout << "5.给定一个学生信息，插入到表中指定的位置(下标是从0开始的)" << endl;
	cout << "6.删除指定位置的学生记录" << endl;
	cout << "7.统计表中学生个数" << endl;
	cout << "输入-1退出程序" << endl;
	for (;;) {
		cout << "请输入您要进行的操作:";
		cin >> num;
		if (num == -1)
			break;
		switch (num) {
		case 1:
			cout << "请输入学生个数:";
			cin >> n;
			for (int i = 0; i < n; i++) {
				cin >> student;
				list.push_back(student);
				cout << endl;
			}
			break;
		case 2:
			for (it = list.begin(); it != list.end(); ++it)
				cout << it.node->value << endl;
			break;
		case 3:
			cout << "请输入您要查找的姓名:";
			cin >> name;
			k = 0;
			for (it = list.begin(); it != list.end(); ++it)
				if (strcmp(it.node->value.getName(), name) == 0)
					students[k++] = it.node->value;
			cout << "一共查询到" << k << "名叫" << name << "的学生" << endl;
			for (int i = 0; i < k; i++) {
				cout << "第" << i + 1 << "名学生的学号为:" << students[i].getId() << endl;
				cout << "第" << i + 1 << "名学生的成绩为:" << students[i].getScore() << endl;
				cout << endl;
			}
			break;
		case 4:
			cout << "请输入您要查询的下标:";
			cin >> index;
			cout << "该学生的信息为:" << endl;
			cout << list[index] << endl;
			break;
		case 5:
			cout << "请输入您要插入的下标:";
			cin >> index;
			cout << "请输入您要插入的学生的信息:" << endl;
			cin >> student;
			if (list.insert(index, student) == OK) {
				cout << "插入成功,插入后的信息为:" << endl;
				for (it = list.begin(); it != list.end(); ++it)
					cout << it.node->value << endl;
			}
			else
				cout << "插入失败" << endl;
			break;
		case 6:
			cout << "请输入您要删除的下标:";
			cin >> index;
			if (list.erase(index) == OK) {
				cout << "删除成功,删除后的信息为:" << endl;
				for (it = list.begin(); it != list.end(); ++it)
					cout << it.node->value << endl;
			}
			else
				cout << "删除失败" << endl;
			break;
		case 7:
			cout << "现在表中的学生个数为:" << list.size() << endl; 
			break;
		default:
			cout << "输入错误请重试" << endl;
			break;
		}
	}
}