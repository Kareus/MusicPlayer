#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //���丮���� ���� ������ �о���� ���� ���
#include "Windows.h"
#include <string>
#include "DoublyLinkedList.h"

class DirectoryReader
{
private:
	DoublyLinkedList<std::wstring> pathList;

public:
	DirectoryReader();
	void openDirWithFileBox();
	bool openDirectory(const std::wstring& filepath);
	DoublyIterator<std::wstring> getIterator();
};
#endif