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
* Doubly Linked List에 쓰일 node를 위한 structure
*/
template <typename T>
struct DoublyNodeType
{
	T data;	///< 저장하는 데이터
	DoublyNodeType* prev;	///< 이전 node를 가리키는 포인터
	DoublyNodeType* next;	///< 이후 node를 가리키는 포인터
};


/**
*	Doubly Linked Structre을 사용하는 Doubly Linked List
*   Add, Delete, Replace 함수는 이전 자료구조처럼 할당 연산자를 통해 데이터를 복사하거나 참조만 하기 때문에, parameter를 const reference로 받아오기로 함
*   MakeEmpty 함수 호출 이후 아직 주소 해제가 완료되지 않은 시점에서 다시 MakeEmpty 등이 호출될 수 있으므로 코드 내용을 약간 수정함.
*   Header/Trailer Node는 generic한 data structure에서는 사용하기 어렵다. 데이터 타입이 최소/최대값이 없을 수 있으며, 각 타입마다 생성하는 방법이 다르기 때문이다.
*/

template <typename T>
class DoublyLinkedList
{
	friend class DoublyIterator<T>;
public:
	/**
	*	기본 생성자
	*/
	DoublyLinkedList();

	/**
	*	소멸자
	*/
	~DoublyLinkedList();

	/**
	*   복사 생성자
	*/
	DoublyLinkedList(const DoublyLinkedList& list);

	/**
	*	@brief	리스트가 꽉 차 있는지 (데이터를 더 추가할 수 없는지)를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	메모리 할당에 문제가 있으면 exception을 throw하고 true 반환. 아닌 경우 false를 반환한다.
	*/
	bool IsFull();

	/**
	*	@brief	리스트가 비어있는지 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	리스트가 비어있으면 (first가 비어있으면) true 반환. 아닌 경우 false를 반환한다.
	*/
	bool IsEmpty() const;

	/**
	*	@brief	리스트를 비운다.
	*	@pre	없음.
	*	@post	리스트가 비게 된다.
	*/
	void MakeEmpty();

	/**
	*	@brief	리스트에 있는 노드의 개수를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	리스트의 노드 개수를 반환한다.
	*/
	int GetLength() const;

	/**
	*	@brief	리스트에 아이템을 추가한다.
	*	@pre	없음.
	*	@post	아이템이 리스트에 추가된다.
	*	@param	item	추가할 아이템
	*	@return	성공하면 1, 실패하면 0을 반환한다.
	*/
	int Add(const T& item);

	/**
	*	@brief	item의 key와 같은 데이터를 리스트에서 찾고, 성공하면 parameter에 할당한다.
	*	@pre	item의 key가 유효해야 한다.
	*	@post	데이터가 존재한다면, parameter에 할당된다.
	*	@param	item	검색하려는 key를 가진 아이템
	*	@return	아이템을 찾으면 1, 실패하면 0을 반환한다.
	*/
	int Get(T& item);

	/**
	*	@brief	item의 key와 같은 데이터를 찾아 삭제한다.
	*	@pre	item의 key가 유효해야 한다.
	*	@post	데이터가 존재한다면, 삭제한다.
	*	@param	item	삭제하려는 key를 가진 아이템
	*	@return 아이템을 찾아 삭제하면 1, 실패하면 0을 반환한다.
	*/
	int Delete(const T& item);

	/**
	*	@brief	item의 key와 같은 데이터를 찾고, parameter의 데이터로 교체한다.
	*	@pre	item의 key가 유효해야 한다.
	*	@post	데이터가 존재한다면, parameter의 데이터로 교체한다.
	*	@param	item	교체하려는 key를 가진 아이템
	*	@return	아이템을 찾아 교체하면 1, 실패하면 0을 반환한다.
	*/
	int Replace(const T& item);

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 할당한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	list	복사할 대상 데이터
	*	@return	할당한 현재 데이터를 반환한다.
	*/
	DoublyLinkedList<T>& operator=(const DoublyLinkedList<T>& list);

