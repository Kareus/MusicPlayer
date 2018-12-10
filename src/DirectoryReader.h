#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //디렉토리에서 파일 정보를 읽어오기 위한 헤더
#include "Windows.h"
#include <string>
#include "DoublyLinkedList.h"

/**
*	@brief	폴더 경로에서 파일 목록을 읽어오는 클래스
*	@author	김성주
*/
class DirectoryReader
{
private:
	DoublyLinkedList<std::wstring> pathList; ///<파일 경로 리스트

public:

	/**
	*	기본 생성자
	*/
	DirectoryReader();

	/**
	*	기본 소멸자
	*/
	~DirectoryReader();

	/**
	*	@brief	파일 상자를 통해 폴더 경로를 열어 읽어오는 함수
	*/
	void openDirWithFileBox();

	/**
	*	@brief	경로를 받아와 파일 목록을 읽어오는 함수'
	*	@param	filepath	폴더 경로
	*	@return	읽어오는 데 성공하면 true, 아니면 false를 반환한다.
	*/
	bool openDirectory(const std::wstring& filepath);

	/**
	*	@brief	경로 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return	경로 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<std::wstring> getIterator();
};
#endif