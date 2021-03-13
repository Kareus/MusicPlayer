#pragma once
#ifndef __AVL_TREE__
#define __AVL_TREE__

#include <iostream>
#include <stdexcept>
#include <functional>
#include "GlobalFunctions.h"

/**
* AVL 트리의 노드
*/
template <typename T>
struct AVLTreeNode
{
	T data; ///<데이터
	AVLTreeNode<T>* left; ///<왼쪽 노드
	AVLTreeNode<T>* right; ///<오른쪽 노드
	int height; ///<높이
};

template <typename T>
class AVLTreeIterator;

/**
*	@brief	AVL 트리 클래스
*	@date	2018.11.21
*	@author	Kareus
*/
template <typename T>
class AVLTree
{
private:
	friend class AVLTreeIterator<T>; ///<반복자를 friend로 선언

	AVLTreeNode<T>* root; ///<루트 노드

	/**
	*	@brief	복사 생성을 위해 호출하는 재귀함수
	*	@param	node	복사한 노드를 저장할 노드의 포인터 주소
	*	@param	copy	복사해올 노드
	*/
	void CopyNode(AVLTreeNode<T>*& node, AVLTreeNode<T>* copy);

	/**
	*	@brief	메모리 해제를 위해 호출하는 재귀함수
	*	@param	node	메모리를 해제할 노드의 포인터 주소
	*/
	void ClearNode(AVLTreeNode<T>*& node);
	
	/**
	*	@brief	노드 개수를 세기 위해 호출하는 재귀함수
	*	@param	node	노드의 포인터 주소
	*/
	int CountNode(const AVLTreeNode<T>* node) const;

	/**
	*	@brief	노드 추가를 위해 호출하는 재귀함수
	*	@param	node	노드의 포인터 주소
	*/
	int AddNode(AVLTreeNode<T>*& node, const T& data);

	/**
	*	@brief	노드 삭제를 위해 호출하는 재귀함수
	*	@param	node	노드의 포인터 주소
	*/
	int DeleteNode(AVLTreeNode<T>*& node, const T& data);

	/**
	*	@brief	노드 교체를 위해 호출하는 재귀함수
	*	@param	node	노드의 포인터 주소
	*/
	int ReplaceNode(AVLTreeNode<T>*& node, const T& data);

	/**
	*	@brief	노드 데이터 반환을 위해 호출하는 재귀함수
	*	@param	node	노드의 포인터 주소
	*/
	int GetNode(AVLTreeNode<T>*& node, T& data);

	/**
	*	@brief	노드의 높이를 계산하는 함수
	*	@param	node	높이를 계산할 노드
	*/
	void CalculateHeight(AVLTreeNode<T>*& node);

	/**
	*	@brief	해당 노드의 높이를 반환하는 함수
	*	@param	node	노드의 포인터 주소
	*	@return	해당 노드가 NULL이면 -1, 아니면 그 노드의 높이를 반환한다.
	*/
	int height(AVLTreeNode<T>*& node);

	/**
	*	@brief	단순 left rotation을 실행하는 함수
	*	@param	node	회전을 실행할 노드
	*	@return	회전 후의 노드를 반환한다.
	*/
	AVLTreeNode<T>* SingleLeftRotate(AVLTreeNode<T>* node);

	/**
	*	@brief	단순 right rotation을 실행하는 함수
	*	@param	node	회전을 실행할 노드
	*	@return	회전 후의 노드를 반환한다.
	*/
	AVLTreeNode<T>* SingleRightRotate(AVLTreeNode<T>* node);

	/**
	*	@brief	이중 left rotation을 실행하는 함수
	*	@param	node	회전을 실행할 노드
	*	@return	회전 후의 노드를 반환한다.
	*/
	AVLTreeNode<T>* DoubleLeftRotate(AVLTreeNode<T>* node);

	/**
	*	@brief	이중 right rotation을 실행하는 함수
	*	@param	node	회전을 실행할 노드
	*	@return	회전 후의 노드를 반환한다.
	*/
	AVLTreeNode<T>* DoubleRightRotate(AVLTreeNode<T>* node);

	/**
	*	@brief	노드가 조건을 충족하면 특정 함수를 실행하는 재귀함수
	*	@param	node	조건 충족을 시험할 노드
	*	@param	search	조건 충족을 반환하는 함수
	*	@param	todo	조건을 충족하면 실행할 함수
	*/
	void findNode(AVLTreeNode<T>*& node, const std::function<bool(T&)>& search, const std::function<void(T&)>& todo);

	/**
	*	@brief	노드에 대해 특정 함수를 실행하는 재귀함수
	*	@param	node	시험할 노드
	*	@param	todo	실행할 함수
	*/
	void doNode(AVLTreeNode<T>*& node, const std::function<void(T&)>& todo);

	std::function<int(const T&, const T&)> compareFunc; ///< 데이터 비교 함수

public:

