#include "DirectoryReader.h"
#include "shlobj.h"
#include <iostream>
#include "FileDialog.h"

using namespace std;

DirectoryReader::DirectoryReader()
{
	pathList.MakeEmpty();
}

DirectoryReader::~DirectoryReader()
{
	pathList.MakeEmpty();
}

void DirectoryReader::openDirWithFileBox()
{
	wstring path = L"";
	FileDialog dialog;
	if (dialog.openDirectoryDialog(path) != DIALOG_SUCCESS) return;
	else openDirectory(path);
}

bool DirectoryReader::openDirectory(const wstring& filepath)
{
	pathList.MakeEmpty();

	WDIR *dir;
	struct wdirent *ent;

	if ((dir = wopendir(filepath.c_str())) != NULL) {
		//디렉토리 및 파일을 찾아서 추가

		while ((ent = wreaddir(dir)) != NULL) {
			wstring name = ent->d_name;
			pathList.Add(name);
		}

		wclosedir(dir);
	}
	else return 0;

	return 1;
}

DoublyIterator<std::wstring> DirectoryReader::getIterator()
{
	return DoublyIterator<std::wstring>(pathList);
}