	/**
	*	@brief	데이터 비교 함수를 설정한다.
	*	@pre	비교 함수는 t1, t2를 비교할 때 t1 > t2이면 양수, t1 = t2이면 0, t1 < t2이면 음수를 반환해야 한다.
	*	@post	데이터 비교 함수가 parameter의 함수로 설정된다.
	*	@param	func	설정할 데이터 비교 함수
	*/
	void SetCompareFunction(function<int(const T&, const T&)>& func);

private:

	DoublyNodeType<T>* m_pFirst;	///< 첫번째 노드를 가리키는 포인터
	DoublyNodeType<T>* m_pLast; ///< 마지막 노드를 가리키는 포인터
	int m_nLength;	///< 노드의 개수 (길이)
	function<int(const T&, const T&)> compareFunc; ///< 데이터 비교 함수
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
	m_pLast = NULL; //복사해오기 전 초기화
	compareFunc = list.compareFunc;
	
	if (!list.IsEmpty()) return; //비어있다면 종료

	DoublyIterator<T> iter(list);
	iter.m_pCurPointer = list.m_pLast; //마지막 포인터로 설정 (접근 횟수를 낮추기 위함)

	while (iter.NotNull()) //iteration을 역방향으로 돌면서 아이템을 각각 추가
	{
		Add(iter.Current());
		iter.Prev();
	}
}


template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
	//리스트내의 모든 node 제거
	MakeEmpty();
}

template <typename T>
bool DoublyLinkedList<T>::IsFull() {
	try {
		DoublyNodeType<T>* temp = new DoublyNodeType<T>(); //node 동적할당을 시도한다.
		delete temp; //성공했다면 메모리가 충분하므로 지우고 false 반환
		return false;
	}
	catch (bad_alloc& e) { //동적 할당에 문제가 생기면 out of memory. bad_alloc 예외가 처리된다.
		cerr << "bad alloc exception: " << e.what() << endl;
		return true;
	}
}

template <typename T>
bool DoublyLinkedList<T>::IsEmpty() const {
	return m_pFirst == NULL; //첫 노드가 NULL이면 비어있다.
}

template <typename T>
void DoublyLinkedList<T>::MakeEmpty()
{
	DoublyNodeType<T>* tempPtr;
	DoublyIterator<T> iter(*this); //iterator 선언

	while (iter.NotNull()) //NULL 포인터가 나올 때 까지
	{
		tempPtr = iter.m_pCurPointer; //현재 포인터
		iter.Next(); //다음 포인터로 넘기고
		delete tempPtr; //그 포인터를 해제한다.
	}

	m_nLength = 0;
	m_pFirst = NULL;
	m_pLast = NULL;
	//첫 노드, 마지막 노드에 NULL pointer를 할당한다.
	//메모리 해제가 완전히 끝나지 않았을 때 MakeEmpty가 다시 호출되면 error가 발생하므로 해결하기 위해 추가함.

}

template <typename T>
int DoublyLinkedList<T>::GetLength() const
{
	return m_nLength;
}


