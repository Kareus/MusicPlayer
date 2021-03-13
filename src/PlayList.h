#pragma once
#ifndef __PLAYLIST__
#define __PLAYLIST__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"

/**
* 재생 목록을 저장하는 클래스
* @date 2018.11.26
* @author Kareus
*/
class PlayList
{
private:
	DoublyLinkedList<SimpleMusicType> list; ///<음악 목록을 저장하는 리스트
	int id; ///<primary key

public:

	/**
	*	기본 생성자
	*/
	PlayList();

	/**
	*	복사 생성자
	*/
	PlayList(const PlayList& data);

	/**
	*	기본 소멸자
	*/
	~PlayList();

	/**
	*	@brief	리스트를 비우는 함수
	*/
	void MakeEmpty();

	/**
	*	@brief	음악을 리스트에 추가한다
	*	@param	data	추가할 음악
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	음악을 리스트에서 삭제한다
	*	@param	data	삭제할 음악
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	음악을 리스트에서 교체한다
	*	@param	data	교체할 음악
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	@brief	음악을 리스트에서 가져온다
	*	@param	data	가져올 음악
	*	@return	성공하면 1, 아니면 0을 반환한다.
	*/
	int GetMusic(SimpleMusicType& data);

	/**
	*	@brief	플레이 리스트의 음악 수를 반환한다.
	*	@return	음악 수
	*/
	int GetMusicNum() const;

	/**
	*	@brief	primary key를 설정한다.
	*	@param	id	설정할 ID
	*/
	void SetID(int id);

	/**
	*	@brief	플레이 리스트의 primary key를 반환한다.
	*	@return	ID
	*/
	int GetID() const;

	/**
	*	@brief	플레이 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return	플레이 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<SimpleMusicType> GetIterator();

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	PlayList& operator=(const PlayList& data);
	
	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const PlayList& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const PlayList& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터와 비교하는 데이터가 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const PlayList& data) const;

	/**
	*	@brief	파일로부터 음악의 정보를 읽는다.
	*	@pre	해당 파일의 스트림이 열려 있어야 한다.
	*	@post	음악의 정보를 할당한다.
	*	@param	fin	읽을 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	파일에 음악의 정보를 저장한다.
	*	@pre	해당 파일의 스트림이 열려 있어야 하며, 데이터 리스트가 초기화되어 있어야 한다.
	*	@post	해당 파일에 음악의 정보가 저장된다.
	*	@param	fout	정보를 쓸 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int WriteDataToFile(ofstream& fout);
};
#endif