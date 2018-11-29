#pragma once
#ifndef _DOUBLY_LINKEDLIST_H
#define _DOUBLY_LINKEDLIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

#include "DoublyIterator.h"

template <typename T>
class DoublyIterator;

/**
* Doubly Linked List�� ���� node�� ���� structure
*/
template <typename T>
struct DoublyNodeType
{
	T data;	///< �����ϴ� ������
	DoublyNodeType* prev;	///< ���� node�� ����Ű�� ������
	DoublyNodeType* next;	///< ���� node�� ����Ű�� ������
};


/**
*	Doubly Linked Structre�� ����ϴ� Doubly Linked List
*   Add, Delete, Replace �Լ��� ���� �ڷᱸ��ó�� �Ҵ� �����ڸ� ���� �����͸� �����ϰų� ������ �ϱ� ������, parameter�� const reference�� �޾ƿ���� ��
*   MakeEmpty �Լ� ȣ�� ���� ���� �ּ� ������ �Ϸ���� ���� �������� �ٽ� MakeEmpty ���� ȣ��� �� �����Ƿ� �ڵ� ������ �ణ ������.
*   Header/Trailer Node�� generic�� data structure������ ����ϱ� ��ƴ�. ������ Ÿ���� �ּ�/�ִ밪�� ���� �� ������, �� Ÿ�Ը��� �����ϴ� ����� �ٸ��� �����̴�.
*	2018.11.16 update : iterator ��ü�� reference�� �޾� delete�ϴ� �Լ� �߰�.
*/

template <typename T>
class DoublyLinkedList
{
	friend class DoublyIterator<T>;
public:
	/**
	*	�⺻ ������
	*/
	DoublyLinkedList();

	/**
	*	�Ҹ���
	*/
	~DoublyLinkedList();

	/**
	*   ���� ������
	*/
	DoublyLinkedList(const DoublyLinkedList& list);

	/**
	*	@brief	����Ʈ�� �� �� �ִ��� (�����͸� �� �߰��� �� ������)�� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	�޸� �Ҵ翡 ������ ������ exception�� throw�ϰ� true ��ȯ. �ƴ� ��� false�� ��ȯ�Ѵ�.
	*/
	bool IsFull();

	/**
	*	@brief	����Ʈ�� ����ִ��� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	����Ʈ�� ��������� (first�� ���������) true ��ȯ. �ƴ� ��� false�� ��ȯ�Ѵ�.
	*/
	bool IsEmpty() const;

	/**
	*	@brief	����Ʈ�� ����.
	*	@pre	����.
	*	@post	����Ʈ�� ��� �ȴ�.
	*/
	void MakeEmpty();

	/**
	*	@brief	����Ʈ�� �ִ� ����� ������ ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	����Ʈ�� ��� ������ ��ȯ�Ѵ�.
	*/
	int GetLength() const;

	/**
	*	@brief	����Ʈ�� �������� �߰��Ѵ�.
	*	@pre	����.
	*	@post	�������� ����Ʈ�� �߰��ȴ�.
	*	@param	item	�߰��� ������
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Add(const T& item);

	/**
	*	@brief	item�� key�� ���� �����͸� ����Ʈ���� ã��, �����ϸ� parameter�� �Ҵ��Ѵ�.
	*	@pre	item�� key�� ��ȿ�ؾ� �Ѵ�.
	*	@post	�����Ͱ� �����Ѵٸ�, parameter�� �Ҵ�ȴ�.
	*	@param	item	�˻��Ϸ��� key�� ���� ������
	*	@return	�������� ã���� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Get(T& item);

	/**
	*	@brief	item�� key�� ���� �����͸� ã�� �����Ѵ�.
	*	@pre	item�� key�� ��ȿ�ؾ� �Ѵ�.
	*	@post	�����Ͱ� �����Ѵٸ�, �����Ѵ�.
	*	@param	item	�����Ϸ��� key�� ���� ������
	*	@return �������� ã�� �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Delete(const T& item);

	/**
	*	@brief	iterator�� ����Ű�� ��带 ����Ʈ���� �����Ѵ�.
	*	@pre	iterator�� ��ȿ�� ��ü�̾�� �Ѵ�.
	*	@post	iterator�� ��� �����Ͱ� ����Ʈ�� ���ԵǴ� ��ȿ�� �����Ͷ��, �����ϰ� iterator�� �������� �ѱ��.
	*	@param	iter	�����Ϸ��� ��带 ����Ű�� iterator ��ü
	*	@return	��带 �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Delete(DoublyIterator<T>& iter);

	/**
	*	@brief	item�� key�� ���� �����͸� ã��, parameter�� �����ͷ� ��ü�Ѵ�.
	*	@pre	item�� key�� ��ȿ�ؾ� �Ѵ�.
	*	@post	�����Ͱ� �����Ѵٸ�, parameter�� �����ͷ� ��ü�Ѵ�.
	*	@param	item	��ü�Ϸ��� key�� ���� ������
	*	@return	�������� ã�� ��ü�ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Replace(const T& item);

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �Ҵ��Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	list	������ ��� ������
	*	@return	�Ҵ��� ���� �����͸� ��ȯ�Ѵ�.
	*/
	DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& list);

	/**
	*	@brief	������ �� �Լ��� �����Ѵ�.
	*	@pre	�� �Լ��� t1, t2�� ���� �� t1 > t2�̸� ���, t1 = t2�̸� 0, t1 < t2�̸� ������ ��ȯ�ؾ� �Ѵ�.
	*	@post	������ �� �Լ��� parameter�� �Լ��� �����ȴ�.
	*	@param	func	������ ������ �� �Լ�
	*/
	void SetCompareFunction(const function<int(const T&, const T&)>& func);

