#pragma once
#ifndef _DOUBLY_LINKEDLIST_H
#define _DOUBLY_LINKEDLIST_H

#include <iostream>
#include <fstream>
#include <string>
#include <functional>

using namespace std;

#include "DoublyIterator.h"

/**
*	Structure for NodeType to use in Linked structure.
*/

template <typename T>
class DoublyIterator;

template <typename T>
struct DoublyNodeType
{
	T data;	///< A data for each node.
	DoublyNodeType* prev;	///< A node pointer of previous node.
	DoublyNodeType* next;	///< A node pointer of next node.
};


/**
*	Single sorted list class for managing items.
*   sample code���� ������ ��
*   1. Add, Delete, Replace �Լ��� ���� �ڷᱸ��ó�� �Ҵ� �����ڸ� ���� �����͸� �����ϰų� ������ �ϱ� ������, parameter�� const reference�� �޾ƿ���� ��
*   2. MakeEmpty �Լ� ȣ�� ���� ���� �ּ� ������ �Ϸ���� ���� �������� �ٽ� MakeEmpty ���� ȣ��� �� �����Ƿ� �ڵ� ������ �ణ ������.
*/

template <typename T>
class DoublyLinkedList
{
	friend class DoublyIterator<T>;
public:
	/**
	*	default constructor.
	*/
	DoublyLinkedList();

	/**
	*	destructor.
	*/
	~DoublyLinkedList();

	/**
	*   copy constructor
	*/
	DoublyLinkedList(const DoublyLinkedList& list);

	/**
	*	@brief	Determines whether the list is full.
	*	@pre	List has been initialized.
	*	@post	None.
	*	@return	If there is not memory left, throws bad_alloc exception and return true, otherwise false.
	*/
	bool IsFull();

	/**
	*	@brief	Determines whether the list is empty.
	*	@pre	List has been initialized.
	*	@post	None.
	*	@return	Return true if the list is empty, otherwise false.
	*/
	bool IsEmpty() const;

	/**
	*	@brief	Initialize list to empty state.
	*	@pre	None
	*	@post	List is empty.
	*/
	void MakeEmpty();

	/**
	*	@brief	Get number of elements in the list.
	*	@pre	None.
	*	@post	Function value = number of elements in list.
	*	@return	Number of elements in this list.
	*/
	int GetLength() const;

	/**
	*	@brief	Add item into this list.
	*	@pre	List is not full. item is not in list.
	*	@post	Item is inserted in this list.
	*	@return	1 if this function works well, otherwise 0.
	*/
	int Add(const T& item);

	/**
	*	@brief	Retrieve list element whose key matches item's key (if present).
	*	@pre	Key member of item is initialized.
	*	@post	If there is an element whose key matches with item's key then the element's record is copied to the item.
	*	@return	1 if any element's primary key matches with item's, otherwise 0.
	*/
	int Get(T& item);

	/**
	*	@brief	Primary key�� �������� �˻��Ͽ� �ش��ϴ� ������ ������.
	*	@pre	����Ʈ�� ����/�ʱ�ȭ �Ϸ�Ǿ��ִ� ���¿��� ��.
	*	@post	��ġ�ϴ� �����Ͱ� ����Ʈ�� ���� ��� ������.
	*	@param	data	primary key�� �ݵ�� ���ԵǾ� �ִ� item ��ü.
	*	@return ������ ��ġ�ϴ� �����Ͱ� ������ ���� �� 1, ������ 0�� ����.
	*/
	int Delete(const T& data);

	/**
	*	@brief	Primary key�� �������� �˻��Ͽ� �ش��ϴ� ������ ���� ä������.
	*	@pre	����Ʈ�� ����/�ʱ�ȭ �Ϸ�Ǿ��ִ� ���¿��� ��.
	*	@post	��ġ�ϴ� �����Ͱ� ����Ʈ�� ���� ��� ���ο� �����ͷ� ��ü��.
	*	@param	data	primary key�� �ݵ�� ���ԵǾ� �ִ� item ��ü.
	*/
	int Replace(const T& data);

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �Ҵ��Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	list	������ ��� ������
	*	@return	�Ҵ��� ���� �����͸� ��ȯ�Ѵ�.
	*/
	DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& list);

	/**
	*	@brief	�� �Լ��� ��ü�Ѵ�.
	*	@pre	�� �Լ������� t1 < t2�� �� ����, t1 = t2�� �� 0, t1 > t2�� �� ����� ��ȯ�ؾ� �Ѵ�.
	*	@post	�� �Լ��� parameter�� �Լ��� ��ü�ȴ�.
	*	@param	func	��ü�� �� �Լ�
	*/
	void SetCompareFunction(function<int(const T&, const T&)>& func);


private:
	///< Header node�� Trailer node�� generic�� data structure���� key�� �ּ�/�ִ��� �˱� �����Ƿ�, ������� ����.
	DoublyNodeType<T>* m_pFirst;	///< Pointer for pointing a first node.
	DoublyNodeType<T>* m_pLast; ///< Last node.
	int m_nLength;	///< Number of node in the list.
	function<int(const T&, const T&)> compareFunc;	///< �� �Լ�
};


// Class constructor
template <typename T>
DoublyLinkedList<T>::DoublyLinkedList()
{
	m_nLength = 0;
	m_pFirst = nullptr;
	m_pLast = nullptr;
	compareFunc = [](const T& t1, const T& t2) { return (t1 > t2) - (t1 < t2); }
}

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList(const DoublyLinkedList<T>& list)
{
	m_nLength = 0;
	m_pFirst = nullptr;
	m_pLast = nullptr;

	DoublyIterator<T> iter(list);
	
	if (!list.IsEmpty()) return; //����ִٸ� ����

	while (iter.NotNull())
	{
		Add(iter.Current());
		iter.Next();
	}
}

