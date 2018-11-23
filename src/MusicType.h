#ifndef __MusicTYPE__
#define __MusicTYPE__
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <functional>
#include "SimpleMusicType.h"

using namespace std;

/**
*	두 아이템 간의 관계를 나타낸다
*/
enum RelationType { LESS, GREATER, EQUAL };

/**
*	음악 정보를 저장하는 클래스
*	@author	김성주
*	@date	2018.09.23
*/

class MusicType {
private:
	string name; ///<곡명
	string artist; ///<가수
	string composer; ///<작곡가
	string writer; ///<작사가
	string record; ///<레코드
	unsigned int date; ///<발표일
	string album; ///<앨범
	string genre; ///<장르
	unsigned int length; ///<재생 길이 (second)
	unsigned int time; ///<재생 횟수
	string note;	///<악보
	string lyrics;	///<가사
	string ID; ///<고유 키
	wstring path; ///<경로

public:
	/**
	* 기본 생성자
	*/
	MusicType();

	/**
	* 기본 소멸자
	*/
	~MusicType();


	//------------------------------
	//------------------------------Getter 함수들
	//------------------------------

	/**
	*	@brief	곡명을 반환한다.
	*	@pre	곡명 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	곡명.
	*/
	string GetName() const;

	/**
	*	@brief	가수의 이름을 반환한다.
	*	@pre	가수 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	가수의 이름.
	*/
	string GetArtist() const;

	/**
	*	@brief	앨범을 반환한다.
	*	@pre	앨범 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	앨범.
	*/
	string GetAlbum() const;

	/**
	*	@brief	장르를 반환한다.
	*	@pre	장르 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	장르.
	*/
	string GetGenre() const;

	/**
	*	@brief	가사를 반환한다.
	*	@pre	가사 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	가사.
	*/
	string GetLyrics() const;

	/**
	*	@brief	곡의 고유 키를 반환한다.
	*	@pre	곡번호 변수가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	고유 키.
	*/
	string GetID() const;

	/**
	*	@brief	작곡가를 반환한다.
	*	@pre	composer가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	작곡가.
	*/
	string GetComposer() const;

	/**
	*	@brief	작사가를 반환한다.
	*	@pre	writer가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	작사가.
	*/
	string GetWriter() const;

	/**
	*	@brief	음반 회사명을 반환한다.
	*	@pre	record가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	음반 회사명.
	*/
	string GetRecord() const;

	/**
	*	@brief	음악의 파일 경로를 반환한다.
	*	@pre	path가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	파일 경로.
	*/
	wstring GetPath() const;

	/**
	*	@brief	발표년월일을 반환한다.
	*	@pre	date가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	발표년월일.
	*/
	unsigned int GetDate() const;

	/**
	*	@brief	곡의 길이를 반환한다.
	*	@pre	length가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	곡의 길이.
	*/
	unsigned int GetLength() const;

	/**
	*	@brief	재생 횟수를 반환한다.
	*	@pre	time이 할당되어 있어야 한다.
	*	@post	없음.
	*	@return	재생 횟수.
	*/
	unsigned int GetPlayedTime() const;

	/**
	*	@brief	악보를 반환한다.
	*	@pre	note가 할당되어 있어야 한다.
	*	@post	없음.
	*	@return 악보.
	*/
	string GetNote() const;

	//------------------------------
	//------------------------------Setter 함수들
	//------------------------------

	/**
	*	@brief	곡명을 설정한다.
	*	@pre	없음.
	*	@post	곡명을 할당한다.
	*	@param	musicName	설정할 곡의 이름.
	*/
	void SetName(const string& musicName);

	/**
	*	@brief	가수의 이름을 설정한다.
	*	@pre	없음.
	*	@post	가수의 이름을 할당한다.
	*	@param	artistName	설정할 가수의 이름.
	*/
	void SetArtist(const string& artistName);

	/**
	*	@brief	앨범을 설정한다.
	*	@pre	없음.
	*	@post	앨범을 할당한다.
	*	@param	album	설정할 앨범.
	*/
	void SetAlbum(const string& album);

	/**
	*	@brief	곡의 장르를 설정한다.
	*	@pre	없음.
	*	@post	곡의 장르를 할당한다.
	*	@param	genre	설정할 장르.
	*/
	void SetGenre(const string& genre);

	/**
	*	@brief	곡의 가사를 설정한다.
	*	@pre	없음.
	*	@post	곡의 가사를 할당한다.
	*	@param	lyrics	설정할 가사.
	*/
	void SetLyrics(const string& lyrics);

	/**
	*	@brief	곡의 고유 키를 설정한다.
	*	@pre	없음.
	*	@post	곡의 고유 키를 할당한다.
	*	@param	id	설정할 고유 키.
	*/
	void SetID(const string& id);

	/**
	*	@brief	작곡가를 설정한다.
	*	@pre	없음.
	*	@post	composer 변수가 할당된다.
	*	@param	composer	설정할 작곡가의 이름.
	*/
	void SetComposer(const string& composer);

