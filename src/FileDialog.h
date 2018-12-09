#pragma once
#ifndef __FILE_DIALOG__
#define __FILE_DIALOG__

#include <stdexcept>
#include <string>
#include <wchar.h>
#include <assert.h>
#include <windows.h>
#include <ShObjIdl.h>

enum DialogResult
{
	DIALOG_SUCCESS, DIALOG_CANCEL, DIALOG_ERROR
};

class filedialog_error : public std::exception
{
private:
	std::string mes;
public:
	filedialog_error(const std::string& err) : mes(err) {};

	virtual const char* what() const throw() { return mes.c_str(); }
};

/**
* ����/������ �����ϴ� ���̾�α� Ŭ����. COM Interface�� ����Ͽ���, github�� native-file-dialog�� ������. (�Ʒ��� ���� ����)
* native-file-dialog�� �״�� ������ �� strncpy���� safety ������ ������
* path�� �ѱ��� �� ���ƾ�� ���ڰ� ���ԵǾ� ���� �� ����� ǥ�õ��� �ʴ� ������ �־� �����Ͽ���.
* ���� ShellItem�� ���̾�α��� result�� �������� �������� ������ �߻��ص� success�� ��ȯ�ϴ� ������ ������.
* @date 2018.11.11
* @author �輺��
*/

class FileDialog
{
private:
	std::wstring defaultPath;

	void free(void* ptr);
	void* allocate(size_t bytes);
	bool isFilterSegment(wchar_t ch);
	DialogResult setFilterToDialog(IFileDialog* dialog, const std::wstring& description, const std::wstring& filter);
	DialogResult setDefaultPath(IFileDialog* dialog);

public:
	FileDialog(const std::wstring& default_path = L"") : defaultPath(default_path) {};

	DialogResult openDirectoryDialog(std::wstring& resultPath);
	DialogResult openFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter);
	DialogResult saveFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter, UINT& filterType);
};
#endif