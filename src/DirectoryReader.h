#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //���丮���� ���� ������ �о���� ���� ���
#include "Windows.h"
#include <string>
#include <vector>

class DirectoryReader
{
private:
	std::vector<std::wstring> pathList; //temporary

public:
	DirectoryReader();
	void openDirWithFileBox();
	bool openDirectory(const std::wstring& filepath);
};
#endif