	/**
	*	@brief	작사가를 설정한다.
	*	@pre	없음.
	*	@post	writer 변수가 할당된다.
	*	@param	writer	설정할 작사가의 이름.
	*/
	void SetWriter(const string& writer);

	/**
	*	@brief	음반 회사를 설정한다.
	*	@pre	없음.
	*	@post	record 변수가 할당된다.
	*	@param	record	설정할 음반 회사명
	*/
	void SetRecord(const string& record);

	/**
	*	@brief	발표일을 설정한다.
	*	@pre	없음.
	*	@post	date 변수가 할당된다.
	*	@param	date	설정할 발표년월일 (YYYYMMDD 형식. ex: 20180924 )
	*/
	void SetDate(unsigned int date);

	/**
	*	@brief	곡의 길이를 설정한다.
	*	@pre	없음.
	*	@post	length 변수가 할당된다.
	*	@param	length	설정할 곡의 길이.
	*/
	void SetLength(unsigned int length);

	/**
	*	@brief	재생 횟수를 설정한다.
	*	@pre	없음.
	*	@post	time 변수가 할당된다.
	*	@param	time	설정할 곡을 재생한 횟수.
	*/
	void SetPlayedTime(unsigned int time);

	/**
	*	@brief	악보를 설정한다.
	*	@pre	없음.
	*	@post	note 변수가 할당된다.
	*	@param	note	설정할 곡의 악보.
	*/
	void SetNote(const string& note);

	/**
	*	@brief	경로를 설정한다.
	*	@pre	없음.
	*	@post	path 변수가 할당된다.
	*	@param	path	설정할 파일 경로.
	*/
	void SetPath(const wstring& path);

	//------------------------------
	//------------------------------기타 함수들
	//------------------------------

	/**
	*	@brief	곡명을 콘솔에 출력한다.
	*	@pre	곡명이 할당되어 있어야 한다.
	*	@post	곡명이 콘솔에 출력된다.
	*/
	void DisplayNameOnScreen();

	/**
	*	@brief	가수의 이름을 콘솔에 출력한다.
	*	@pre	가수의 이름이 할당되어 있어야 한다.
	*	@post	가수의 이름이 콘솔에 출력된다.
	*/
	void DisplayArtistOnScreen();

	/**
	*	@brief	앨범 이름을 콘솔에 출력한다.
	*	@pre	앨범이 할당되어 있어야 한다.
	*	@post	앨범 이름이 콘솔에 출력된다.
	*/
	void DisplayAlbumOnScreen();

	/**
	*	@brief	장르를 콘솔에 출력한다.
	*	@pre	장르가 할당되어 있어야 한다.
	*	@post	장르가 콘솔에 출력된다.
	*/
	void DisplayGenreOnScreen();

	/**
	*	@brief	가사를 콘솔에 출력한다.
	*	@pre	가사가 할당되어 있어야 한다.
	*	@post	가사가 콘솔에 출력된다.
	*/
	void DisplayLyricsOnScreen();

	/**
	*	@brief	곡의 고유 키를 콘솔에 출력한다.
	*	@pre	곡의 고유 키가 할당되어 있어야 한다.
	*	@post	곡의 고유 키가 콘솔에 출력된다.
	*/
	void DisplayIDOnScreen();

	/**
	*	@brief	작곡가의 이름을 콘솔에 출력한다.
	*	@pre	composer 변수가 할당되어 있어야 한다.
	*	@post	작곡가의 이름이 콘솔에 출력된다.
	*/
	void DisplayComposerOnScreen();

	/**
	*	@brief	작사가의 이름을 콘솔에 출력한다.
	*	@pre	writer 변수가 할당되어 있어야 한다.
	*	@post	작사가의 이름이 콘솔에 출력된다.
	*/
	void DisplayWriterOnScreen();

	/**
	*	@brief	음반 회사의 이름을 콘솔에 출력한다.
	*	@pre	record가 할당되어 있어야 한다.
	*	@post	음악이 나온 음반의 회사 이름이 콘솔에 출력된다.
	*/
	void DisplayRecordOnScreen();

	/**
	*	@brief	음악의 발표일을 콘솔에 출력한다.
	*	@pre	day 변수가 할당되어 있어야 한다.
	*	@post	음악의 발표년도, 월, 일이 콘솔에 출력된다.
	*/
	void DisplayDateOnScreen();

	/**
	*	@brief	곡의 길이를 콘솔에 출력한다.
	*	@pre	length 변수가 할당되어 있어야 한다.
	*	@post	곡의 길이가 콘솔에 출력된다.
	*/
	void DisplayLengthOnScreen();

	/**
	*	@brief	곡의 재생횟수를 콘솔에 출력한다.
	*	@pre	time 변수가 할당되어 있어야 한다.
	*	@post	곡의 재생횟수가 콘솔에 출력된다.
	*/
	void DisplayPlayedTimeOnScreen();

