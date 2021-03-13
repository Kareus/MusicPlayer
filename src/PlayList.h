#pragma once
#ifndef __PLAYLIST__
#define __PLAYLIST__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"

/**
* ��� ����� �����ϴ� Ŭ����
* @date 2018.11.26
* @author Kareus
*/
class PlayList
{
private:
	DoublyLinkedList<SimpleMusicType> list; ///<���� ����� �����ϴ� ����Ʈ
	int id; ///<primary key

public:

	/**
	*	�⺻ ������
	*/
	PlayList();

	/**
	*	���� ������
	*/
	PlayList(const PlayList& data);

	/**
	*	�⺻ �Ҹ���
	*/
	~PlayList();

	/**
	*	@brief	����Ʈ�� ���� �Լ�
	*/
	void MakeEmpty();

	/**
	*	@brief	������ ����Ʈ�� �߰��Ѵ�
	*	@param	data	�߰��� ����
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	������ ����Ʈ���� �����Ѵ�
	*	@param	data	������ ����
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	������ ����Ʈ���� ��ü�Ѵ�
	*	@param	data	��ü�� ����
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	@brief	������ ����Ʈ���� �����´�
	*	@param	data	������ ����
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int GetMusic(SimpleMusicType& data);

	/**
	*	@brief	�÷��� ����Ʈ�� ���� ���� ��ȯ�Ѵ�.
	*	@return	���� ��
	*/
	int GetMusicNum() const;

	/**
	*	@brief	primary key�� �����Ѵ�.
	*	@param	id	������ ID
	*/
	void SetID(int id);

	/**
	*	@brief	�÷��� ����Ʈ�� primary key�� ��ȯ�Ѵ�.
	*	@return	ID
	*/
	int GetID() const;

	/**
	*	@brief	�÷��� ����Ʈ�� ������ iterator�� �����Ѵ�.
	*	@pre	����.
	*	@post	iterator ��ü�� �����ȴ�.
	*	@return	�÷��� ����Ʈ�� ���� iterator ��ü.
	*/
	DoublyIterator<SimpleMusicType> GetIterator();

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	PlayList& operator=(const PlayList& data);
	
	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const PlayList& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const PlayList& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ϳ� ���ϴ� �����Ͱ� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const PlayList& data) const;

	/**
	*	@brief	���Ϸκ��� ������ ������ �д´�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �Ѵ�.
	*	@post	������ ������ �Ҵ��Ѵ�.
	*	@param	fin	���� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	���Ͽ� ������ ������ �����Ѵ�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �ϸ�, ������ ����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�ش� ���Ͽ� ������ ������ ����ȴ�.
	*	@param	fout	������ �� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int WriteDataToFile(ofstream& fout);
};
#endif