#pragma once
#ifndef __PLAYLIST__
#define __PLAYLIST__

#include "SimpleMusicType.h"
#include "AVLTree.h"

/**
* 재생 목록을 저장하는 클래스
* @date 2018.11.26
* @author 김성주
*/
class PlayList
{
private:
	AVLTree<SimpleMusicType> list;

public:
	PlayList();

	PlayList(const PlayList& data);

	~PlayList();

	int AddMusic(const SimpleMusicType& data);

	int DeleteMusic(const SimpleMusicType& data);

	int ReplaceMusic(const SimpleMusicType& data);

	int GetMusic(SimpleMusicType& data);

	PlayList& operator=(const PlayList& data);
	
	bool operator<(const PlayList& data) const;
	bool operator>(const PlayList& data) const;
	bool operator==(const PlayList& data) const;
};
#endif