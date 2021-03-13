#pragma once
#ifndef __AVLTREE_ITERATOR__
#define __AVLTREE_ITERATOR__

#include "AVLTree.h"

/**
*	@brief	AVLTree���� ����ϴ� �ݺ��� Ŭ����
*	@author	Kareus
*/
template <typename T>
class AVLTreeIterator
{
private:
	const AVLTree<T>& m_list; ///<�ݺ��ڿ��� Ž���� Ʈ��
	AVLTreeNode<T>* node; ///<���� ���

public:
	/**
	* �⺻ ������
	*/
	AVLTreeIterator(const AVLTree<T>& tree) : m_list(tree), node(tree.root) {};

	/**
	* ���� ������
	*/
	AVLTreeIterator(const AVLTreeIterator<T>& iter) : m_list(iter.m_list), node(iter.m_list.root) {};

	/**
	* �����͸� �ʱ�ȭ�Ѵ�.
	*/
	void Reset();

	/**
	* ���� ��尡 NULL���� ��ȯ�Ѵ�.
	*/
	bool NotNull();

	/**
	* ���� ����� ���� ��尡 NULL���� ��ȯ�Ѵ�.
	*/
	bool LeftNotNull();

	/**
	* ���� ����� ������ ��尡 NULL���� ��ȯ�Ѵ�.
	*/
	bool RightNotNull();

	/**
	* ���� ����� �����͸� ��ȯ�Ѵ�.
	*/
	T Current();

	/**
	* ���� ����� �����͸� ��ȯ�Ѵ�.
	*/
	T* CurrentPtr();

	/**
	* ���� ���� �̵��Ѵ�.
	*/
	void GotoLeft();

	/**
	* ������ ���� �̵��Ѵ�.
	*/
	void GotoRight();

	/**
	* �Ҵ� ������
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
	AVLTreeIterator(iter); //const���� �Ҵ��� �Ұ����� �ʱ�ȭ�� �ؾ��ϹǷ� ������ ȣ��
	node = iter.node;
	return *this;
}

#endif