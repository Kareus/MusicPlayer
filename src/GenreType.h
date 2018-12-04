#pragma once
#ifndef __GENRETYPE__
#define __GENRETYPE__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"

/**
*	장르 정보를 저장하는 클래스
*	@author	김성주
*	@date	2018.10.02
*/
class GenreType
{
private:
	string genreName; ///<장르명
	DoublyLinkedList<SimpleMusicType> list; ///<음악 정보를 담는 리스트

public:
	/**
	* 기본 생성자
	*/
	GenreType();

	/**
	* 복사 생성자
	*/
	GenreType(const GenreType& data);

	/**
	* 기본 소멸자
	*/
	~GenreType();

	/**
	*	@brief	장르를 반환한다.
	*	@pre	장르 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	장르.
	*/
	string GetGenre() const;

	/**
	*	@brief	장르명을 설정한다.
	*	@pre	없음.
	*	@post	장르명을 할당한다.
	*	@param	genre	설정할 장르명.
	*/
	void SetGenre(const string& genre);

	/**
	*	@brief	음악 정보를 장르의 음악 리스트에 추가한다.
	*	@pre	parameter의 음악 정보에 필요한 정보가 할당되어 있어야 한다.
	*	@post	parameter의 음악 정보가 리스트에 추가된다.
	*	@param	music	추가할 음악 정보.
	*	@return 추가에 성공하면 1, 실패하면 0을 반환한다.
	*/
	int AddMusic(const SimpleMusicType& music);

	/**
	*	@brief	음악 정보를 장르의 음악 리스트에서 갱신한다.
	*	@pre	리스트에 음악 정보가 존재하고, parameter의 음악 정보에 필요한 정보가 할당되어 있어야 한다.
	*	@post	parameter의 음악 정보가 리스트에서 갱신된다.
	*	@param	music	갱신할 음악 정보.
	*	@return 갱신에 성공하면 1, 실패하면 0을 반환한다.
	*/
	int ReplaceMusic(const SimpleMusicType& music);

	/**
	*	@brief	음악 정보를 장르의 음악 리스트에서 삭제한다.
	*	@pre	리스트에 음악 정보가 존재하고, parameter의 음악 정보에 필요한 정보가 할당되어 있어야 한다.
	*	@post	parameter의 정보와 일치하는 음악 정보가 리스트에서 삭제된다.
	*	@param	music	삭제할 음악 정보.
	*	@return 삭제에 성공하면 1, 실패하면 0을 반환한다.
	*/
	int DeleteMusic(const SimpleMusicType& music);

	/**
	*	@brief	parameter와 일치하는 음악 정보를 parameter에 저장한다.
	*	@pre	리스트에 음악 정보가 존재하고, parameter의 음악 정보에 필요한 정보가 할당되어 있어야 한다.
	*	@post	parameter에 찾은 음악 정보가 할당된다.
	*	@param	music	검색할 음악 정보.
	*	@return	찾으면 1, 찾지 못하면 0을 리턴한다.
	*/
	int GetMusic(SimpleMusicType& music);

	/**
	*	@brief	장르에 해당하는 음악의 수를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	음악의 수.
	*/
	int GetMusicNum() const;
	
	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const GenreType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const GenreType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key와 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const GenreType& data) const;
	
	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	GenreType& operator=(const GenreType& data);

	/**
	*	@brief	장르의 수록곡 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return 장르의 수록곡 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<SimpleMusicType>& GetIterator() const;
};

#endif