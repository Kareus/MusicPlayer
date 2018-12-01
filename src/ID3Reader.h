#pragma once
#ifndef __ID3_READER__
#define __ID3_READER__

#include <fstream>
#include <string>
#include <stdexcept>
#include "AVLTree.h"
#include "GlobalFunctions.h"

/**
* id3 ������ �дٰ� �߻��� ������ ���� ���� ó�� class
*/
class id3_error : public std::exception
{
private:
	std::string msg;

public:
	id3_error(const std::string& message) : msg(message) {};

	const char* what() const noexcept //�� �Լ� ȣ�⿡�� ���� ó���� ������ ��
	{
		return msg.c_str();
	}
};

/**
* id3 frame ���� ������ ���� Ŭ���� ����.
*/
class ID3Frame
{
private:
	std::string frame;
	std::wstring content;

public:
	ID3Frame(const std::string& Frame = "", const std::wstring& Content = L"") : frame(Frame), content(Content) {}
	void setContent(const std::wstring& content) { this->content = content; }
	std::wstring getContent() const { return content; }
	std::string getFrame() const { return frame; }
	bool operator<(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) < 0; }
	bool operator>(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) > 0; }
	bool operator==(const ID3Frame& data) const { return !strcmp(frame.c_str(), data.getFrame().c_str()); }
};

/**
* mp3���Ͽ��� ID3 Tag���� ������ �о���� Ŭ����. ���� ���� : v1/1.1, v2.3/2.4
* ����: 1] http://id3.org/id3v2.3.0  2] http://id3.org/id3v2.4.0-structure 3] http://id3.org/id3v2.4.0-frames 4] https://en.wikipedia.org/wiki/ID3
* @date 2018.11.04
* @author �輺��
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
	AVLTree<ID3Frame> frames;

	int decodeSync(char* byte);
	int getFrameCase(const std::string& id);

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