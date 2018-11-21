#pragma once
#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <iostream>
#include <stdexcept>

/**
* AVL 트리의 노드
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
* AVL 트리 클래스
* @date 2018.11.21
* @author 김성주
*/
template <typename T>
class AVLTree
{
private:
	AVLTreeNode<T>* root;

	void CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy);

	void ClearNode(AVLTreeNode<T>*& node);

	int CountNode(AVLTreeNode<T>*& node) const;

	int AddNode(AVLTreeNode<T>*& node, const T& data);

	int DeleteNode(AVLTreeNode<T>*& node, const T& data);

	int ReplaceNode(AVLTreeNode<T>*& node, const T& data);

	int GetNode(AVLTreeNode<T>*& node, T& data);

	void CalculateHeight(AVLTreeNode<T>*& node);

	AVLTreeNode* GetMinNode(AVLTreeNode<T>*& node);

	AVLTreeNode* GetMaxNode(AVLTreeNode<T>*& node);

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
	else if (node->data > data) //data가 작은 경우
	{
		int success = AddNode(node->left, data, success); //왼쪽 subtree 업데이트

		if ((node->left->height - node->right->height) == 2) //불균형이 발생한 경우
		{
			if (data < node->left->data) node = SingleRightRotate(node); //data가 왼쪽 subtree의 왼쪽에 추가된 경우 = 단순 right rotation
			else node = DoubleRightRotate(node); //data가 왼쪽 subtree의 오른쪽에 추가된 경우 = 이중 right rotation
		}

		CalculateHeight(node);
		return success;
	}
	else if (node->data < data)
	{
		int success = AddNode(node->right, data, success); //오른쪽 subtree 업데이트

		if ((node->right->height - node->left->height) == 2) //불균형이 발생한 경우
		{
			if (data > node->left->data) node = SingleLeftRotate(node); //data가 오른쪽 subtree의 오른쪽에 추가된 경우 = 단순 left rotation
			else node = DoubleLeftRotate(node); //data가 오른쪽 subtree의 왼쪽에 추가된 경우 = 이중 left rotation
		}

		CalculateHeight(node);
		return success;
	}
	
	return 0; //same data found
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
#endif