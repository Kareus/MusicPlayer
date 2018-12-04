#pragma once
#ifndef __ARTIST__
#define __ARTIST__

#include "SimpleMusicType.h"

class Artist
{
private:
	string name; ///<이름
	DoublyLinkedList<SimpleMusicType> musicList; ///<음악 리스트
	string ID; ///<고유 키

public:
	/**
	* 기본 생성자
	*/
	Artist();

	/**
	* 복사 생성자
	*/
	Artist(const Artist& data);

	/**
	* 기본 소멸자
	*/
	~Artist();

	/**
	*	@brief	가수의 이름을 반환한다.
	*	@pre	name 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	가수의 이름.
	*/
	string GetName() const;

	/**
	*	@brief	가수가 작업한 곡의 개수를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	곡의 개수.
	*/
	unsigned int GetMusicNum() const;

	/**
	*	@brief	가수의 고유 키를 반환한다.
	*	@pre	없음.
	*	@post	없음.
	*	@return	고유 키.
	*/
	string GetID() const;

	/**
	*	@brief	가수의 이름을 설정한다.
	*	@pre	없음.
	*	@post	name 변수를 할당한다.
	*	@param	name	설정할 가수의 이름.
	*/
	void SetName(const string& name);

	/**
	*	@brief	가수의 고유 키를 설정한다.
	*	@pre	없음.
	*	@post	ID 변수를 할당한다.
	*	@param	id	설정할 가수의 고유 키.
	*/
	void SetID(const string& id);

	/**
	*	@brief	음악을 추가한다.
	*	@pre	없음.
	*	@post	음악이 리스트에 추가된다.
	*	@param	data	추가할 음악 데이터.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	음악을 삭제한다.
	*	@pre	없음.
	*	@post	주어진 데이터와 같은 음악명을 가진 앨범을 찾아 삭제한다.
	*	@param	data	삭제할 음악 데이터.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	주어진 데이터과 이름이 같은 음악을 찾아 교체한다.
	*	@pre	없음.
	*	@post	같은 이름의 음악 데이터가 교체된다.
	*	@param	data	교체할 음악 데이터.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 name, birthDate가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	이름이 알파벳순으로 앞에 오는지, 같다면 생년월일이 빠른지 검사하여 앞에 오면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const Artist& data) const;
	
	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 name, birthDate가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	이름이 알파벳순으로 뒤에 오는지, 같다면 생년월일이 늦는지 검사하여 앞에 오면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const Artist& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 name, birthDate가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	이름과 생년월일을 검사하여 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const Artist& data) const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	Artist& operator=(const Artist& data);

	/**
	*	@brief	파일로부터 아티스트의 정보를 읽는다.
	*	@pre	해당 파일의 스트림이 열려 있어야 한다.
	*	@post	아티스트의 정보를 할당한다.
	*	@param	fin	읽을 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	파일에 아티스트의 정보를 저장한다.
	*	@pre	해당 파일의 스트림이 열려 있어야 하며, 데이터 리스트가 초기화되어 있어야 한다.
	*	@post	해당 파일에 아티스트의 정보가 저장된다.
	*	@param	fout	정보를 쓸 파일의 스트림.
	*	@return	성공하면 1, 아니면 0을 반환.
	*/
	int WriteDataToFile(ofstream& fout);

	/**
	*	@brief	앨범 리스트에 관련한 iterator를 생성한다.
	*	@pre	없음.
	*	@post	iterator 객체가 생성된다.
	*	@return	앨범 리스트에 대한 iterator 객체.
	*/
	DoublyIterator<SimpleMusicType>& GetIterator() const;
};
#endif