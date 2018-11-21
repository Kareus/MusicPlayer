#pragma once
#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <iostream>
#include <stdexcept>

/**
* AVL Ʈ���� ���
*/
template <typename T>
struct AVLTreeNode
{
	T data;
	AVLTreeNode<T>* left;
	AVLTreeNode<T>* right;
	int height;
};

/**
* AVL Ʈ�� Ŭ����
* @date 2018.11.21
* @author �輺��
*/
template <typename T>
class AVLTree
{
private:
	friend void test(AVLTree<T>& a);
	AVLTreeNode<T>* root;

	void CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy);

	void ClearNode(AVLTreeNode<T>*& node);

	int CountNode(AVLTreeNode<T>*& node) const;

	int AddNode(AVLTreeNode<T>*& node, const T& data);

	int DeleteNode(AVLTreeNode<T>*& node, const T& data);

	int ReplaceNode(AVLTreeNode<T>*& node, const T& data);

	int GetNode(AVLTreeNode<T>*& node, T& data);

	void CalculateHeight(AVLTreeNode<T>*& node);

	int height(AVLTreeNode<T>*& node);

	AVLTreeNode<T>* SingleLeftRotate(AVLTreeNode<T>*& node);

	AVLTreeNode<T>* SingleRightRotate(AVLTreeNode<T>*& node);

	AVLTreeNode<T>* DoubleLeftRotate(AVLTreeNode<T>*& node);

	AVLTreeNode<T>* DoubleRightRotate(AVLTreeNode<T>*& node);

public:
	AVLTree();

	AVLTree(const AVLTree& tree);

	~AVLTree();

	void MakeEmpty();

	bool IsEmpty() const;

	bool IsFull();

	int Add(const T& data);

	int Delete(const T& data);

	int Replace(const T& data);

	int Get(T& data);

	int GetLength() const;

	AVLTree<T>& operator=(const AVLTree<T>& tree);

};

template <typename T>
AVLTree<T>::AVLTree()
{
	root = nullptr;
}

template <typename T>
AVLTree<T>::AVLTree(const AVLTree<T>& tree)
{
	MakeEmpty();
	CopyNode(root, tree.root);
}

template <typename T>
AVLTree<T>::~AVLTree()
{
	MakeEmpty();
}

template <typename T>
void AVLTree<T>::MakeEmpty()
{
	ClearNode(root);
}

template <typename T>
bool AVLTree<T>::IsEmpty() const
{
	return root == nullptr;
}

template <typename T>
bool AVLTree<T>::IsFull()
{
	try
	{
		AVLTreeNode<T>* node = new AVLTreeNode<T>;
		delete node;
		return false;
	}
	catch (std::bad_alloc& e)
	{
		std::cerr << "bad alloc exception: " << e.what() << std::endl;
		return true;
	}
}

template <typename T>
int AVLTree<T>::Add(const T& data)
{
	return AddNode(root, data);
}

template <typename T>
int AVLTree<T>::Delete(const T& data)
{
	DeleteNode(root, data);
}

template <typename T>
int AVLTree<T>::Replace(const T& data)
{
	ReplaceNode(root, data);
}

template <typename T>
int AVLTree<T>::Get(T& data)
{
	GetNode(root, data);
}

template <typename T>
int AVLTree<T>::GetLength() const
{
	return CountNode(root);
}

template <typename T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& tree)
{
	MakeEmpty();
	CopyNode(root, tree.root);
	return *this;
}

/* -------- recursive functions -------- */
template <typename T>
void AVLTree<T>::CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy)
{
	if (copy == nullptr) return;
	node = new AVLTreeNode<T>;
	node->data = copy->data;
	node->left = nullptr;
	node->right = nullptr;
	node->height = copy->height;
	CopyNode(node->left, copy->left);
	CopyNode(node->right, copy->right);
}

template <typename T>
void AVLTree<T>::ClearNode(AVLTreeNode<T>*& node)
{
	if (node == nullptr) return;
	ClearNode(node->left);
	ClearNode(node->right);
	delete node;
	node = nullptr;
}

template <typename T>
int AVLTree<T>::CountNode(AVLTreeNode<T>*& node) const
{
	if (node == nullptr) return 0;
	return 1 + CountNode(node->left) + CoundNode(node->right);
}