	/**
	*	기본 생성자
	*/
	AVLTree();

	/**
	*	복사 생성자
	*/
	AVLTree(const AVLTree& tree);

	/**
	*	기본 소멸자
	*/
	~AVLTree();

	/**
	*	@brief	트리를 비우는 함수
	*/
	void MakeEmpty();

	/**
	*	@brief	트리가 비어있는지 반환한다
	*	@return	트리가 비어있으면 true, 아니면 false를 반환한다.
	*/
	bool IsEmpty() const;

	/**
	*	@brief	트리가 꽉 차 있는지 반환한다
	*	@return	트리가 꽉 차 있으면 true, 아니면 false를 반환한다.
	*/
	bool IsFull();

	/**
	*	@brief	트리에 데이터를 추가한다.
	*	@param	data	추가할 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int Add(const T& data);

	/**
	*	@brief	트리에서 데이터를 삭제한다.
	*	@param	data	삭제할 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int Delete(const T& data);

	/**
	*	@brief	트리에서 데이터를 교체한다.
	*	@param	data	교체할 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int Replace(const T& data);

	/**
	*	@brief	트리에서 데이터를 찾는다.
	*	@param	data	찾은 데이터를 저장할 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int Get(T& data);

	/**
	*	@brief	트리의 노드 개수를 반환한다.
	*	@return	노드 개수
	*/
	int GetLength() const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	tree	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	AVLTree<T>& operator=(const AVLTree<T>& tree);

	/**
	*	@brief	조건을 만족하는 노드를 찾아 특정 함수를 실행한다.
	*	@param	search	조건 만족을 시험할 함수
	*	@param	todo	조건을 만족하면 시행할 함수
	*/
	void Find(const std::function<bool(T&)>& search, const std::function<void(T&)>& todo);
	
	/**
	*	@brief	모든 노드에 대해 특정 함수를 실행한다.
	*	@param	todo	조건을 만족하면 시행할 함수
	*/
	void Do(const std::function<void(T&)>& todo);

	/**
	*	@brief	데이터 비교 함수를 설정한다.
	*	@pre	비교 함수는 t1, t2를 비교할 때 t1 > t2이면 양수, t1 = t2이면 0, t1 < t2이면 음수를 반환해야 한다.
	*	@post	데이터 비교 함수가 parameter의 함수로 설정된다.
	*	@param	func	설정할 데이터 비교 함수
	*/
	void SetCompareFunction(const std::function<int(const T&, const T&)>& func);
};

