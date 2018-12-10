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
*	@details	This class processes as First In, First Out (FIFO), ���ø��� ������ �پ��� ���� Ÿ���� ������ �� �ִ�.
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
	*	@post	m_iFront�� m_iRear is set same value as Constructer.
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
	*	@brief	ť�� �����͸� �����Ѵ�.
	*	@pre	����.
	*	@post	ť�� �����Ͱ� ���µȴ�.
	*/
	void ResetQueue();

	/**
	*	@brief	���� �������� item�� �Ҵ��Ѵ�
	*	@pre	����.
	*	@post	ť�� ����ִٸ� EmptyQueue�� ȣ��ǰ�, �ƴ϶�� ���� �������� item�� �Ҵ�ȴ�. �� ��, �����ϸ� �ε����� ��ȯ�ϰ� ť�� ���̶�� -1�� ��ȯ�Ѵ�.
	*/
	int GetNextItem(T &item);

	/**
	*	@brief	find �������� ã�� data�� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	ť�� ����ִٸ� EmptyQueue�� ȣ��ǰ�, �ƴ϶�� find�� ã�� data�� ��ü�Ѵ�. �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Replace(const T &find, const T &data);

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	CircularQueueType<T>& operator=(const CircularQueueType<T>& data);

	/**
	*	@brief	ť�� �������� ������ ��ȯ�Ѵ�.
	*	@return	ť�� ������ ����
	*/
	int GetLength() const;

private:
	int m_iFront;	//index of one infront of the first element.
	int m_iRear;	//index of the last element.
	int m_nMaxQueue;	//max size of the queue.
	int m_nLength; //���� ť�� �ִ� ������ ����
	int m_curPointer;	//���� ������
	T* m_pItems;	//pointer for dynamic allocation.
};

//Allocate dynamic array whose size is maxQueue.
template <typename T>
CircularQueueType<T>::CircularQueueType()
{
	m_pItems = new T[maxQueue];	//m_pItems�� ũ�Ⱑ maxQueue�� �迭�� �����.
	m_nMaxQueue = maxQueue;
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//��� �������� �ʱ�ȭ�����ش�.
	m_nLength = 0;
}

//Allocate dynamic array whose size is max.
template <typename T>
CircularQueueType<T>::CircularQueueType(int max)
{
	m_pItems = new T[max];	//m_pItems�� ũ�Ⱑ max�� �迭�� �����.
	m_nMaxQueue = max;
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//��� �������� �ʱ�ȭ�����ش�.
	m_nLength = 0;
}

//Destruct the object. Free the array dynamically allocated.
template <typename T>
CircularQueueType<T>::~CircularQueueType()
{
	delete[] m_pItems;	//�����Ҵ��ߴ� �޸𸮸� �������ش�.
}

//Determines whether the queue is full.
template <typename T>
bool CircularQueueType<T>::IsFull()
{
	if (m_iFront - m_iRear == 1 || m_iRear - m_iFront == m_nMaxQueue - 1)
		return 1;	//m_iFront�� m_iRear���� 1 ���϶��� �� �����̹Ƿ� 1�� �����Ѵ�. m_iFront�� 0�϶��� m_iRear�� m_nMaxQueue-1�� ������ �� �����̴�.
	return 0;	//�� ���� �ʾ����� 0�� �����Ѵ�.
}

//Determines whether the queue is empty.
template <typename T>
bool CircularQueueType<T>::IsEmpty()
{
	if (m_iFront == m_iRear)
		return 1;	//m_iFront�� m_iRear�� ���� ���� queue�� ����ִ� �����̹Ƿ� 1�� �����Ѵ�.
	return 0;	//������� ������ 0�� �����Ѵ�.
}

//Makes the queue empty.
template <typename T>
void CircularQueueType<T>::MakeEmpty()
{
	m_iFront = m_nMaxQueue - 1;
	m_iRear = m_nMaxQueue - 1;	//�����ڿ� ���� ������ m_iFront�� m_iRear�� �������ش�.
}

//Adds newItem to the last of the queue.
template <typename T>
void CircularQueueType<T>::EnQueue(const T& item)
{
	if (IsFull()) throw FullQueue();	//�� á���� throw FullQueue
	m_iRear = (m_iRear + 1) % m_nMaxQueue;
	m_pItems[m_iRear] = item;	//m_iRear�� 1���������ְ� item���� �־��ش�.
	m_nLength++;
}

//Removes first item from the queue.
template <typename T>
void CircularQueueType<T>::DeQueue(T& item)
{
	if (IsEmpty()) throw EmptyQueue();	//��������� throw EmptyQueue
	m_iFront = (m_iFront + 1) % m_nMaxQueue;
	item = m_pItems[m_iFront];	//m_iFront�� 1 ���������ְ� ������ ���� item�� �������ش�.
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
	if (m_curPointer == (m_iRear+1) % m_nMaxQueue) return 0; //ť�� ���� �����ϸ� 0�� ��ȯ
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