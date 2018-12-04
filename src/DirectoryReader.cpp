#include "DirectoryReader.h"
#include "shlobj.h"
#include <iostream>
#include "FileDialog.h"

using namespace std;

DirectoryReader::DirectoryReader()
{
	pathList.MakeEmpty();
}

void DirectoryReader::openDirWithFileBox()
{
	wstring path = L"";
	FileDialog dialog;
	if (dialog.openDirectoryDialog(path) != DIALOG_SUCCESS)
	{
		cout << "error!" << endl;
		return;
	}
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
			OutputDebugStringW((name + L'\n').c_str());
		}

		wclosedir(dir);
	}
	else {
		cout << "Cannot open the directory" << endl;
		return 0;
	}

	return 1;
}

DoublyIterator<std::wstring> DirectoryReader::getIterator()
{
	return DoublyIterator<std::wstring>(pathList);
}