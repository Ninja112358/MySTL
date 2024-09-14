#ifndef MYBINARYTREE_H
#define MYBINARYTREE_H

#include <vector>

template<class T>
class MyBinaryTree{
	void create_tree_dfs(std::vector<T>)
public:
	MyBinaryTree();
	~MyBinaryTree();
	typedef struct TREENODE {
		T value;
		TREENODE* lchild;
		TREENODE* rchild;
	}TreeNode;
	TreeNode* root;
	void clear(TreeNode* root);
	void create_tree(std::vector<T> v);
};

template<class T>
MyBinaryTree<T>::MyBinaryTree(){
	root = new TreeNode;
	root->lchild = NULL;
	root->rchild = NULL;
}

template<class T>
MyBinaryTree<T>::~MyBinaryTree(){
	clear(root);
	delete root;
}

template<class T>
void MyBinaryTree<T>::clear(TreeNode* root){
	if (root == NULL) {
		return;
	}
	clear(root->lchild);
	clear(root->rchild);
	if (root->lchild == NULL && root->rchild == NULL)
		delete root;
}

template<class T>
bool MyBinaryTree<T>::create_tree(std::vector<T> v){
}

#endif