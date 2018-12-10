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
	std::string msg; ///<�޽���

public:
	/**
	*	@brief	�⺻ ������
	*	@param	message	����
	*/
	id3_error(const std::string& message) : msg(message) {};

	/**
	*	@brief	�޽��� ��ȯ �Լ�
	*	@return	�޽���
	*/
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
	std::string frame; ///<������
	std::wstring content; ///<����

public:

	/**
	*	@brief	�⺻ ������
	*	@param	Frame	������
	*	@param	Content	����
	*/
	ID3Frame(const std::string& Frame = "", const std::wstring& Content = L"") : frame(Frame), content(Content) {}

	/**
	*	@brief	������ �����ϴ� �Լ�
	*	@param	content	������ ����
	*/
	void setContent(const std::wstring& content) { this->content = content; }

	/**
	*	@brief	������ ��ȯ�ϴ� �Լ�
	*	@return	����
	*/
	std::wstring getContent() const { return content; }

	/**
	*	@brief	������ �±׸� ��ȯ�ϴ� �Լ�
	*	@return	������
	*/
	std::string getFrame() const { return frame; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) < 0; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const ID3Frame& data) const { return strcmp(frame.c_str(), data.getFrame().c_str()) > 0; }

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ϳ� ���ϴ� �����Ͱ� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
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
	std::ifstream m_inFile; ///<�±׸� �о���� ���� ��Ʈ��
	char minorVersion; ///<ID3�� ���̳� ����
	char majorVersion; ///<ID3�� ���� ����
	char revision; ///<ID3�� ���� ����

	char title[30]; ///<id3v1�� Ÿ��Ʋ
	char artist[30]; ///<id3v1�� ��Ƽ��Ʈ
	char album[30]; ///<id3v1�� �ٹ�
	char year[4]; ///<id3v1�� ��¥
	char comment[30]; ///<id3v1�� ���
	char track; ///<id3v1�� Ʈ�� ��ȣ
	char genre; ///<idev1�� �帣 ��ȣ

	int tagSize; ///<�±� ������ ũ��
	AVLTree<ID3Frame> frames; ///<�±� �������� �����ϴ� ����Ʈ

	/**
	*	@brief	�±� ������ synchronization�� ���ڵ��ϴ� �Լ�
	*	@param	byte	synchronize�� ������
	*	@return	���ڵ��� ����
	*/
	int decodeSync(char* byte);

	/**
	*	@brief	�� �����ӿ� ���� ó���� ����� ���� �������� �Լ�
	'*	@param	������ �±�
	*	@return	ó���� ����� ��
	*/
	int getFrameCase(const std::string& id);

	/**
	*	@brief	�ش� ������ �±׸� �о�� �� �ִ��� ��ȯ�ϴ� �Լ�
	*	@param	id	������ �±�
	*	@return	���� �� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool canRead(char* id);

public:

	/**
	*	�⺻ ������
	*/
	ID3Reader();

	/**
	*	@brief	���� ��θ� �޾� �ٷ� �о���̴� ������
	*	@param	filepath	���� ���
	*/
	ID3Reader(const std::wstring& filepath);

	/**
	*	�⺻ �Ҹ���
	*/
	~ID3Reader();

	/**
	*	@brief	�ش� ��ο��� ID3 �±׸� �о���� �Լ�
	*	@param	filepath	���� ���
	*	@return	�����ϸ� ���� ��Ʈ���� �ݰ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	bool read(const std::wstring& filepath);

	/**
	*	@brief	���� ��Ʈ���� �����ϴ� �Լ�
	*/
	void close();

	/**
	*	@brief	ID3 �±��� ���� ������ ��ȯ�Ѵ�.
	*	@return	���� ����
	*/
	int getMajorVersion();

	/**
	*	@brief	ID3 �±��� ���̳� ������ ��ȯ�Ѵ�.
	*	@return	���̳� ����
	*/
	int getMinorVersion();

	/**
	*	@brief	ID3 �±��� ���� ������ ��ȯ�Ѵ�.
	*	@return	���� ����
	*/
	int getRevision();

	/**
	*	@brief	ID3 �±׿� ����� ���� �̸��� ��ȯ�Ѵ�.
	*	@return	���� �̸�
	*/
	std::wstring getTitle();

	/**
	*	@brief	ID3 �±׿� ����� ��Ƽ��Ʈ�� ��ȯ�Ѵ�.
	*	@return	��Ƽ��Ʈ
	*/
	std::wstring getArtist();

	/**
	*	@brief	ID3 �±׿� ����� �ٹ��� ��ȯ�Ѵ�.
	*	@return	�ٹ�
	*/
	std::wstring getAlbum();

	/**
	*	@brief	ID3 �±׿� ����� ��¥(Ȥ�� ����)�� ��ȯ�Ѵ�.
	*	@return	��¥
	*/
	std::wstring getYear();

	/**
	*	@brief	ID3 �±׿� ����� ��¥(Ȥ�� ����)�� ���ڷ� ��ȯ�Ѵ�.
	*	@return	��¥
	*/
	int getYearAsInteger();

	/**
	*	@brief	ID3 �±׿� ����� ��� ��ȯ�Ѵ�.
	*	@return	���
	*/
	std::wstring getComment();

	/**
	*	@brief	ID3 �±׿� ����� Ʈ�� ���ڸ� ��ȯ�Ѵ�.
	*	@return	Ʈ�� ����
	*/
	int getTrackNum();

	/**
	*	@brief	ID3 �±׿� ����� �帣�� ��ȣ�� �ش��ϴ� �帣 ���̺��� ������ ��ȯ�Ѵ�.
	*	@return	�帣
	*/
	std::wstring getGenre();

	/**
	*	@brief	ID3 �±׿��� �ش� �±��� ���� ��ȯ�Ѵ�.
	*	@param	frameName	ã�� �±��� �̸�
	*	@return	ã���� �� ����, �ƴϸ� �� ���ڿ��� ��ȯ�Ѵ�.
	*/
	std::wstring getFrame(const std::string& frameName);
};

#endif