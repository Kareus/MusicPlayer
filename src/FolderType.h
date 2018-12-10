#pragma once
#ifndef __FOLDER_TYPE__
#define __FOLDER_TYPE__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"
#include <string>

/**
* 폴더 별로 음악을 저장하기 위한 클래스.
* @date 2018.11.26
* @author 김성주
*/
class FolderType
{
private:
	std::wstring dirPath; ///<directory path
	DoublyLinkedList<SimpleMusicType> list; ///<music list

public:

	/**
	*	기본 생성자
	*/
	FolderType();

	/**
	*	복사 생성자
	*/
	FolderType(const FolderType& data);

	/**
	*	기본 소멸자
	*/
	~FolderType();

	/**
	*	@brief	폴더 경로를 반환한다.
	*	@return	폴더 경로
	*/
	std::wstring GetPath() const;

	/**
	*	@brief	폴더 경로를 설정한다.
	*	@param	path	폴더 경로
	*/
	void SetPath(const std::wstring& path);

	/**
	*	@brief	리스트에 음악을 추가한다.
	*	@param	data	추가할 음악 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	리스트에서 음악을 삭제한다.
	*	@param	data	삭제할 음악 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	리스트에서 음악을 교체한다.
	*	@param	data	교체할 음악 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	@brief	리스트에서 음악을 찾는다.
	*	@param	data	추가할 음악 데이터
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int GetMusic(SimpleMusicType& data);

	/**
	*	@brief	리스트의 음악 수를 반환한다.
	*	@return	음악 수
	*/
	int GetLength() const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	FolderType& operator=(const FolderType& data);

	/**
	*	@brief	폴더 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return	폴더 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<SimpleMusicType> GetIterator();

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const FolderType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const FolderType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터와 비교하는 데이터가 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const FolderType& data) const;
};
#endif