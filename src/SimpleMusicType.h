#pragma once
#ifndef __SIMPLEMUSICTYPE__
#define __SIMPLEMUSICTYPE__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
/**
*	간단한 음악 정보를 저장하는 클래스
*	@author	김성주
*	@date	2018.10.02
*/

class SimpleMusicType
{
private:
	string id; ///<곡의 primary key
	string name; ///<곡명
	unsigned int time; ///<재생 횟수
	wstring path; ///<파일 경로

public:
	/**
	* 기본 생성자
	*/
	SimpleMusicType();

	/**
	* 복사 생성자
	*/
	SimpleMusicType(const SimpleMusicType& data);

	/**
	* 기본 소멸자
	*/
	~SimpleMusicType();

	/**
	*	@brief	곡명을 반환한다.
	*	@pre	곡명 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	곡명.
	*/
	string GetName() const;

	/**
	*	@brief	곡의 primary key를 반환한다.
	*	@pre	id 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	곡의 primary key.
	*/
	string GetID() const;
	
	/**
	*	@brief	곡의 재생 횟수를 반환한다.
	*	@pre	time 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	곡의 재생 횟수.
	*/
	unsigned int GetPlayedTime() const;

	/**
	*	@brief	곡의 파일 경로를 반환한다.
	*	@pre	path 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	파일 경로.
	*/
	wstring GetPath() const;
	
	/**
	*	@brief	곡명을 설정한다.
	*	@pre	없음.
	*	@post	곡명을 할당한다.
	*	@param	name	설정할 곡의 이름.
	*/
	void SetName(const string& name);

	/**
	*	@brief	곡의 primary key를 설정한다.
	*	@pre	없음.
	*	@post	곡의 primary key를 할당한다.
	*	@param	id	설정할 곡의 primary key.
	*/
	void SetID(const string& id);

	/**
	*	@brief	곡의 재생횟수를 설정한다.
	*	@pre	없음.
	*	@post	곡의 재생횟수를 할당한다.
	*	@param	time	설정할 곡의 재생횟수.
	*/
	void SetPlayedTime(unsigned int time);

	/**
	*	@brief	곡의 파일 경로를 설정한다.
	*	@pre	없음.
	*	@post	곡의 파일 경로를 할당한다.
	*	@param	path	설정할 파일 경로.
	*/
	void SetPath(const wstring& path);

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const SimpleMusicType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const SimpleMusicType& data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key와 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const SimpleMusicType& data) const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	SimpleMusicType& operator=(const SimpleMusicType& data);

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