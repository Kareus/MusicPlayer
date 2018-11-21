#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //디렉토리에서 파일 정보를 읽어오기 위한 헤더
#include "Windows.h"
#include <string>
#include "AVLTree.h"

class DirectoryReader
{
private:
	AVLTree<std::wstring> pathList;

public:
	DirectoryReader();
	void openDirWithFileBox();
	bool openDirectory(const std::wstring& filepath);
	AVLTree<std::wstring>& getTree() { return pathList; }
};
#endif