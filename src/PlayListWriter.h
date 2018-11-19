#pragma once
#ifndef __PLAYLIST_WRITER__
#define __PLAYLIST_WRITER__

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

/**
* 재생목록을 wpl 파일로 출력하는 클래스. wpl 파일은 utf-8로 작성되어야 하므로, wide char (wstring, wofstream)로 작성한다.
* 또한 zpl이 wpl과 파일 구조가 유사하여 zpl 파일도 작성할 수 있다.
* @date 2018.10.27
* @author 김성주
*/

class WPLWriter
{
private:
	wofstream m_outFile; ///<파일을 작성하는 스트림
	wstring head; ///<wpl의 meta data에 들어갈 문자열
	wstring body; ///<wpl의 음악 목록에 들어갈 문자열
	wstring title; ///<재생목록 타이틀
	int extFlag; ///<파일이 유효한 확장자를 가지는지에 대한 flag

public:
	WPLWriter();

	WPLWriter(const wstring& filepath);

	~WPLWriter();

	bool open(const wstring& filepath);

	void close();

	void addFile(const wstring& mediaPath);

	void addMeta(const wstring& name, const wstring& data);

	void setTitle(const wstring& title);

	wstring getTitle();

	void resetHead();

	void resetBody();
};

/**
* 재생목록을 m3u8 파일로 출력하는 클래스. m3u 파일구조를 따르며, utf-8 인코딩을 명시하기 때문에 m3u8 파일로 출력한다.
* @data 2018.11.19
* @author 김성주
*/

class MusicType; ///<음악의 정보를 받아와야 하므로 클래스를 언급한다.

class M3U8Writer
{
private:
	wofstream m_outFile; ///<파일을 작성하는 스트림
	wstring body; ///< 음악 목록에 들어갈 문자열
	wstring title; ///<재생목록 타이틀

public:
	M3U8Writer();

	M3U8Writer(const wstring& filepath);

	~M3U8Writer();

	bool open(const wstring& filepath);

	void close();

	void addMedia(const MusicType& music);

	void addMedia(unsigned int length, const wstring& name, const wstring& mediapath);

	wstring getTitle();

	void setTitle(const wstring& title);

	void resetBody();
};
#endif