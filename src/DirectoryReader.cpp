#include "DirectoryReader.h"
#include "shlobj.h"
#include <iostream>
#include "FileDialog.h"

using namespace std;

DirectoryReader::DirectoryReader()
{
	pathList.clear();
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
	pathList.clear();

	WDIR *dir;
	struct wdirent *ent;

	if ((dir = wopendir(filepath.c_str())) != NULL) {
		/* print all the files and directories within directory */

		while ((ent = wreaddir(dir)) != NULL) {
			wstring name = ent->d_name;
			pathList.push_back(name);

			wcout << name << L' ';

			if (name.size() < 4)
			{
				cout << endl;
				continue;
			}

			wstring ext = name.substr(name.size() - 4, 4).c_str();
			wcout << ext;
			if (!wcscmp(ext.c_str(), L".mp3"))
			{
				cout << " It is mp3 file";
			}

			cout << endl;
		}

		wclosedir(dir);
	}
	else {
		cout << "Cannot open the directory" << endl;
		return 0;
	}

	return 1;
}