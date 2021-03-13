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
* @author Kareus
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
	/**
	*	�⺻ ������
	*/
	WPLWriter();

	/**
	*	@brief	���� ��θ� �ٷ� ���� �����ϴ� ������
	*	@param	filepath	���� ���
	*/
	WPLWriter(const wstring& filepath);

	/**
	*	�⺻ �Ҹ���
	*/
	~WPLWriter();

	/**
	*	@brief	���� ����� ��Ʈ���� ���� �Լ�
	*	@param	filepath	���� ���
	*	@return	��Ʈ���� ���� �� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool open(const wstring& filepath);

	/**
	*	@brief	��Ʈ���� �ݰ� ������ �����ϴ� �Լ�
	*/
	void close();

	/**
	*	@brief	�̵�� ������ �����Ͽ� �߰��ϴ� �Լ�
	*	@param	mediaPath	�̵�� ���� ���
	*/
	void addFile(const wstring& mediaPath);

	/**
	*	@brief	��Ÿ�����͸� �����Ͽ� �߰��ϴ� �Լ�
	*	@param	name	��Ÿ������ �̸�
	*	@param	data	��Ÿ������ ����
	*/
	void addMeta(const wstring& name, const wstring& data);

	/**
	*	@brief	�������� �̸��� �����ϴ� �Լ�
	*	@param	title	�̸�
	*/
	void setTitle(const wstring& title);

	/**
	*	@brief	�������� �̸��� ��ȯ�ϴ� �Լ�
	*	@return	�̸�
	*/
	wstring getTitle();

	/**
	*	@brief	��Ÿ������ ������ �ʱ�ȭ�ϴ� �Լ�
	*/
	void resetHead();

	/**
	*	@brief	�̵�� ��� ������ �ʱ�ȭ�ϴ� �Լ�
	*/
	void resetBody();
};

/**
* �������� m3u8 ���Ϸ� ����ϴ� Ŭ����. m3u ���ϱ����� ������, utf-8 ���ڵ��� ����ϱ� ������ m3u8 ���Ϸ� ����Ѵ�.
* @data 2018.11.19
* @author Kareus
*/

class MusicType; ///<������ ������ �޾ƿ;� �ϹǷ� Ŭ������ ����Ѵ�.

class M3U8Writer
{
private:
	wofstream m_outFile; ///<������ �ۼ��ϴ� ��Ʈ��
	wstring body; ///< ���� ��Ͽ� �� ���ڿ�

public:
	/**
	*	�⺻ ������
	*/
	M3U8Writer();

	/**
	*	@brief	���� ��θ� �ٷ� ���� �����ϴ� ������
	*	@param	filepath	���� ���
	*/
	M3U8Writer(const wstring& filepath);

	/**
	*	�⺻ �Ҹ���
	*/
	~M3U8Writer();

	/**
	*	@brief	���� ����� ��Ʈ���� ���� �Լ�
	*	@param	filepath	���� ���
	*	@return	��Ʈ���� ���� �� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool open(const wstring& filepath);

	/**
	*	@brief	��Ʈ���� �ݰ� ������ �����ϴ� �Լ�
	*/
	void close();

	/**
	*	@brief	�ش� ���� �����ͷκ��� �̵�� ������ �����Ͽ� �߰��ϴ� �Լ�
	*	@param	music	���� ������
	*/
	void addMedia(const MusicType& music);

	/**
	*	@brief	�ش� �̵�� ������ �����Ͽ� �߰��ϴ� �Լ�
	*	@param	length	�̵���� ����
	*	@param	name	�̵�� �̸�
	*	@param	mediapath	�̵�� ���
	*/
	void addMedia(unsigned int length, const wstring& name, const wstring& mediapath);

	/**
	*	@brief	������ ������ �ʱ�ȭ�ϴ� �Լ�
	*/
	void resetBody();
};
#endif