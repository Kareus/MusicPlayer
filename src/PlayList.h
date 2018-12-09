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
	AVLTree<SimpleMusicType> list;
	int id;

public:
	PlayList();

	PlayList(const PlayList& data);

	~PlayList();

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
};
#endif