template <typename T>
AVLTree<T>::AVLTree()
{
	root = nullptr;
	compareFunc = [](const T& t1, const T& t2) {
		return (t1 > t2) - (t1 < t2);
	};
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
		System::AlertError("bad alloc exception: " + e.what() + '\n');
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
void AVLTree<T>::Find(const std::function<bool(T&)>& search, const std::function<void(T&)>& todo)
{
	findNode(root, search, todo);
}

template <typename T>
void AVLTree<T>::Do(const std::function<void(T&)>& todo)
{
	doNode(root, todo);
}

template <typename T>
void AVLTree<T>::SetCompareFunction(const std::function<int(const T&, const T&)>& func)
{
	compareFunc = func;
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
int AVLTree<T>::CountNode(const AVLTreeNode<T>* node) const
{
	if (node == nullptr) return 0;
	return 1 + CountNode(node->left) + CountNode(node->right);
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
	
	int compare = compareFunc(node->data, data);
	if (compare > 0) //data가 작은 경우
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
	else if (compare < 0) //data가 큰 경우
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

	int compare = compareFunc(node->data, data);

	if (compare > 0) //data가 작은 경우
		success = DeleteNode(node->left, data);
	else if (compare < 0) //data가 큰 경우
		success = DeleteNode(node->right, data);
	else //찾은 경우
	{
		success = 1;
		if (node->left && node->right) //child가 둘인 경우
		{
			AVLTreeNode<T>* r = node->right;
			AVLTreeNode<T>* n = node; //r의 부모 노드

			bool leftChild = false;

			while (r->left)
			{
				n = r;
				r = r->left;
				leftChild = true;
			}
			//현재 data보다 큰 값 중 가장 작은 값을 찾는다. (되도록 complete tree를 만들기 위함)
			//r은 leaf node임이 보장된다.

			node->data = r->data;
			delete r;

			if (leftChild) n->left = nullptr; //부모 노드가 dangling pointer를 가리키지 않도록 방지
			else n->right = nullptr; //right node가 leaf node인 경우
		}
		else //child가 없거나 하나인 경우
		{
			AVLTreeNode<T>* temp = node;
			if (node->left == nullptr) node = node->right;
			else if (node->right == nullptr) node = node->left;
			delete temp;

			if (root == temp) root = nullptr; //최상위 노드인 경우 root가 dangling pointer를 가리키지 않도록 방지 (노드가 root 하나 뿐이므로)
			//이외 발생하는 dangling pointer는 회전 과정에서 해결된다.

			return 1; //child가 하나인 경우엔 node가 overwrite되고, 이 node는 leaf node가 되므로 회전 과정 불필요
			//child가 없는 경우엔 node가 delete되고 nullptr이 됨. 회전 과정이 불필요할 뿐더러, 접근 자체가 불가능
		}
	}

	CalculateHeight(node); //삭제 실행 후 height을 계산

	if (height(node->left) - height(node->right) == 2) //왼쪽 불균형
	{
		if (height(node->left->left) - height(node->left->right) == 1) //왼쪽의 왼쪽 (left left case)
			node = SingleRightRotate(node);
		else //왼쪽의 오른쪽 (left right case)
			node = DoubleRightRotate(node);
	}
	else if (height(node->right) - height(node->left) == 2) //오른쪽 불균형
	{
		if (height(node->right->right) - height(node->right->left) == 1) //오른쪽의 오른쪽 (right right case)
			node = SingleLeftRotate(node);
		else //오른쪽의 왼쪽 (right left case)
			node = DoubleLeftRotate(node);
	}

	return success;
}

template <typename T>
int AVLTree<T>::ReplaceNode(AVLTreeNode<T>*& node, const T& data)
{
	if (node == nullptr) return 0;

	int compare = compareFunc(node->data, data);
	if (compare > 0) //데이터가 작은 경우
		return ReplaceNode(node->left, data);
	else if (compare < 0) //데이터가 큰 경우
		return ReplaceNode(node->right, data);

	//same data found
	node->data = data;
	return 1;
}

template <typename T>
int AVLTree<T>::GetNode(AVLTreeNode<T>*& node, T& data)
{
	if (node == nullptr) return 0;

	int compare = compareFunc(node->data, data);
	if (compare > 0) //데이터가 작은 경우
		return GetNode(node->left, data);
	else if (compare < 0) //데이터가 큰 경우
		return GetNode(node->right, data);

	//same data found
	data = node->data;
	return 1;
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::SingleLeftRotate(AVLTreeNode<T>* node)
{
	//단순 left rotation. 오른쪽 subtree의 오른쪽 node에 불균형이 발생한 경우. (outside, right right case)
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
	//단순 right rotation. 왼쪽 subtree의 왼쪽 node에 불균형이 발생한 경우. (outside, left left case)
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
	//이중 left rotation. 오른쪽 subtree의 왼쪽 node에 불균형이 발생한 경우. (inside, right left case)
	node->right = SingleRightRotate(node->right);
	return SingleLeftRotate(node);
}

template <typename T>
AVLTreeNode<T>* AVLTree<T>::DoubleRightRotate(AVLTreeNode<T>* node)
{
	//이중 right rotation. 왼쪽 subtree의 오른쪽 node에 불균형이 발생한 경우. (inside, left right case)
	node->left = SingleLeftRotate(node->left);
	return SingleRightRotate(node);
}

template <typename T>
void AVLTree<T>::CalculateHeight(AVLTreeNode<T>*& node)
{
	//해당 노드의 height를 계산
	if (node == nullptr) return; //null이면 return

	int leftH = -1; //기본값 -1 (left가 null인 경우)
	int rightH = -1; //기본값 -1 (right가 null인 경우)

	if (node->left != nullptr) leftH = node->left->height; //null이 아니면 left의 height를 가져옴
	if (node->right != nullptr) rightH = node->right->height; //null이 아니면 right의 height를 가져옴

	node->height = 1 + (leftH > rightH ? leftH : rightH); //둘 중 더 큰 값 + 1로 설정 (null이 -1, leaf node가 0이기 때문)
}

template <typename T>
int AVLTree<T>::height(AVLTreeNode<T>*& node)
{
	return node == nullptr ? -1 : node->height; //node->height은 nullptr인 경우 에러가 발생하므로 분기 판단하여 -1 혹은 height를 반환함
}

template <typename T>
void AVLTree<T>::findNode(AVLTreeNode<T>*& node, const std::function<bool(T&)>& search, const std::function<void(T&)>& todo)
{
	if (node == nullptr) return; //node가 null이면 return

	findNode(node->left, search, todo); //왼쪽 노드에 대한 search and do something 실행
	if (search(node->data)) todo(node->data); //search function에 parameter로 현재 노드의 아이템을 넣고 실행. 결과가 true면 todo 함수를 실행.
	findNode(node->right, search, todo); //오른쪽 노드에 대한 search and do something 실행
}

template <typename T>
void AVLTree<T>::doNode(AVLTreeNode<T>*& node, const std::function<void(T&)>& todo)
{
	if (node == nullptr) return;
	doNode(node->left, todo);
	todo(node->data);
	doNode(node->right, todo);
}
#endif