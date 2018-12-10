#pragma once
#ifndef __PLAYLIST__
#define __PLAYLIST__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"

/**
* ��� ����� �����ϴ� Ŭ����
* @date 2018.11.26
* @author �輺��
*/
class PlayList
{
private:
	DoublyLinkedList<SimpleMusicType> list;
	int id;

public:
	PlayList();

	PlayList(const PlayList& data);

	~PlayList();

	void MakeEmpty();

	int AddMusic(const SimpleMusicType& data);

	int DeleteMusic(const SimpleMusicType& data);

	int ReplaceMusic(const SimpleMusicType& data);

	int GetMusic(SimpleMusicType& data);

	int GetMusicNum() const;

	void SetID(int id);

	int GetID() const;

	DoublyIterator<SimpleMusicType> GetIterator();

	PlayList& operator=(const PlayList& data);
	
	bool operator<(const PlayList& data) const;
	bool operator>(const PlayList& data) const;
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