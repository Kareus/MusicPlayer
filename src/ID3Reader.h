#pragma once
#ifndef __ID3_READER__
#define __ID3_READER__

#include <fstream>
#include <string>
#include <stdexcept>

#include <map>

/**
* id3 정보를 읽다가 발생한 문제에 대한 예외 처리 class
*/
class id3_error : public std::exception
{
private:
	std::string msg;

public:
	id3_error(const std::string& message) : msg(message) {};

	const char* what() const noexcept //이 함수 호출에는 예외 처리가 없도록 함
	{
		return msg.c_str();
	}
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
	std::ifstream m_inFile;
	char minorVersion;
	char majorVersion;
	char revision;

	char title[30];
	char artist[30];
	char album[30];
	char year[4];
	char comment[30];
	char track;
	char genre;

	int tagSize;
	std::map<std::string, std::wstring> frames;

	int decodeSync(char* byte);

public:
	ID3Reader();

	ID3Reader(const std::wstring& filepath);

	~ID3Reader();

	bool read(const std::wstring& filepath);

	void close();

	int getMajorVersion();

	int getMinorVersion();

	int getRevision();

	std::wstring getTitle();

	std::wstring getArtist();

	std::wstring getAlbum();

	std::wstring getYear();

	int getYearAsInteger();

	std::wstring getComment();

	int getTrackNum();

	std::wstring getGenre();

	std::wstring getFrame(const std::string& frameName);
};

#endif