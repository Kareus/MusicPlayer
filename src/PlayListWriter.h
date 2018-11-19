#pragma once
#ifndef __PLAYLIST_WRITER__
#define __PLAYLIST_WRITER__

#include <fstream>
#include <string>
#include <locale>
#include <codecvt>

using namespace std;

/**
* �������� wpl ���Ϸ� ����ϴ� Ŭ����. wpl ������ utf-8�� �ۼ��Ǿ�� �ϹǷ�, wide char (wstring, wofstream)�� �ۼ��Ѵ�.
* ���� zpl�� wpl�� ���� ������ �����Ͽ� zpl ���ϵ� �ۼ��� �� �ִ�.
* @date 2018.10.27
* @author �輺��
*/

class WPLWriter
{
private:
	wofstream m_outFile; ///<������ �ۼ��ϴ� ��Ʈ��
	wstring head; ///<wpl�� meta data�� �� ���ڿ�
	wstring body; ///<wpl�� ���� ��Ͽ� �� ���ڿ�
	wstring title; ///<������ Ÿ��Ʋ
	int extFlag; ///<������ ��ȿ�� Ȯ���ڸ� ���������� ���� flag

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
* �������� m3u8 ���Ϸ� ����ϴ� Ŭ����. m3u ���ϱ����� ������, utf-8 ���ڵ��� ����ϱ� ������ m3u8 ���Ϸ� ����Ѵ�.
* @data 2018.11.19
* @author �輺��
*/

class MusicType; ///<������ ������ �޾ƿ;� �ϹǷ� Ŭ������ ����Ѵ�.

class M3U8Writer
{
private:
	wofstream m_outFile; ///<������ �ۼ��ϴ� ��Ʈ��
	wstring body; ///< ���� ��Ͽ� �� ���ڿ�
	wstring title; ///<������ Ÿ��Ʋ

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