template <typename T>
int DoublyLinkedList<T>::Add(const T& item)
{
	if (IsFull()) return 0; //꽉 찼으면 실패

	DoublyNodeType<T>* node = new DoublyNodeType<T>; //추가할 노드 생성
	DoublyIterator<T> iter(*this);

	node->data = item;
	node->prev = NULL;
	node->next = NULL;

	if (m_pFirst == NULL)
	{ //비어 있는 경우
		m_pFirst = node;
		m_pLast = m_pFirst;
	}
	else
	{ //아닌 경우

		DoublyNodeType<T>* current;	// iterator에서 접근할 노드
		while (iter.NotNull())
		{
			current = iter.m_pCurPointer;

			if (node->data < current->data)
			{  //추가하려는 노드가 현재보다 작다면
				node->next = current; //다음 노드를 current로 설정

				if (current == m_pFirst) m_pFirst = node; //맨 앞인 경우 first로 설정
				else current->prev->next = node; //아닌 경우 current의 이전 노드가 다음을 node로 가리키도록 설정

				node->prev = current->prev; //이전 노드를 current의 이전 노드로 설정
				current->prev = node; //current의 이전 노드를 node로 설정
				break;
			}
			else if (node->data > current->data)
			{	//추가하려는 노드가 현재보다 크다면

				if (current->next == NULL) //current의 다음이 NULL이 없다면 (데이터가 더 없다면) 맨 뒤.
				{
					current->next = node;
					node->prev = current;
					m_pLast = node; //마지막 노드를 node로 설정
					break;
				}
				else iter.Next();
			}
			else //같은 데이터를 발견하면
			{
				delete node; //메모리를 해제하고 0 반환
				return 0;
			}
		}
	}

	m_nLength++;
	return 1;
}

template <typename T>
int DoublyLinkedList<T>::Get(T& item)
{
	DoublyIterator<T> iter(*this);

	while (iter.NotNull()) //현재 포인터가 NULL이 아니면
	{
		if (iter.m_pCurPointer->data == item) //찾으면
		{
			item = iter.m_pCurPointer->data; //할당 후 반환
			return 1;
		}
		else if (iter.m_pCurPointer->data > item) //item보다 현재 아이템이 크면
			return 0; //없다는 뜻이므로 0 반환

		iter.Next(); //못 찾았다면 다음 포인터로 이동
	}

	return 0; //없으면 0 반환
}

template <typename T>
int DoublyLinkedList<T>::Delete(const T& item)
{
	DoublyIterator<T> iter(*this);

	while (iter.NotNull()) //현재 포인터가 NULL이 아니면
	{
		if (iter.m_pCurPointer->data == item) //찾으면
		{
			DoublyNodeType<T>* node = iter.m_pCurPointer;
			node->prev->next = node->next; //이전 노드의 다음을 다음 노드로 설정
			node->next->prev = node->prev; //다음 노드의 이전을 이전 노드로 설정
			delete node; //메모리 해제
			return 1;
		}
		else if (iter.m_pCurPointer->data > item) //item보다 현재 아이템이 크면
			return 0; //없다는 뜻이므로 0 반환

		iter.Next(); //못 찾았다면 다음 포인터로 이동
	}

	return 0; //못 찾았다면 0 반환
}

template <typename T>
int DoublyLinkedList<T>::Replace(const T& item)
{

	DoublyIterator<T> iter(*this);

	while (iter.NotNull()) //현재 포인터가 NULL이 아니면
	{
		if (iter.m_pCurPointer->data == item) //찾으면
		{
			iter.m_pCurPointer->data = item; //item을 할당하여 교체
			return 1;
		}
		else if (iter.m_pCurPointer->data > item) //item보다 현재 아이템이 크면
			return 0; //없다는 뜻이므로 0 반환

		iter.Next(); //못 찾았다면 다음 포인터로 이동
	}

	return 0; //못 찾았다면 0 반환
}

template <typename T>
DoublyLinkedList<T>& DoublyLinkedList<T>::operator=(const DoublyLinkedList<T>& list)
{
	MakeEmpty();
	if (list.IsEmpty()) return *this; //비어있다면 바로 반환

	DoublyIterator<T> iter(list);
	iter.m_pCurPointer = list.m_pLast; //마지막 포인터로 설정 (접근 횟수를 낮추기 위함)

	while (iter.NotNull()) //iteration을 역방향으로 돌면서 아이템을 각각 추가
	{
		Add(iter.Current());
		iter.Prev();
	}

	return *this;
}

template <typename T>
void DoublyLinkedList<T>::SetCompareFunction(function<int(const T&, const T&)>& func)
{
	compareFunc = func;
}

#endif