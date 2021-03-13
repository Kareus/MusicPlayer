#pragma once
#ifndef __FILE_DIALOG__
#define __FILE_DIALOG__

#include <stdexcept>
#include <string>
#include <wchar.h>
#include <assert.h>
#include <windows.h>
#include <ShObjIdl.h>

//파일 상자에서 반환한 결과
enum DialogResult
{
	DIALOG_SUCCESS, DIALOG_CANCEL, DIALOG_ERROR
};

/**
*	@brief	파일 상자에서 발생한 오류를 반환하는 예외 클래스
*	@author	Kareus
*/

class filedialog_error : public std::exception
{
private:
	std::string mes; //메시지
public:
	/**
	*	기본 생성자
	*	@param	err	에러 메시지
	*/
	filedialog_error(const std::string& err) : mes(err) {};

	/**
	*	@brief	메시지 반환 함수
	*	@return	메시지
	*/
	virtual const char* what() const throw() { return mes.c_str(); }
};

/**
* 파일/폴더를 선택하는 다이얼로그 클래스. COM Interface를 사용하였고, github의 native-file-dialog를 참고함. (아래는 개선 내용)
* native-file-dialog을 그대로 컴파일 시 strncpy등의 safety 문제가 지적됨
* path에 한국어 등 비라틴계 문자가 포함되어 있을 때 제대로 표시되지 않는 문제가 있어 개량하였음.
* 또한 ShellItem에 다이얼로그의 result를 가져오는 과정에서 문제가 발생해도 success를 반환하는 오류를 수정함.
* @date 2018.11.11
* @author Kareus
*/

class FileDialog
{
private:
	std::wstring defaultPath; ///<기본 경로

	/**
	*	@brief	해당 포인터 주소의 메모리를 해제하는 함수
	*	@param	ptr	포인터 주소
	*/
	void free(void* ptr);

	/**
	*	@brief	해당 크기 만큼의 메모리 공간을 동적 할당하여 반환하는 함수
	*	@param	bytes	할당할 메모리 크기
	*	@return	할당된 메모리의 주소
	*/
	void* allocate(size_t bytes);

	/**
	*	@brief	해당 문자가 필터 구분 문자인지 반환하는 함수
	*	@param	ch	문자
	*	@return	해당 문자가 필터 구분 문자이면 true, 아니면 false를 반환한다.
	*/
	bool isFilterSegment(wchar_t ch);

	/**
	*	@brief	파일 상자의 확장자를 설정하는 함수
	*	@pre	확장자 목록은 비어있지 않아야 한다.
	*	@post	파일 상자의 확장자를 설정한다.
	*	@param	dialog	파일 상자
	*	@param	description	확장자의 설명 목록
	*	@param	filter	확장자 목록
	*	@return	성공하면 DIALOG_SUCCESS, 아니면 DIALOG_ERROR를 반환한다.
	*/
	DialogResult setFilterToDialog(IFileDialog* dialog, const std::wstring& description, const std::wstring& filter);

	/**
	*	@brief	기본 경로를 설정하는 함수
	*	@param	dialog	파일 상자
	*	@return	성공하면 DIALOG_SUCCESS, 아니면 DIALOG_ERROR를 반환한다.
	*/
	DialogResult setDefaultPath(IFileDialog* dialog);

public:

	/**
	*	@brief	기본 생성자
	*	@param	default_path	기본 경로
	*/
	FileDialog(const std::wstring& default_path = L"") : defaultPath(default_path) {};

	/**
	*	@brief	폴더 경로를 파일 상자를 열어 선택한다.
	*	@param	resultPath	선택한 경로를 저장할 문자열
	*	@return	성공하면 DIALOG_SUCCESS, 취소하면 DIALOG_CANCEL, 아니면 DIALOG_ERROR를 반환한다.
	*/
	DialogResult openDirectoryDialog(std::wstring& resultPath);

	/**
	*	@brief	파일 경로를 파일 상자를 열어 선택한다.
	*	@pre	description 및 filter는 같은 필터 내에선 ','로 구분해야 하고 다른 필터는 ';'로 구분해야 한다.
	*	@post	파일 상자를 열고 파일 경로를 선택한다.
	*	@param	resultPath	선택한 경로를 저장할 문자열
	*	@param	description	확장자의 설명 목록
	*	@param	filter	확장자 목록
	*	@return	성공하면 DIALOG_SUCCESS, 취소하면 DIALOG_CANCEL, 아니면 DIALOG_ERROR를 반환한다.
	*/
	DialogResult openFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter);

	/**
	*	@brief	파일을 저장할 경로를 파일 상자를 열어 선택한다.
	*	@pre	description 및 filter는 같은 필터 내에선 ','로 구분해야 하고 다른 필터는 ';'로 구분해야 한다.
	*	@post	파일 상자를 열고 파일 경로를 선택한다.
	*	@param	resultPath	선택한 경로를 저장할 문자열
	*	@param	description	확장자의 설명 목록
	*	@param	filter	확장자 목록
	*	@param	filterType	선택한 확장자 번호가 무엇인지 저장할 변수
	*	@return	성공하면 DIALOG_SUCCESS, 취소하면 DIALOG_CANCEL, 아니면 DIALOG_ERROR를 반환한다.
	*/
	DialogResult saveFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter, UINT& filterType);
};
#endif