private:

	DoublyNodeType<T>* m_pFirst;	///< ù��° ��带 ����Ű�� ������
	DoublyNodeType<T>* m_pLast; ///< ������ ��带 ����Ű�� ������
	int m_nLength;	///< ����� ���� (����)
	function<int(const T&, const T&)> compareFunc; ///< ������ �� �Լ�
};


template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
{
	m_nLength = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
	compareFunc = [](const T& t1, const T& t2)
	{
		return (t1 > t2) - (t1 < t2);
	};
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& list)
{
	m_nLength = 0;
	m_pFirst = NULL;
	m_pLast = NULL; //�����ؿ��� �� �ʱ�ȭ
	compareFunc = list.compareFunc;

	if (!list.IsEmpty()) return; //����ִٸ� ����

	DoublyIterator<T> iter(list);
	iter.m_pCurPointer = list.m_pLast; //������ �����ͷ� ���� (���� Ƚ���� ���߱� ����)

	while (iter.NotNull()) //iteration�� ���������� ���鼭 �������� ���� �߰�
	{
		Add(iter.Current());
		iter.Prev();
	}
}


template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	//����Ʈ���� ��� node ����
	MakeEmpty();
}

template <typename T>
bool DoublyLinkedList<T>::IsFull() {
	try {
		DoublyNodeType<T>* temp = new DoublyNodeType<T>(); //node �����Ҵ��� �õ��Ѵ�.
		delete temp; //�����ߴٸ� �޸𸮰� ����ϹǷ� ����� false ��ȯ
		return false;
	}
	catch (bad_alloc& e) { //���� �Ҵ翡 ������ ����� out of memory. bad_alloc ���ܰ� ó���ȴ�.
		cerr << "bad alloc exception: " << e.what() << endl;
		return true;
	}
}

template <typename T>
bool DoublyLinkedList<T>::IsEmpty() const {
	return m_pFirst == NULL; //ù ��尡 NULL�̸� ����ִ�.
}

template <typename T>
void DoublyLinkedList<T>::MakeEmpty()
{
	DoublyNodeType<T>* tempPtr;
	DoublyIterator<T> iter(*this); //iterator ����

	while (iter.NotNull()) //NULL �����Ͱ� ���� �� ����
	{
		tempPtr = iter.m_pCurPointer; //���� ������
		iter.Next(); //���� �����ͷ� �ѱ��
		delete tempPtr; //�� �����͸� �����Ѵ�.
	}

	m_nLength = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
	//ù ���, ������ ��忡 NULL pointer�� �Ҵ��Ѵ�.
	//�޸� ������ ������ ������ �ʾ��� �� MakeEmpty�� �ٽ� ȣ��Ǹ� error�� �߻��ϹǷ� �ذ��ϱ� ���� �߰���.

}

template <typename T>
int DoublyLinkedList<T>::GetLength() const
{
	return m_nLength;
}


template <typename T>
int DoublyLinkedList<T>::Add(const T& item)
{
	if (IsFull()) return 0; //�� á���� ����

	DoublyNodeType<T>* node = new DoublyNodeType<T>; //�߰��� ��� ����
	DoublyIterator<T> iter(*this);
	DoublyIterator<T> iter2(*this);
	iter2.ResetToLastPointer();

	node->data = item;
	node->prev = NULL;
	node->next = NULL;

	if (m_pFirst == NULL)
	{ //��� �ִ� ���
		m_pFirst = node;
		m_pLast = m_pFirst;
	}
	else
	{ //�ƴ� ���

		DoublyNodeType<T>* current;	// iterator���� ������ ���
		do
		{
			//�� Ž��
			current = iter.m_pCurPointer;

			int compare = compareFunc(node->data, current->data);
			if (compare < 0)
			{  //�߰��Ϸ��� ��尡 ���纸�� �۴ٸ� (node < current)
				node->next = current; //���� ��带 current�� ����

				if (current == m_pFirst) m_pFirst = node; //�� ���� ��� first�� ����
				else current->prev->next = node; //�ƴ� ��� current�� ���� ��尡 ������ node�� ����Ű���� ����

				node->prev = current->prev; //���� ��带 current�� ���� ���� ����
				current->prev = node; //current�� ���� ��带 node�� ����
				break;
			}
			else if (compare > 0) iter.Next();
			//�߰��Ϸ��� ��尡 ���纸�� ũ�ٸ� (node > current) �������� �ѱ��. (iter�� iter2�� �߰����� ������ ����ǹǷ� null üũ�� ���� ����.)
			else //���� �����͸� �߰��ϸ�
			{
				delete node; //�޸𸮸� �����ϰ� 0 ��ȯ
				return 0;
			}

			//�� Ž��
			current = iter2.m_pCurPointer;
			compare = compareFunc(node->data, current->data);
			if (compare > 0)
			{  //�߰��Ϸ��� ��尡 ���纸�� ũ�ٸ� (node > current)
				node->prev = current; //���� ��带 current�� ����

				if (current == m_pLast) m_pLast = node; //�� �������� ��� last�� ����
				else current->next->prev = node; //�ƴ� ��� current�� ���� ��尡 ������ node�� ����Ű���� ����

				node->next = current->next; //���� ��带 current�� ���� ���� ����
				current->next = node; //current�� ���� ��带 node�� ����
				break;
			}
			else if (compare < 0) iter2.Prev();
			//�߰��Ϸ��� ��尡 ���纸�� �۴ٸ� (node < current) �������� �ѱ��. (iter�� iter2�� �߰����� ������ ����ǹǷ� null üũ�� ���� ����.)
			else //���� �����͸� �߰��ϸ�
			{
				delete node; //�޸𸮸� �����ϰ� 0 ��ȯ
				return 0;
			}

		} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));
	}

	m_nLength++;
	return 1;
}

