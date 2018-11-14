#pragma once
#ifndef _DOUBLYITERATOR_H
#define _DOUBLYITERATOR_H

#include "DoublyLinkedList.h"

template<typename T>
struct DoublyNodeType;
template<typename T>
class DoublyLinkedList;

/**
*	���Ḯ��Ʈ���� ���̴� Iterator�� Ŭ����.
*   sample code���� ������ ��
*   1. Next ȣ�� �ÿ� Next�� null pointer�� ��� data ��ȯ���� ���� �߻�. �̿� ���� data�� ��ȯ���� �ʵ��� ������
*   2. current pointer�� data�� ��ȯ�ϴ� Current �Լ��� �߰���.
*   3. Doubly Linked List���� ����� Iterator�̹Ƿ�, Prev �Լ��� �߰���.
*/

template <typename T>
class DoublyIterator
{
	friend class DoublyLinkedList<T>;
public:
	/**
	*	����Ʈ ������.
	*/
	DoublyIterator(const DoublyLinkedList<T> &list) : m_List(list), m_pCurPointer(list.m_pFirst)
	{}; ///< DoublyLinkedList�� ģ�� Ŭ����.


	/**
	*  ���� pointer�� NULL�� �ƴϸ� true, NULL�̸� false�� ��ȯ��.
	*/
	bool NotNull();

	/**
	*  ���� pointer�� ���� pointer�� NULL�� �ƴϸ� true, NULL�̸� false�� ��ȯ��.
	*/
	bool NextNotNull();

	/**
	*  ù��° Node�� data�� ��ȯ��.
	*/
	T First();

	/**
	*  Next pointer�� NULL�� �ƴ��� Ȯ���� �� �����Ƿ�, pointer�� �ѱ�⸸ ��. data ������ CurrentNode Ȥ�� Current������ �����ϵ��� ����.
	*/
	void Next();

	/**
	*  ���� �����ͷ� �̵��ϴ� �Լ�
	*/
	void Prev();

	/**
	*  ���� point�� data�� ��ȯ��.
	*/
	T Current();

	/**
	*  ���� pointer�� node�� ��ȯ��. (pass by value)
	*/
	DoublyNodeType<T> GetCurrentNode();
private:
	const DoublyLinkedList<T> &m_List;
	DoublyNodeType<T>* m_pCurPointer;
};

template <typename T>
bool DoublyIterator<T> :: NotNull()
{
	if(m_pCurPointer == NULL)
		return false;
	else
		return true;
}

template <typename T>
bool DoublyIterator<T> :: NextNotNull()
{
	if(m_pCurPointer->next == NULL)
		return false;
	else
		return true;
}

template <typename T>
T DoublyIterator<T> :: First()
{
	return m_List.m_pFirst->data;
}

template <typename T>
void DoublyIterator<T> :: Next()
{
	if (m_pCurPointer != NULL) m_pCurPointer = m_pCurPointer->next;
	//current pointer�� NULL�� �ƴ� ��쿡�� �̵��ϵ��� ��.
}

template <typename T>
void DoublyIterator<T>::Prev()
{
	if (m_pCurPointer != NULL) m_pCurPointer = m_pCurPointer->prev;
	//current pointer�� NULL�� �ƴ� ��쿡�� �̵��ϵ��� ��.
}

template <typename T>
T DoublyIterator<T>::Current()
{
	return m_pCurPointer->data;
}

template <typename T>
DoublyNodeType<T> DoublyIterator<T> :: GetCurrentNode()
{
	return *m_pCurPointer;
}

#endif