	/**
	*	@brief	곡의 악보를 콘솔에 출력한다.
	*	@pre	note 변수가 할당되어 있어야 한다.
	*	@post	곡의 악보가 콘솔에 출력된다.
	*/
	void DisplayNoteOnScreen();

	/**
	*	@brief	음악의 모든 정보를 콘솔에 출력한다.
	*	@pre	정보를 나타내는 변수들이 할당되어 있어야 한다.
	*	@post	음악의 모든 정보가 콘솔에 출력된다.
	*/
	void DisplayAllOnScreen();



	/**
	*	@brief	키보드로부터 음악의 이름을 입력받는다.
	*	@pre	없음.
	*	@post	곡명을 할당한다.
	*/
	void SetNameFromKB();

	/**
	*	@brief	키보드로부터 가수의 이름을 입력받는다.
	*	@pre	없음.
	*	@post	가수의 이름을 할당한다.
	*/
	void SetArtistFromKB();

	/**
	*	@brief	키보드로부터 앨범을 입력받는다.
	*	@pre	없음.
	*	@post	앨범을 할당한다.
	*/
	void SetAlbumFromKB();

	/**
	*	@brief	키보드로부터 장르를 입력받는다.
	*	@pre	없음.
	*	@post	장르를 할당한다.
	*/
	void SetGenreFromKB();

	/**
	*	@brief	키보드로부터 가사를 입력받는다.
	*	@pre	없음.
	*	@post	가사를 할당한다.
	*/
	void SetLyricsFromKB();

	/**
	*	@brief	키보드로부터 곡의 고유 키를 입력받는다.
	*	@pre	없음.
	*	@post	곡의 고유 키를 할당한다.
	*/
	void SetIDFromKB();

	/**
	*	@brief	작곡가의 이름을 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 이름을 composer 변수에 할당한다.
	*/
	void SetComposerFromKB();

	/**
	*	@brief	작사가의 이름을 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 이름을 writer 변수에 할당한다.
	*/
	void SetWriterFromKB();

	/**
	*	@brief	키보드로부터 음반 회사명을 입력받는다.
	*	@pre	없음.
	*	@post	음반 회사명을 할당한다.
	*/
	void SetRecordFromKB();

	/**
	*	@brief	음악의 발표일을 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 날짜를 day 변수에 할당한다.
	*/
	void SetDateFromKB();

	/**
	*	@brief	곡의 길이를 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 길이를 length 변수에 할당한다.
	*/
	void SetLengthFromKB();

	/**
	*	@brief	재생 횟수를 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 횟수를 time 변수에 할당한다.
	*/
	void SetPlayedTimeFromKB();

	/**
	*	@brief	곡의 악보를 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 악보를 note 변수에 할당한다.
	*/
	void SetNoteFromKB();

	/**
	*	@brief	모든 변수를 키보드로부터 입력받는다.
	*	@pre	없음.
	*	@post	입력받은 데이터를 각각 변수에 할당한다.
	*/
	void SetAllFromKB();

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

	/**
	*	두 아이템 데이터를 비교한다.
	*	@brief	음악의 고유 키를 이용해 두 아이템의 순서를 비교한다.
	*	@pre	두 아이템이 초기화되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 음악 데이터.
	*	@return	현재 곡의 고유 키가 비교하는 키보다 작다면 LESS를 반환,
	*			현재 곡의 고유 키가 비교하는 키보다 크다면 GREATER를 반환,
	*			그렇지 않은 경우엔 (고유 키가 같다면) EQUAL을 반환.
	*/
	RelationType Compare(const MusicType &data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const MusicType &data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key보다 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const MusicType &data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key와 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const MusicType &data) const;

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 다른지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터의 primary key가 비교하는 primary key와 다르면 true, 아니면 false를 반환한다.
	*/
	bool operator!=(const MusicType &data) const;

	/**
	*	@brief	현재 데이터에 대상 데이터의 값을 복사하여 대입한다.
	*	@pre	대상 데이터의 변수가 할당되어 있어야 한다.
	*	@post	대상 데이터의 변수 값이 현재 데이터에 복사된다.
	*	@param	data	복사할 대상 데이터
	*	@return	복사 대입한 현재 데이터를 반환한다.
	*/
	MusicType& operator=(const MusicType &data);

	/**
	*	@brief	곡의 파일 경로로부터 ID3 태그를 읽어와 정보를 저장한다.
	*	@pre	올바른 파일 경로가 지정되어 있어야 한다.
	*	@post	ID3 태그로부터 읽은 정보를 저장한다.
	*	@return	성공하면 1, 그렇지 않으면 0을 반환한다.
	*/
	int ReadDataFromID3();

	/**
	*	@brief	SimpleMusicType으로 형변환하는 연산자.
	*	@return	형변환된 SimpleMusicType 객체를 반환한다.
	*/
	operator SimpleMusicType()
	{
		SimpleMusicType simple;
		simple.SetID(ID);
		simple.SetLength(length);
		simple.SetName(name);
		simple.SetPlayedTime(time);

		return simple;
	}

	explicit 
};
#endif