template <typename T>
int AVLTree<T>::AddNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr)
	{
		node = new AVLTreeNode<T>;
		node->data = data;
		node->left = nullptr;
		node->right = nullptr;
		node->height = 0;
		return 1;
	}
	else if (node->data > data) //data�� ���� ���
	{
		int success = AddNode(node->left, data); //���� subtree ������Ʈ

		if ((height(node->left) - height(node->right)) == 2) //�ұ����� �߻��� ���
		{
			if (data < node->left->data) node = SingleRightRotate(node); //data�� ���� subtree�� ���ʿ� �߰��� ��� = �ܼ� right rotation
			else node = DoubleRightRotate(node); //data�� ���� subtree�� �����ʿ� �߰��� ��� = ���� right rotation
		}

		CalculateHeight(node);
		return success;
	}
	else if (node->data < data)
	{
		int success = AddNode(node->right, data); //������ subtree ������Ʈ

		if ((height(node->right) - height(node->left)) == 2) //�ұ����� �߻��� ���
		{
			if (data > node->left->data) node = SingleLeftRotate(node); //data�� ������ subtree�� �����ʿ� �߰��� ��� = �ܼ� left rotation
			else node = DoubleLeftRotate(node); //data�� ������ subtree�� ���ʿ� �߰��� ��� = ���� left rotation
		}

		CalculateHeight(node);
		return success;
	}
	
	return 0; //same data found
}

template <typename T>
int AVLTree<T>::DeleteNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;
	int success = 0;

	if (node->data > data) //data�� ���� ���
		success = DeleteNode(node->left, data);
	else if (node->data < data) //data�� ū ���
		success = DeleteNode(node->right, data);
	else //ã�� ���
	{
		if (node->left && node->right) //child�� ���� ���
		{
			AVLTreeNode<T>* r = node->right;
			while (r->right) r = r->right;
			//���� data���� ū �� �� ���� ���� ���� ã�´�. (�ǵ��� complete tree�� ����� ����)

			node->data = r->data;
			delete r;
			r = nullptr;
		}
		else //child�� ���ų� �ϳ��� ���
		{
			AVLTreeNode<T>* temp = node;
			if (node->left == nullptr) node = node->right;
			else if (node->right == nullptr) node = node->left;
			delete temp;
			temp = nullptr;
		}

		CalculateHeight(node);

		if (height(node->left) - height(node->right) == 2) //���� �ұ���
		{
			if (height(node->left->left) - height(node->left->right) == 1) //������ ����
				node = SingleLeftRotate(node);
			else //������ ������
				node = DoubleLeftRotate(node);
		}
		else if (height(node->right) - height(node->left) == 2) //������ �ұ���
		{
			if (height(node->right->right) - height(node->right->left) == 1) //�������� ������
				node = SingleRightRotate(node);
			else //�������� ����
				node = DoubleRightRotate(node);
		}

	}
}

template <typename T>
int AVLTree<T>::ReplaceNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;

	if (node->data > data) //�����Ͱ� ���� ���
		return ReplaceNode(node->left, data);
	else if (node->data < data) //�����Ͱ� ū ���
		return ReplaceNode(node->right, data);

	//same data found
	node->data = data;
	return 1;
}

template <typename T>
int AVLTree<T>::GetNode(AVLTreeNode<T>*& node, T& data)
{
	if (node == nullptr) return 0;

	if (node->data > data) //�����Ͱ� ���� ���
		return GetNode(node->left, data);
	else if (node->data < data) //�����Ͱ� ū ���
		return GetNode(node->right, data);

	//same data found
	data = node->data;
	return 1;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleLeftRotate(AVLTreeNode<T>*& node)
{
	AVLTreeNode<T>* right = node->right;
	node->right = right->left;
	right->left = node;
	CalculateHeight(node);
	CalculateHeight(right);
	return right;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleRightRotate(AVLTreeNode<T>*& node)
{
	AVLTreeNode<T>* left = node->left;
	node->left = left->right;
	left->right = node;
	CalculateHeight(node);
	CalculateHeight(left);
	return left;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleLeftRotate(AVLTreeNode<T>*& node)
{
	node->right = SingleRightRotate(node->right);
	return SingleLeftRotate(node);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleRightRotate(AVLTreeNode<T>*& node)
{
	node->left = SingleLeftRotate(node->left);
	return SingleRightRotate(node);
}

template <typename T>
void AVLTree<T>::CalculateHeight(AVLTreeNode<T>*& node)
{
	if (node == nullptr) return;

	int leftH = -1;
	int rightH = -1;

	if (node->left != nullptr) leftH = node->left->height;
	if (node->right != nullptr) rightH = node->right->height;

	node->height = 1 + (leftH > rightH ? leftH : rightH);
}

template <typename T>
int AVLTree<T>::height(AVLTreeNode<T>*& node)
{
	return node == nullptr ? -1 : node->height;
}
#endif