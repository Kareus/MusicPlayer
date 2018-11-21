#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //���丮���� ���� ������ �о���� ���� ���
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