template <typename T>
int DoublyLinkedList<T>::Get(T& item)
{
	if (IsEmpty()) return 0;

	DoublyIterator<T> iter(*this);
	DoublyIterator<T> iter2(*this);

	iter2.ResetToLastPointer();

	do
	{
		int compare = compareFunc(iter.m_pCurPointer->data, item);
		int compare2 = compareFunc(iter2.m_pCurPointer->data, item);

		if (compare == 0) item = iter.m_pCurPointer->data;
		else if (compare2 == 0) item = iter2.m_pCurPointer->data;
		else //ã�� ���ϸ�
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		return 1;
	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //������ 0 ��ȯ
}

template <typename T>
int DoublyLinkedList<T>::Delete(const T& item)
{
	if (IsEmpty()) return 0;

	DoublyIterator<T> iter(*this);
	DoublyIterator<T> iter2(*this);
	DoublyNodeType<T>* node = nullptr;

	iter2.ResetToLastPointer();

	do
	{
		int compare = compareFunc(iter.m_pCurPointer->data, item);
		int compare2 = compareFunc(iter2.m_pCurPointer->data, item);

		if (compare == 0) node = iter.m_pCurPointer;
		else if (compare2 == 0) node = iter2.m_pCurPointer;
		else //ã�� ���ϸ�
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		if (node->prev != nullptr) node->prev->next = node->next; //���� ����� ������ ���� ���� ����
		else m_pFirst = node->next; //���� ��尡 nullptr�̸� ó�� ���
		if (node->next != nullptr) node->next->prev = node->prev; //���� ����� ������ ���� ���� ����
		else m_pLast = node->prev; //���� ��尡 nullptr�̸� ������ ���

		delete node; //�޸� ����
		return 1;

	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //�� ã�Ҵٸ� 0 ��ȯ
}

template <typename T>
int DoublyLinkedList<T>::Replace(const T& item)
{
	if (IsEmpty()) return 0;

	DoublyIterator<T> iter(*this);
	DoublyIterator<T> iter2(*this);

	iter2.ResetToLastPointer();

	do
	{
		int compare = compareFunc(iter.m_pCurPointer->data, item);
		int compare2 = compareFunc(iter2.m_pCurPointer->data, item);

		if (compare == 0) iter.m_pCurPointer->data = item;
		else if (compare2 == 0) iter2.m_pCurPointer->data = item;
		else //ã�� ���ϸ�
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		return 1;
	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //������ 0 ��ȯ
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& list)
{
	MakeEmpty();
	if (list.IsEmpty()) return *this; //����ִٸ� �ٷ� ��ȯ

	DoublyIterator<T> iter(list);
	iter.m_pCurPointer = list.m_pLast; //������ �����ͷ� ���� (���� Ƚ���� ���߱� ����)

	while (iter.NotNull()) //iteration�� ���������� ���鼭 �������� ���� �߰�
	{
		Add(iter.Current());
		iter.Prev();
	}

	return *this;
}

template <typename T>
void DoublyLinkedList<T>::SetCompareFunction(const function<int(const T&, const T&)>& func)
{
	compareFunc = func;
}

template <typename T>
int DoublyLinkedList<T>::Delete(DoublyIterator<T>& iter)
{
	if (&iter.m_List != this) return 0; //����Ʈ�� �����Ͱ� ���� ������ 0 ��ȯ

	DoublyNodeType<T>* node = iter.m_pCurPointer;
	if (node == NULL) return 0; //���� �����Ͱ� NULL�̸� 0 ��ȯ

	if (node->prev != nullptr) node->prev->next = node->next;
	else m_pFirst = node->next;
	if (node->next != nullptr) node->next->prev = node->prev;
	else m_pLast = node->prev;
	iter.Next();

	delete node;
	return 1;
}
#endif