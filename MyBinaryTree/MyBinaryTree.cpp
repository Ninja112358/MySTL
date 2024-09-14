#include <iostream>

using namespace std;

typedef char ElemType;

typedef struct TREENODE{
	ElemType data;
	TREENODE* lchild;
	TREENODE* rchild;
}TreeNode;

bool CreateBinaryTree(TreeNode* root) {
	if (root == NULL)
		return false;
	ElemType value;
	cin >> value;
	if (value == '#')
		return false;
	root->data = value;
	root->lchild = new TreeNode;
	root->rchild = new TreeNode;
	if (!CreateBinaryTree(root->lchild)) {
		free(root->lchild);
		root->lchild = NULL;
	}
	if (!CreateBinaryTree(root->rchild)) {
		free(root->rchild);
		root->rchild = NULL;
	}
	return true;
}
void  DestroyBinaryTree(TreeNode* root) {
	if (root == NULL)
		return;
	DestroyBinaryTree(root->lchild);
	DestroyBinaryTree(root->rchild);
	delete root;
}

void  PreOrderTraverseRecursion(TreeNode* root) {
	if (root == NULL)
		return;
	cout << root->data << " ";
	PreOrderTraverseRecursion(root->lchild);
	PreOrderTraverseRecursion(root->rchild);
}

void  MidOrderTraverseRecursion(TreeNode* root) {
	if (root == NULL)
		return;
	MidOrderTraverseRecursion(root->lchild);
	cout << root->data << " ";
	MidOrderTraverseRecursion(root->rchild);
}

void  PostOrderTraverseRecursion(TreeNode* root) {
	if (root == NULL)
		return;
	PostOrderTraverseRecursion(root->lchild);
	PostOrderTraverseRecursion(root->rchild);
	cout << root->data << " ";
}

//测试数据:4 3 2 # # 1 6 # # # 8 # 7 # #
int main() {
	TreeNode* root = new TreeNode;
	//利用先序遍历建树
	CreateBinaryTree(root);
	cout << "递归先序遍历:";
	PreOrderTraverseRecursion(root);
	cout << endl;
	cout << "递归中序遍历:";
	MidOrderTraverseRecursion(root);
	cout << endl;
	cout << "递归后序遍历:";
	PostOrderTraverseRecursion(root);
	cout << endl;

	//利用后序遍历free树
	DestroyBinaryTree(root);
	return 0;
}