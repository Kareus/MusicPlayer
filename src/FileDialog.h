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
* 파일/폴더를 선택하는 다이얼로그 클래스. COM Interface를 사용하였고, github의 native-file-dialog를 참고함. (아래는 개선 내용)
* native-file-dialog을 그대로 컴파일 시 strncpy등의 safety 문제가 지적됨
* path에 한국어 등 비라틴계 문자가 포함되어 있을 때 제대로 표시되지 않는 문제가 있어 개량하였음.
* 또한 ShellItem에 다이얼로그의 result를 가져오는 과정에서 문제가 발생해도 success를 반환하는 오류를 수정함.
* @date 2018.11.11
* @author 김성주
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