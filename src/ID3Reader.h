#pragma once
#ifndef __ID3_READER__
#define __ID3_READER__

#include <fstream>
#include <string>
#include <stdexcept>
#include "AVLTree.h"
#include "GlobalFunctions.h"

/**
* id3 정보를 읽다가 발생한 문제에 대한 예외 처리 class
*/
class id3_error : public std::exception
{
private:
	std::string msg; ///<메시지

public:
	/**
	*	@brief	기본 생성자
	*	@param	message	멧지
	*/
	id3_error(const std::string& message) : msg(message) {};

	/**
	*	@brief	메시지 반환 함수
	*	@return	메시지
	*/
	const char* what() const noexcept //이 함수 호출에는 예외 처리가 없도록 함
	{
		return msg.c_str();
	}
};

/**
* id3 frame 정보 저장을 위한 클래스 구조.
*/
class ID3Frame
{
private:
	std::string frame; ///<프레임
	std::wstring content; ///<정보

public:

	/**
	*	@brief	기본 생성자
	*	@param	Frame	프레임
	*	@param	Content	정보
	*/
	ID3Frame(const std::string& Frame = "", const std::wstring& Content = L"") : frame(Frame), content(Content) {}

	/**
	*	@brief	정보를 설정하는 함수
	*	@param	content	설정할 정보
	*/
	void setContent(const std::wstring& content) { this->content = content; }

	/**
	*	@brief	정보를 반환하는 함수
	*	@return	정보
	*/
	std::wstring getContent() const { return content; }

	/**
	*	@brief	프레임 태그를 반환하는 함수
	*	@return	프레임
	*/
	std::string getFrame() const { return frame; }

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 작은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 작으면 true, 아니면 false를 반환한다.
	*/
	bool operator<(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) < 0; }

	/**
	*	@brief	현재 데이터가 비교하는 데이터보다 큰지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터가 더 크면 true, 아니면 false를 반환한다.
	*/
	bool operator>(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) > 0; }

	/**
	*	@brief	현재 데이터가 비교하는 데이터와 같은지 반환한다.
	*	@pre	현재 데이터와 비교하는 데이터의 primary key가 할당되어 있어야 한다.
	*	@post	없음.
	*	@param	data	비교할 데이터
	*	@return	현재 데이터와 비교하는 데이터가 같으면 true, 아니면 false를 반환한다.
	*/
	bool operator==(const ID3Frame& data) const { return !strcmp(frame.c_str(), data.getFrame().c_str()); }
};

/**
* mp3파일에서 ID3 Tag에서 정보를 읽어오는 클래스. 지원 버전 : v1/1.1, v2.3/2.4
* 참고: 1] http://id3.org/id3v2.3.0  2] http://id3.org/id3v2.4.0-structure 3] http://id3.org/id3v2.4.0-frames 4] https://en.wikipedia.org/wiki/ID3
* @date 2018.11.04
* @author 김성주
*/

class ID3Reader
{
private:
	std::ifstream m_inFile; ///<태그를 읽어오는 파일 스트림
	char minorVersion; ///<ID3의 마이너 버전
	char majorVersion; ///<ID3의 메인 버전
	char revision; ///<ID3의 세부 버전

	char title[30]; ///<id3v1의 타이틀
	char artist[30]; ///<id3v1의 아티스트
	char album[30]; ///<id3v1의 앨범
	char year[4]; ///<id3v1의 날짜
	char comment[30]; ///<id3v1의 비고
	char track; ///<id3v1의 트랙 번호
	char genre; ///<idev1의 장르 번호

	int tagSize; ///<태그 정보의 크기
	AVLTree<ID3Frame> frames; ///<태그 프레임을 저장하는 리스트

	/**
	*	@brief	태그 정보의 synchronization을 디코딩하는 함수
	*	@param	byte	synchronize된 데이터
	*	@return	디코딩된 정수
	*/
	int decodeSync(char* byte);

	/**
	*	@brief	각 프레임에 대해 처리할 경우의 수를 가져오는 함수
	'*	@param	프레임 태그
	*	@return	처리할 경우의 수
	*/
	int getFrameCase(const std::string& id);

	/**
	*	@brief	해당 프레임 태그를 읽어올 수 있는지 반환하는 함수
	*	@param	id	프레임 태그
	*	@return	읽을 수 있으면 true, 아니면 false를 반환한다.
	*/
	bool canRead(char* id);

public:

	/**
	*	기본 생성자
	*/
	ID3Reader();

	/**
	*	@brief	파일 경로를 받아 바로 읽어들이는 생성자
	*	@param	filepath	파일 경로
	*/
	ID3Reader(const std::wstring& filepath);

	/**
	*	기본 소멸자
	*/
	~ID3Reader();

	/**
	*	@brief	해당 경로에서 ID3 태그를 읽어오는 함수
	*	@param	filepath	파일 경로
	*	@return	성공하면 파일 스트림을 닫고 1, 아니면 0을 반환한다.
	*/
	bool read(const std::wstring& filepath);

	/**
	*	@brief	파일 스트림을 종료하는 함수
	*/
	void close();

	/**
	*	@brief	ID3 태그의 메인 버전을 반환한다.
	*	@return	메인 버전
	*/
	int getMajorVersion();

	/**
	*	@brief	ID3 태그의 마이너 버전을 반환한다.
	*	@return	마이너 버전
	*/
	int getMinorVersion();

	/**
	*	@brief	ID3 태그의 세부 버전을 반환한다.
	*	@return	세부 버전
	*/
	int getRevision();

	/**
	*	@brief	ID3 태그에 저장된 음악 이름을 반환한다.
	*	@return	음악 이름
	*/
	std::wstring getTitle();

	/**
	*	@brief	ID3 태그에 저장된 아티스트를 반환한다.
	*	@return	아티스트
	*/
	std::wstring getArtist();

	/**
	*	@brief	ID3 태그에 저장된 앨범을 반환한다.
	*	@return	앨범
	*/
	std::wstring getAlbum();

	/**
	*	@brief	ID3 태그에 저장된 날짜(혹은 연도)를 반환한다.
	*	@return	날짜
	*/
	std::wstring getYear();

	/**
	*	@brief	ID3 태그에 저장된 날짜(혹은 연도)를 숫자로 반환한다.
	*	@return	날짜
	*/
	int getYearAsInteger();

	/**
	*	@brief	ID3 태그에 저장된 비고를 반환한다.
	*	@return	비고
	*/
	std::wstring getComment();

	/**
	*	@brief	ID3 태그에 저장된 트랙 숫자를 반환한다.
	*	@return	트랙 숫자
	*/
	int getTrackNum();

	/**
	*	@brief	ID3 태그에 저장된 장르를 번호에 해당하는 장르 테이블의 값으로 반환한다.
	*	@return	장르
	*/
	std::wstring getGenre();

	/**
	*	@brief	ID3 태그에서 해당 태그의 값을 반환한다.
	*	@param	frameName	찾는 태그의 이름
	*	@return	찾으면 그 값을, 아니면 빈 문자열을 반환한다.
	*/
	std::wstring getFrame(const std::string& frameName);
};

#endif