#pragma once
#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <iostream>
#include <stdexcept>
#include <functional>

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

template <typename T>
class AVLTreeIterator;
/**
* AVL 트리 클래스
* @date 2018.11.21
* @author 김성주
*/
template <typename T>
class AVLTree
{
private:
	friend class AVLTreeIterator<T>;

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

	AVLTreeNode<T>* SingleLeftRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* SingleRightRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* DoubleLeftRotate(AVLTreeNode<T>* node);

	AVLTreeNode<T>* DoubleRightRotate(AVLTreeNode<T>* node);

	void findNode(AVLTreeNode<T>*& node, std::function<bool(const T&)>& search, std::function<void(const T&)>& todo);

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

	void Find(std::function<bool(const T&)>& search, std::function<void(const T&)>& todo);
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
	return DeleteNode(root, data);
}

template <typename T>
int AVLTree<T>::Replace(const T& data)
{
	return ReplaceNode(root, data);
}

template <typename T>
int AVLTree<T>::Get(T& data)
{
	return GetNode(root, data);
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

template <typename T>
void AVLTree<T>::Find(std::function<bool(const T&)>& search, std::function<void(const T&)>& todo)
{
	findNode(root, search, todo);
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
		int success = AddNode(node->left, data); //왼쪽 subtree 업데이트

		if ((height(node->left) - height(node->right)) == 2) //불균형이 발생한 경우
		{
			if (data < node->left->data) node = SingleRightRotate(node); //data가 왼쪽 subtree의 왼쪽에 추가된 경우 = 단순 right rotation
			else node = DoubleRightRotate(node); //data가 왼쪽 subtree의 오른쪽에 추가된 경우 = 이중 right rotation
		}

		CalculateHeight(node);
		return success;
	}
	else if (node->data < data)
	{
		int success = AddNode(node->right, data); //오른쪽 subtree 업데이트

		if ((height(node->right) - height(node->left)) == 2) //불균형이 발생한 경우
		{

			if (data > node->right->data) node = SingleLeftRotate(node); //data가 오른쪽 subtree의 오른쪽에 추가된 경우 = 단순 left rotation
			else node = DoubleLeftRotate(node); //data가 오른쪽 subtree의 왼쪽에 추가된 경우 = 이중 left rotation
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

	if (node->data > data) //data가 작은 경우
		success = DeleteNode(node->left, data);
	else if (node->data < data) //data가 큰 경우
		success = DeleteNode(node->right, data);
	else //찾은 경우
	{
		if (node->left && node->right) //child가 둘인 경우
		{
			AVLTreeNode<T>* r = node->right;
			while (r->right) r = r->right;
			//현재 data보다 큰 값 중 가장 작은 값을 찾는다. (되도록 complete tree를 만들기 위함)

			node->data = r->data;
			delete r;
			r = nullptr;
		}
		else //child가 없거나 하나인 경우
		{
			AVLTreeNode<T>* temp = node;
			if (node->left == nullptr) node = node->right;
			else if (node->right == nullptr) node = node->left;
			delete temp;
			temp = nullptr;
		}
		return 1;
	}

	CalculateHeight(node);

	if (height(node->left) - height(node->right) == 2) //왼쪽 불균형
	{
		if (height(node->left->left) - height(node->left->right) == 1) //왼쪽의 왼쪽
			node = SingleRightRotate(node);
		else //왼쪽의 오른쪽
			node = DoubleRightRotate(node);
	}
	else if (height(node->right) - height(node->left) == 2) //오른쪽 불균형
	{
		if (height(node->right->right) - height(node->right->left) == 1) //오른쪽의 오른쪽
			node = SingleLeftRotate(node);
		else //오른쪽의 왼쪽
			node = DoubleLeftRotate(node);
	}

	return success;
}

template <typename T>
int AVLTree<T>::ReplaceNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;

	if (node->data > data) //데이터가 작은 경우
		return ReplaceNode(node->left, data);
	else if (node->data < data) //데이터가 큰 경우
		return ReplaceNode(node->right, data);

	//same data found
	node->data = data;
	return 1;
}

template <typename T>
int AVLTree<T>::GetNode(AVLTreeNode<T>*& node, T& data)
{
	if (node == nullptr) return 0;

	if (node->data > data) //데이터가 작은 경우
		return GetNode(node->left, data);
	else if (node->data < data) //데이터가 큰 경우
		return GetNode(node->right, data);

	//same data found
	data = node->data;
	return 1;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleLeftRotate(AVLTreeNode<T>* node)
{
	AVLTreeNode<T>* right = node->right;
	node->right = right->left;
	right->left = node;
	CalculateHeight(node);
	CalculateHeight(right);
	return right;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleRightRotate(AVLTreeNode<T>* node)
{
	AVLTreeNode<T>* left = node->left;
	node->left = left->right;
	left->right = node;
	CalculateHeight(node);
	CalculateHeight(left);
	return left;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleLeftRotate(AVLTreeNode<T>* node)
{
	node->right = SingleRightRotate(node->right);
	return SingleLeftRotate(node);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleRightRotate(AVLTreeNode<T>* node)
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

template <typename T>
void AVLTree<T>::findNode(AVLTreeNode<T>*& node, std::function<bool(const T&)>& search, std::function<void(const T&)>& todo)
{
	if (node == nullptr) return;

	findNode(node->left, search, todo);
	if (search(node->data)) todo(node->data);
	findNode(node->right, search, todo);
}
#endif