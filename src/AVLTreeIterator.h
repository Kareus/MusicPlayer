#pragma once
#ifndef __AVLTREE_ITERATOR__
#define __AVLTREE_ITERATOR__

#include "AVLTree.h"

/**
*	@brief	AVLTree에서 사용하는 반복자 클래스
*	@author	Kareus
*/
template <typename T>
class AVLTreeIterator
{
private:
	const AVLTree<T>& m_list; ///<반복자에서 탐색할 트리
	AVLTreeNode<T>* node; ///<현재 노든

public:
	/**
	* 기본 연산자
	*/
	AVLTreeIterator(const AVLTree<T>& tree) : m_list(tree), node(tree.root) {};

	/**
	* 복사 연산자
	*/
	AVLTreeIterator(const AVLTreeIterator<T>& iter) : m_list(iter.m_list), node(iter.m_list.root) {};

	/**
	* 포인터를 초기화한다.
	*/
	void Reset();

	/**
	* 현재 노드가 NULL인지 반환한다.
	*/
	bool NotNull();

	/**
	* 현재 노드의 왼쪽 노드가 NULL인지 반환한다.
	*/
	bool LeftNotNull();

	/**
	* 현재 노드의 오른쪽 노드가 NULL인지 반환한다.
	*/
	bool RightNotNull();

	/**
	* 현재 노드의 데이터를 반환한다.
	*/
	T Current();

	/**
	* 현재 노드의 포인터를 반환한다.
	*/
	T* CurrentPtr();

	/**
	* 왼쪽 노드로 이동한다.
	*/
	void GotoLeft();

	/**
	* 오른쪽 노드로 이동한다.
	*/
	void GotoRight();

	/**
	* 할당 연산자
	*/
	AVLTreeIterator<T>& operator=(const AVLTreeIterator<T>& iter);
};

template <typename T>
void AVLTreeIterator<T>::Reset()
{
	node = m_list.root;
}

template <typename T>
bool AVLTreeIterator<T>::NotNull()
{
	return node != nullptr;
}

template <typename T>
bool AVLTreeIterator<T>::LeftNotNull()
{
	return node->left != nullptr;
}

template <typename T>
bool AVLTreeIterator<T>::RightNotNull()
{
	return node->right != nullptr;
}

template <typename T>
T AVLTreeIterator<T>::Current()
{
	return node->data;
}

template <typename T>
T* AVLTreeIterator<T>::CurrentPtr()
{
	return &node->data;
}

template <typename T>
void AVLTreeIterator<T>::GotoLeft()
{
	if (node != nullptr) node = node->left;
}

template <typename T>
void AVLTreeIterator<T>::GotoRight()
{
	if (node != nullptr) node = node->right;
}

template <typename T>
AVLTreeIterator<T>& AVLTreeIterator<T>::operator=(const AVLTreeIterator<T>& iter)
{
	AVLTreeIterator(iter); //const에는 할당이 불가능해 초기화를 해야하므로 생성자 호출
	node = iter.node;
	return *this;
}

#endif