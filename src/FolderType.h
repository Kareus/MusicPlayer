#pragma once
#ifndef __FOLDER_TYPE__
#define __FOLDER_TYPE__

#include "SimpleMusicType.h"
#include "AVLTree.h"
#include <string>

/**
* 폴더 별로 음악을 저장하기 위한 클래스.
* @date 2018.11.26
* @author 김성주
*/
class FolderType
{
private:
	std::wstring dirPath; ///<directory path
	AVLTree<SimpleMusicType> list; ///<music list

public:
	FolderType();

	FolderType(const FolderType& data);

	~FolderType();

	std::wstring GetPath() const;

	void SetPath(const std::wstring& path);

	int AddMusic(const SimpleMusicType& data);

	int DeleteMusic(const SimpleMusicType& data);

	int ReplaceMusic(const SimpleMusicType& data);

	int GetMusic(SimpleMusicType& data);

	int GetLength() const;

	FolderType& operator=(const FolderType& data);

	bool operator<(const FolderType& data) const;
	bool operator>(const FolderType& data) const;
	bool operator==(const FolderType& data) const;
};
#endif