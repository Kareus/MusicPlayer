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
*	2018.11.16 update : iterator 객체를 reference로 받아 delete하는 함수 추가.
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
	*	@brief	iterator가 가리키는 노드를 리스트에서 제거한다.
	*	@pre	iterator는 유효한 객체이어야 한다.
	*	@post	iterator의 노드 데이터가 리스트에 포함되는 유효한 데이터라면, 삭제하고 iterator를 다음으로 넘긴다.
	*	@param	iter	삭제하려는 노드를 가리키는 iterator 객체
	*	@return	노드를 삭제하면 1, 실패하면 0을 반환한다.
	*/
	int Delete(DoublyIterator<T>& iter);

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
	void SetCompareFunction(const function<int(const T&, const T&)>& func);

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
	DoublyIterator<T> iter2(*this);
	iter2.ResetToLastPointer();

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
		do
		{
			//앞 탐색
			current = iter.m_pCurPointer;

			int compare = compareFunc(node->data, current->data);
			if (compare < 0)
			{  //추가하려는 노드가 현재보다 작다면 (node < current)
				node->next = current; //다음 노드를 current로 설정

				if (current == m_pFirst) m_pFirst = node; //맨 앞인 경우 first로 설정
				else current->prev->next = node; //아닌 경우 current의 이전 노드가 다음을 node로 가리키도록 설정

				node->prev = current->prev; //이전 노드를 current의 이전 노드로 설정
				current->prev = node; //current의 이전 노드를 node로 설정
				break;
			}
			else if (compare > 0) iter.Next();
			//추가하려는 노드가 현재보다 크다면 (node > current) 다음으로 넘긴다. (iter과 iter2가 중간에서 만남이 보장되므로 null 체크는 하지 않음.)
			else //같은 데이터를 발견하면
			{
				delete node; //메모리를 해제하고 0 반환
				return 0;
			}

			//뒤 탐색
			current = iter2.m_pCurPointer;
			compare = compareFunc(node->data, current->data);
			if (compare > 0)
			{  //추가하려는 노드가 현재보다 크다면 (node > current)
				node->prev = current; //이전 노드를 current로 설정

				if (current == m_pLast) m_pLast = node; //맨 마지막인 경우 last로 설정
				else current->next->prev = node; //아닌 경우 current의 다음 노드가 이전을 node로 가리키도록 설정

				node->next = current->next; //다음 노드를 current의 다음 노드로 설정
				current->next = node; //current의 다음 노드를 node로 설정
				break;
			}
			else if (compare < 0) iter2.Prev();
			//추가하려는 노드가 현재보다 작다면 (node < current) 이전으로 넘긴다. (iter과 iter2가 중간에서 만남이 보장되므로 null 체크는 하지 않음.)
			else //같은 데이터를 발견하면
			{
				delete node; //메모리를 해제하고 0 반환
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
		else //찾지 못하면
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		return 1;
	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //없으면 0 반환
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
		else //찾지 못하면
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		if (node->prev != nullptr) node->prev->next = node->next; //이전 노드의 다음을 다음 노드로 설정
		else m_pFirst = node->next; //이전 노드가 nullptr이면 처음 노드
		if (node->next != nullptr) node->next->prev = node->prev; //다음 노드의 이전을 이전 노드로 설정
		else m_pLast = node->prev; //다음 노드가 nullptr이면 마지막 노드

		delete node; //메모리 해제
		return 1;

	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //못 찾았다면 0 반환
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
		else //찾지 못하면
		{
			if (compare < 0) iter.Next();
			if (compare2 > 0) iter2.Prev();
			continue;
		}

		return 1;
	} while (!(iter.m_pCurPointer->data > iter2.m_pCurPointer->data));

	return 0; //없으면 0 반환
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
void DoublyLinkedList<T>::SetCompareFunction(const function<int(const T&, const T&)>& func)
{
	compareFunc = func;
}

template <typename T>
int DoublyLinkedList<T>::Delete(DoublyIterator<T>& iter)
{
	if (&iter.m_List != this) return 0; //리스트의 포인터가 같지 않으면 0 반환

	DoublyNodeType<T>* node = iter.m_pCurPointer;
	if (node == NULL) return 0; //현재 포인터가 NULL이면 0 반환

	if (node->prev != nullptr) node->prev->next = node->next;
	else m_pFirst = node->next;
	if (node->next != nullptr) node->next->prev = node->prev;
	else m_pLast = node->prev;
	iter.Next();

	delete node;
	return 1;
}
#endif