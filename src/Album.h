#pragma once
#ifndef __ALBUM__
#define __ALBUM__

#include "DoublyLinkedList.h"
#include "SimpleMusicType.h"
#include "MusicType.h"
#include <string>

using namespace std;

/**
*	앨범 정보를 저장하는 클래스
*	@author	김성주
*	@date	2018.09.24
*/

class Album
{
private:
	string album; ///<앨범명
	string artist; ///<가수
	string ID; ///<고유 키
	DoublyLinkedList<SimpleMusicType> list; ///<수록곡 리스트

public:

	/**
	* 기본 생성자
	*/
	Album();

	/**
	* 복사 생성자
	*/
	Album(const Album& data);

	/**
	* 기본 소멸자
	*/
	~Album();

	/**
	*	@brief	앨범명을 반환한다.
	*	@pre	album 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	앨범명.
	*/
	string GetAlbumName() const;

	/**
	*	@brief	가수의 이름을 반환한다.
	*	@pre	artist 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	가수의 이름.
	*/
	string GetArtist() const;

	/**
	*	@brief	수록곡 수를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	수록곡 수.
	*/
	unsigned int GetMusicNum() const;

	/**
	*	@brief	앨범의 고유 키를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	고유 키.
	*/
	string GetID() const;

	/**
	*	@brief	앨범명을 설정한다.
	*	@pre	없음.
	*	@post	album 변수를 할당한다.
	*	@param	name	설정할 앨범의 이름.
	*/
	void SetAlbumName(const string& name);

	/**
	*	@brief	가수의 이름을 설정한다.
	*	@pre	없음.
	*	@post	artist 변수를 할당한다.
	*	@param	artist	설정할 가수의 이름.
	*/
	void SetArtist(const string& artist);

	/**
	*	@brief	앨범의 고유 키를 설정한다.
	*	@pre	없음.
	*	@post	ID 변수를 할당한다.
	*	@param	id	설정할 앨범의 고유 키.
	*/
	void SetID(const string& id);

	/**
	*	@brief	수록곡 리스트에 해당 음악 데이터를 추가한다.
	*	@pre	data는 곡명, primary key, 재생시간 정보만 가지고 있어야 한다.
	*	@post	data가 수록곡 리스트에 추가된다.
	*	@param	data	추가할 음악 데이터
	*	@return	성공하면 1, 실패하면 0을 반환.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	수록곡 리스트에서 주어진 음악 데이터의 키와 동일한 키를 가진 음악을 삭제한다.
	*	@pre	data는 primary key를 가지고 있어야 한다.
	*	@post	찾은 음악 데이터를 삭제한다.
	*	@param	data	삭제할 음악 데이터의 키를 가진 데이터.
	*	@return	성공하면 1, 실패하면 0을 반환.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	수록곡 리스트에서 주어진 키를 가진 음악 데이터를 찾아 삭제한다.
	*	@pre	없음.
	*	@post	찾은 음악 데이터를 삭제한다.
	*	@param	key	삭제할 데이터의 primary key.
	*	@return	성공하면 1, 실패하면 0을 반환.
	*/
	int DeleteMusicFrom(const string& key);

	/**
	*	@brief	수록곡 리스트에서 주어진 음악 데이터의 키와 동일한 키를 가진 음악을 찾아 교체한다.
	*	@pre	data는 곡명, primary key, 재생시간 정보만 가지고 있어야 한다.
	*	@post	찾은 음악 데이터를 주어진 데이터로 교체한다.
	*	@param	data	교체할 음악 데이터
	*	@return	성공하면 1, 실패하면 0을 반환.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	두 아이템 데이터를 비교한다.
	*	@brief	두 아이템의 순서를 비교한다.
	*	@pre	두 아이템에서 필요한 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터.
	*	@return	현재 아이템이 비교 아이템보다 작다면 LESS를 반환,
	*			현재 아이템이 비교 아이템보다 크다면 GREATER를 반환,
	*			그렇지 않은 경우엔 (같다면) EQUAL을 반환.
	*/
	RelationType Compare(const Album& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 album, record, artist가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	앨범명, 가수명, 음반회사명이 알파벳순으로 앞에 오는지를 순서대로 검사하여 앞에 오면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const Album& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 album, record, artist가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	앨범명, 가수명, 음반회사명이 알파벳순으로 뒤에 오는지를 순서대로 검사하여 뒤에 오면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const Album& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 album, record, artist가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	앨범명, 가수명, 음반회사명을 순서대로 검사하여 동일하면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const Album& data) const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	Album& operator=(const Album& data);

	/**
	*	@brief	파일로부터 앨범의 정보를 읽는다.
	*	@pre	해당 파일의 스트림이 열려 있어야 한다.
	*	@post	앨범의 정보를 할당한다.
	*	@param	fin	읽을 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	파일에 앨범의 정보를 저장한다.
	*	@pre	해당 파일의 스트림이 열려 있어야 하며, 데이터 리스트가 초기화되어 있어야 한다.
	*	@post	해당 파일에 앨범의 정보가 저장된다.
	*	@param	fout	정보를 쓸 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int WriteDataToFile(ofstream& fout);

	/**
	*	@brief	앨범의 수록곡 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return	앨범의 수록곡 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<SimpleMusicType>& GetIterator() const;

};
#endif