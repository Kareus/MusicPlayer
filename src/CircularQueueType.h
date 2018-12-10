#ifndef __CIRCULARQUEUETYPE__
#define __CIRCULARQUEUETYPE__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define maxQueue 10


/**
*	@brief	Exception class thrown by Enqueue when queue is full.
*/
class FullQueue
{
public:
	/**
	*	@brief	Display a message for full queue on screen.
	*/
	void print()
	{
		cout << "FullQueue exception thrown." << endl;
	}
};


/**
*	@brief	Exception class thrown by Dequeue when queue is empty.
*/
class EmptyQueue
{
public:
	/**
	*	@brief	Display a message for empty queue on screen.
	*/
	void print()
	{
		cout << "EmtpyQueue exception thrown." << endl;
	}
};

/**
*	@brief	Circular Queue class.
*	@details	This class processes as First In, First Out (FIFO), 템플릿을 적용해 다양한 변수 타입을 저장할 수 있다.
*/
template <typename T>
class CircularQueueType
{
public:
	/**
	*	@brief	Allocate dynamic array whose size is maxQueue.
	*	@pre	The maxQueue is defined
	*	@post	Member of items points the allocated array.
	*/
	CircularQueueType();

	/**
	*	@brief	Allocate dynamic array whose size is max.
	*	@pre	none.
	*	@post	Member of items points the allocated array.
	*/
	CircularQueueType(int max);

	/**
	*	@brief	Destruct the object. Free the array dynamically allocated.
	*	@pre	Release memory for queue pointer.
	*	@post	none.
	*/
	~CircularQueueType();


	/**
	*	@brief	Determines whether the queue is full.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (queue is full)
	*/
	bool IsFull();

	/**
	*	@brief	Determines whether the queue is empty.
	*	@pre	Queue has been initialized.
	*	@post	Function value = (queue is empty)
	*/
	bool IsEmpty();

	/**
	*	@brief	Makes the queue empty.
	*	@pre	Queue has been initialized.
	*	@post	m_iFront와 m_iRear is set same value as Constructer.
	*/
	void MakeEmpty();

	/**
	*	@brief	Adds newItem to the last of the queue.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is full), FullQueue exception is thrown; otherwise, newItem is at the last.
	*/
	void EnQueue(const T& item);

	/**
	*	@brief	Removes first item from the queue.
	*	@pre	Queue has been initialized.
	*	@post	If (queue is empty), EmptyQueue exception is thrown; otherwise, first element has been removed from queue. item gets value of removed item.
	*/
	void DeQueue(T &item);

	/**
	*	@brief	큐의 포인터를 리셋한다.
	*	@pre	없음.
	*	@post	큐의 포인터가 리셋된다.
	*/
	void ResetQueue();

	/**
	*	@brief	다음 아이템을 item에 할당한다
	*	@pre	없음.
	*	@post	큐가 비어있다면 EmptyQueue가 호출되고, 아니라면 다음 아이템이 item에 할당된다. 이 때, 성공하면 인덱스를 반환하고 큐의 끝이라면 -1을 반환한다.
	*/
	int GetNextItem(T &item);

	/**
	*	@brief	find 아이템을 찾아 data로 교체한다.
	*	@pre	없음.
	*	@post	큐가 비어있다면 EmptyQueue가 호출되고, 아니라면 find를 찾아 data로 교체한다. 성공하면 1, 실패하면 0을 반환한다.
	*/
	int Replace(const T &find, const T &data);

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	CircularQueueType<T>& operator=(const CircularQueueType<T>& data);

	/**
	*	@brief	큐의 데이터의 개수를 반환한다.
	*	@return	큐의 데이터 개수
	*/
	int GetLength() const;

private:
	int m_iFront;	//index of one infront of the first element.
	int m_iRear;	//index of the last element.
	int m_nMaxQueue;	//max size of the queue.
	int m_nLength; //현재 큐에 있는 데이터 개수
	int m_curPointer;	//현재 포인터
	T* m_pItems;	//pointer for dynamic allocation.
};

