#pragma once
#ifndef _DOUBLYITERATOR_H
#define _DOUBLYITERATOR_H

#include "DoublyLinkedList.h"

template<typename T>
struct DoublyNodeType;
template<typename T>
class DoublyLinkedList;

/**
*	연결리스트에서 쓰이는 Iterator의 클래스.
*   Next 호출 시에 Next가 null pointer인 경우 data 반환에서 에러 발생. 이에 따라 data는 반환하지 않도록 수정함
*   current pointer의 data를 반환하는 Current 함수를 추가함.
*   Doubly Linked List에서 사용할 Iterator이므로, Prev 함수를 추가함.
*	2018.11.15 update : CurrentPtr 함수 추가
*	2018.11.16 update : Reset 함수 추가
*/

template <typename T>
class DoublyIterator
{
	friend class DoublyLinkedList<T>;
public:
	/**
	*	기본 생성자
	*/
	DoublyIterator(const DoublyLinkedList<T> &list) : m_List(list), m_pCurPointer(list.m_pFirst) {};


	/**
	*  현재 pointer가 NULL이 아니면 true, NULL이면 false를 반환함.
	*/
	bool NotNull();

	/**
	*  현재 pointer의 다음 pointer가 NULL이 아니면 true, NULL이면 false를 반환함.
	*/
	bool NextNotNull();

	/**
	*  첫번째 Node의 data를 반환함.
	*/
	T First();

	/**
	*  Next pointer가 NULL이 아님을 확증할 수 없으므로, pointer를 넘기기만 함. data 접근은 CurrentNode 혹은 Current에서만 가능하도록 수정.
	*/
	void Next();

	/**
	*  이전 포인터로 이동하는 함수
	*/
	void Prev();

	/**
	*  현재 point의 data를 반환함.
	*/
	T Current();

	/**
	*	현재 point의 data pointer를 반환함.
	*/
	T* CurrentPtr();

	/**
	*	현재 pointer의 node를 반환함. (pass by value)
	*/
	DoublyNodeType<T> GetCurrentNode();

	/**
	*	처음 노드로 포인터를 초기화하는 함수.
	*/
	void Reset();

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
	//current pointer가 NULL이 아닌 경우에만 이동하도록 함.
}

template <typename T>
void DoublyIterator<T>::Prev()
{
	if (m_pCurPointer != NULL) m_pCurPointer = m_pCurPointer->prev;
	//current pointer가 NULL이 아닌 경우에만 이동하도록 함.
}

template <typename T>
T DoublyIterator<T>::Current()
{
	return m_pCurPointer->data;
}

template <typename T>
T* DoublyIterator<T>::CurrentPtr()
{
	return &m_pCurPointer->data;
}

template <typename T>
DoublyNodeType<T> DoublyIterator<T> :: GetCurrentNode()
{
	return *m_pCurPointer;
}

template <typename T>
void DoublyIterator<T>::Reset()
{
	m_pCurPointer = list.m_pFirst;
}
#endif