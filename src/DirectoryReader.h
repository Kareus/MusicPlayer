#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //디렉토리에서 파일 정보를 읽어오기 위한 헤더
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