// Class destructor
template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	//����Ʈ���� ��� node ����
	MakeEmpty();
}

// Determines whether the list is full.
// ���������� ������, ���� �޸𸮰� ���� ���� �� �̻� �Ҵ��� �Ұ����� ��� �ý��� ���� ó��
template <typename T>
bool DoublyLinkedList<T>::IsFull() {
	try {
		DoublyNodeType<T>* temp = nullptr;	// if there is not memory left, temp would not be initialized as NULL and
		delete temp;					// deleting temp(pointer) could not be executed.
		return false;
	}
	catch (bad_alloc& e_ba) {
		cerr << "bad_alloc exception caught : " << e_ba.what() << endl;
		return true;
	}
}

// Determines whether the list is empty.
template <typename T>
bool DoublyLinkedList<T>::IsEmpty() const {
	if (m_pFirst == nullptr) {
		return true;
	}
	else return false;
}

// Initialize list to empty state.
template <typename T>
void DoublyLinkedList<T>::MakeEmpty()
{
	// ����Ʈ���� ��� ��� ���� �ϰ� ����Ʈ�� length�� �ʱ�ȭ
	DoublyNodeType<T>* tempPtr;
	DoublyIterator<T> iter(*this);

	while (iter.NotNull())
	{
		tempPtr = iter.m_pCurPointer;
		iter.Next();
		delete tempPtr;
	}

	m_nLength = 0;
	m_pFirst = nullptr;
	m_pLast = nullptr;

}


// Get number of elements in the list.
template <typename T>
int DoublyLinkedList<T>::GetLength() const
{
	return m_nLength;
}


// Add item into this list.
template <typename T>
int DoublyLinkedList<T>::Add(const T& item)
{
	if (!IsFull()) {
		DoublyNodeType<T>* node = new DoublyNodeType<T>;
		DoublyIterator<T> iter(*this);

		node->data = item;
		node->prev = nullptr;
		node->next = nullptr;

		// ����ִ� ���
		if (m_pFirst == nullptr) {
			m_pFirst = node;
			m_pLast = m_pFirst;
		}
		// 1�� �̻�
		else {
			DoublyNodeType<T>* pNode;	// iterator���� ������ ������ ��ü
			while (iter.NotNull()) {
				// GetCurrentNode�� return by value�̹Ƿ� ���� �� �� �޸� ���� �Ұ��ؼ� ��� ����
				pNode = iter.m_pCurPointer;

				if (node->data < pNode->data) { // �������� �ϴ°� �� ����
					node->next = pNode;
					if (m_nLength == 1) {	// �� ��
						m_pFirst = node;
					}
					else {	// �߰�
						node->prev = pNode->prev;
						node->prev->next = node;
					}
					pNode->prev = node;
					break;
				}
				else {
					if (!iter.NextNotNull()) {	// �� ��
						pNode->next = node;
						node->prev = pNode;
						m_pLast = node;
						break;
					}
					else iter.Next();
				}
			}
		}

		m_nLength++;
		return 1;
	}
	else return 0;
}


// Retrieve list element whose key matches item's key (if present).
template <typename T>
int DoublyLinkedList<T>::Get(T& item)
{
	DoublyIterator<T> iter(*this);
	int count = 0;	// �� ��°�� ��ġ�ϰ� �ִ��� ���� (������ 0)
				// iterator�� ����ϸ鼭 curPointer�� ������ �� �����Ƿ� return value�� �ǹ̸� ����
	bool found = false;
	while (iter.NotNull()) {
		count++;
		T current = iter.Current();
		if (item == current) {
			found = true;
			item = current;
			break;
		}
		else if (item < current) {
			break;
		}
		else {
			iter.Next();
		}
	}

	if (found) {
		return count;
	}
	else return 0;

}



// Ư�� ��Ҹ� ã�� �����ϴ� �Լ�
template <typename T>
int DoublyLinkedList<T>::Delete(const T& data) {
	DoublyNodeType<T>* pNode = m_pFirst;
	T temp = data; //�����͸� ����� ���� ã�� ��ü

	int positionIndex = Get(temp);
	if (positionIndex) {
		for (int i = 1; i < positionIndex; i++) {
			pNode = pNode->next;
		}
		if (pNode->next != nullptr) {	// ���� �ƴϸ�
			pNode->next->prev = pNode->prev;
		}
		else m_pLast = pNode->prev;
		if (pNode->prev != nullptr) {	// ó���� �ƴϸ�
			pNode->prev->next = pNode->next;
		}
		else m_pFirst = pNode->next;
		delete pNode;
		m_nLength--;
		return 1;
	}
	else return 0;

}

// Ư�� ��Ҹ� ã�� ���ο� �������� ��ġ��Ű�� �Լ�
template <typename T>
int DoublyLinkedList<T>::Replace(const T& data) {

	T target = data;
	DoublyNodeType<T>* pNode = m_pFirst;
	int positionIndex = Get(target);
	if (!positionIndex) return 0;
	for (int i = 1; i < positionIndex; i++) {
		pNode = pNode->next;
	}
	pNode->data = data;
	return 1;
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& list)
{
	MakeEmpty();
	DoublyIterator<T> iter(list);

	while (iter.NotNull())
	{
		Add(iter.Current());
		iter.Next();
	}

	return *this;
}

template <typename T>
void DoublyLinkedList<T>::SetCompareFunction(function<int(const T&, const T&)>& func)
{
	this->compareFunc = func;
}
#endif