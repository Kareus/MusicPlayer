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
* @author Kareus
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
	/**
	*	기본 생성자
	*/
	WPLWriter();

	/**
	*	@brief	파일 경로를 바로 열어 생성하는 생성자
	*	@param	filepath	파일 경로
	*/
	WPLWriter(const wstring& filepath);

	/**
	*	기본 소멸자
	*/
	~WPLWriter();

	/**
	*	@brief	파일 경로의 스트림을 여는 함수
	*	@param	filepath	파일 경로
	*	@return	스트림을 여는 데 성공하면 true, 아니면 false를 반환한다.
	*/
	bool open(const wstring& filepath);

	/**
	*	@brief	스트림을 닫고 파일을 저장하는 함수
	*/
	void close();

	/**
	*	@brief	미디어 파일을 재생목록에 추가하는 함수
	*	@param	mediaPath	미디어 파일 경로
	*/
	void addFile(const wstring& mediaPath);

	/**
	*	@brief	메타데이터를 재생목록에 추가하는 함수
	*	@param	name	메타데이터 이름
	*	@param	data	메타데이터 정보
	*/
	void addMeta(const wstring& name, const wstring& data);

	/**
	*	@brief	재생목록의 이름을 설정하는 함수
	*	@param	title	이름
	*/
	void setTitle(const wstring& title);

	/**
	*	@brief	재생목록의 이름을 반환하는 함수
	*	@return	이름
	*/
	wstring getTitle();

	/**
	*	@brief	메타데이터 정보를 초기화하는 함수
	*/
	void resetHead();

	/**
	*	@brief	미디어 목록 정보를 초기화하는 함수
	*/
	void resetBody();
};

/**
* 재생목록을 m3u8 파일로 출력하는 클래스. m3u 파일구조를 따르며, utf-8 인코딩을 명시하기 때문에 m3u8 파일로 출력한다.
* @data 2018.11.19
* @author Kareus
*/

class MusicType; ///<음악의 정보를 받아와야 하므로 클래스를 언급한다.

class M3U8Writer
{
private:
	wofstream m_outFile; ///<파일을 작성하는 스트림
	wstring body; ///< 음악 목록에 들어갈 문자열

public:
	/**
	*	기본 생성자
	*/
	M3U8Writer();

	/**
	*	@brief	파일 경로를 바로 열어 생성하는 생성자
	*	@param	filepath	파일 경로
	*/
	M3U8Writer(const wstring& filepath);

	/**
	*	기본 소멸자
	*/
	~M3U8Writer();

	/**
	*	@brief	파일 경로의 스트림을 여는 함수
	*	@param	filepath	파일 경로
	*	@return	스트림을 여는 데 성공하면 true, 아니면 false를 반환한다.
	*/
	bool open(const wstring& filepath);

	/**
	*	@brief	스트림을 닫고 파일을 저장하는 함수
	*/
	void close();

	/**
	*	@brief	해당 음악 데이터로부터 미디어 정보를 재생목록에 추가하는 함수
	*	@param	music	음악 데이터
	*/
	void addMedia(const MusicType& music);

	/**
	*	@brief	해당 미디어 정보를 재생목록에 추가하는 함수
	*	@param	length	미디어의 길이
	*	@param	name	미디어 이름
	*	@param	mediapath	미디어 경로
	*/
	void addMedia(unsigned int length, const wstring& name, const wstring& mediapath);

	/**
	*	@brief	재생목록 내용을 초기화하는 함수
	*/
	void resetBody();
};
#endif