//Allocate dynamic array whose size is maxQueue.
template <typename T>
CircularQueueType<T>::CircularQueueType()
{
	m_pItems = new T[maxQueue];	//m_pItems에 크기가 maxQueue인 배열을 만든다.
	m_nMaxQueue = maxQueue;
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//멤버 변수들을 초기화시켜준다.
	m_nLength = 0;
}

//Allocate dynamic array whose size is max.
template <typename T>
CircularQueueType<T>::CircularQueueType(int max)
{
	m_pItems = new T[max];	//m_pItems에 크기가 max인 배열을 만든다.
	m_nMaxQueue = max;
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//멤버 변수들을 초기화시켜준다.
	m_nLength = 0;
}

//Destruct the object. Free the array dynamically allocated.
template <typename T>
CircularQueueType<T>::~CircularQueueType()
{
	delete[] m_pItems;	//동적할당했던 메모리를 해제해준다.
}

//Determines whether the queue is full.
template <typename T>
bool CircularQueueType<T>::IsFull()
{
	if (m_iFront - m_iRear == 1 || m_iRear - m_iFront == m_nMaxQueue - 1)
		return 1;	//m_iFront가 m_iRear보다 1 앞일때는 꽉 찬것이므로 1을 리턴한다. m_iFront가 0일때는 m_iRear가 m_nMaxQueue-1과 같을때 꽉 찬것이다.
	return 0;	//꽉 차지 않았으면 0을 리턴한다.
}

//Determines whether the queue is empty.
template <typename T>
bool CircularQueueType<T>::IsEmpty()
{
	if (m_iFront == m_iRear)
		return 1;	//m_iFront와 m_iRear가 같은 경우는 queue가 비어있는 상태이므로 1을 리턴한다.
	return 0;	//비어있지 않으면 0을 리턴한다.
}

//Makes the queue empty.
template <typename T>
void CircularQueueType<T>::MakeEmpty()
{
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//생성자와 같은 값으로 m_iFront와 m_iRear를 변경해준다.
}

//Adds newItem to the last of the queue.
template <typename T>
void CircularQueueType<T>::EnQueue(const T& item)
{
	if (IsFull()) throw FullQueue();	//꽉 찼을땐 throw FullQueue
	m_iRear = (m_iRear + 1) % m_nMaxQueue;
	m_pItems[m_iRear] = item;	//m_iRear를 1증가시켜주고 item값을 넣어준다.
	m_nLength++;
}

//Removes first item from the queue.
template <typename T>
void CircularQueueType<T>::DeQueue(T& item)
{
	if (IsEmpty()) throw EmptyQueue();	//비어있을땐 throw EmptyQueue
	m_iFront = (m_iFront + 1) % m_nMaxQueue;
	item = m_pItems[m_iFront];	//m_iFront를 1 증가시켜주고 삭제될 값을 item에 복사해준다.
	m_nLength--;
}

template <typename T>
void CircularQueueType<T>::ResetQueue()
{
	m_curPointer = m_iFront;
}

template <typename T>
int CircularQueueType<T>::GetNextItem(T& Item)
{
	if (IsEmpty()) throw EmptyQueue();

	++m_curPointer %= m_nMaxQueue;
	if (m_curPointer == (m_iRear+1) % m_nMaxQueue) return 0; //큐의 끝에 도달하면 0을 반환
	Item = m_pItems[m_curPointer];
	return 1;
}

template <typename T>
int CircularQueueType<T>::Replace(const T& item, const T& data)
{
	for (int i = m_iFront + 1; i != (m_iRear + 1) % m_nMaxQueue; i = (i + 1) % m_nMaxQueue)
	{
		if (m_pItems[i] == item)
		{
			m_pItems[i] = data;
			return 1;
		}
	}

	return 0;
}

template <typename T>
CircularQueueType<T>& CircularQueueType<T>::operator=(const CircularQueueType<T>& data)
{
	delete[] m_pItems;
	m_nLength = 0;

	m_pItems = new T[data.m_nMaxQueue];
	m_nMaxQueue = data.m_nMaxQueue;
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;

	for (int i = 0; i < data.GetLength(); i++) EnQueue(data.m_pItems[i]);

	return *this;
}

template <typename T>
int CircularQueueType<T>::GetLength() const
{
	return m_nLength;
}

#endif