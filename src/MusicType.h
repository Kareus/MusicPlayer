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
*	�� ������ ���� ���踦 ��Ÿ����
*/
enum RelationType { LESS, GREATER, EQUAL };

/**
*	���� ������ �����ϴ� Ŭ����
*	@author	�輺��
*	@date	2018.09.23
*/

class MusicType
{
private:
	string name; ///<���
	string artist; ///<����
	string composer; ///<�۰
	string writer; ///<�ۻ簡
	unsigned int date; ///<��ǥ��
	string album; ///<�ٹ�
	string genre; ///<�帣
	unsigned int length; ///<��� ���� (second)
	unsigned int time; ///<��� Ƚ��
	string lyrics;	///<����
	string ID; ///<���� Ű
	wstring path; ///<���

public:
	/**
	* �⺻ ������
	*/
	MusicType();

	/**
	* �⺻ �Ҹ���
	*/
	~MusicType();

	/**
	* ���� ������
	*/
	MusicType(const MusicType& data);


	//------------------------------
	//------------------------------Getter �Լ���
	//------------------------------

	/**
	*	@brief	����� ��ȯ�Ѵ�.
	*	@pre	��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���.
	*/
	string GetName() const;

	/**
	*	@brief	������ �̸��� ��ȯ�Ѵ�.
	*	@pre	���� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	������ �̸�.
	*/
	string GetArtist() const;

	/**
	*	@brief	�ٹ��� ��ȯ�Ѵ�.
	*	@pre	�ٹ� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�ٹ�.
	*/
	string GetAlbum() const;

	/**
	*	@brief	�帣�� ��ȯ�Ѵ�.
	*	@pre	�帣 ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�帣.
	*/
	string GetGenre() const;

	/**
	*	@brief	���縦 ��ȯ�Ѵ�.
	*	@pre	���� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	����.
	*/
	string GetLyrics() const;

	/**
	*	@brief	���� ���� Ű�� ��ȯ�Ѵ�.
	*	@pre	���ȣ ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� Ű.
	*/
	string GetID() const;

	/**
	*	@brief	�۰�� ��ȯ�Ѵ�.
	*	@pre	composer�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�۰.
	*/
	string GetComposer() const;

	/**
	*	@brief	�ۻ簡�� ��ȯ�Ѵ�.
	*	@pre	writer�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�ۻ簡.
	*/
	string GetWriter() const;

	/**
	*	@brief	������ ���� ��θ� ��ȯ�Ѵ�.
	*	@pre	path�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� ���.
	*/
	wstring GetPath() const;

	/**
	*	@brief	��ǥ������� ��ȯ�Ѵ�.
	*	@pre	date�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	��ǥ�����.
	*/
	unsigned int GetDate() const;

	/**
	*	@brief	���� ���̸� ��ȯ�Ѵ�.
	*	@pre	length�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� ����.
	*/
	unsigned int GetLength() const;

	/**
	*	@brief	��� Ƚ���� ��ȯ�Ѵ�.
	*	@pre	time�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	��� Ƚ��.
	*/
	unsigned int GetPlayedTime() const;

	//------------------------------
	//------------------------------Setter �Լ���
	//------------------------------

	/**
	*	@brief	����� �����Ѵ�.
	*	@pre	����.
	*	@post	����� �Ҵ��Ѵ�.
	*	@param	musicName	������ ���� �̸�.
	*/
	void SetName(const string& musicName);

	/**
	*	@brief	������ �̸��� �����Ѵ�.
	*	@pre	����.
	*	@post	������ �̸��� �Ҵ��Ѵ�.
	*	@param	artistName	������ ������ �̸�.
	*/
	void SetArtist(const string& artistName);

	/**
	*	@brief	�ٹ��� �����Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� �Ҵ��Ѵ�.
	*	@param	album	������ �ٹ�.
	*/
	void SetAlbum(const string& album);

	/**
	*	@brief	���� �帣�� �����Ѵ�.
	*	@pre	����.
	*	@post	���� �帣�� �Ҵ��Ѵ�.
	*	@param	genre	������ �帣.
	*/
	void SetGenre(const string& genre);

	/**
	*	@brief	���� ���縦 �����Ѵ�.
	*	@pre	����.
	*	@post	���� ���縦 �Ҵ��Ѵ�.
	*	@param	lyrics	������ ����.
	*/
	void SetLyrics(const string& lyrics);

	/**
	*	@brief	���� ���� Ű�� �����Ѵ�.
	*	@pre	����.
	*	@post	���� ���� Ű�� �Ҵ��Ѵ�.
	*	@param	id	������ ���� Ű.
	*/
	void SetID(const string& id);

	/**
	*	@brief	�۰�� �����Ѵ�.
	*	@pre	����.
	*	@post	composer ������ �Ҵ�ȴ�.
	*	@param	composer	������ �۰�� �̸�.
	*/
	void SetComposer(const string& composer);

	/**
	*	@brief	�ۻ簡�� �����Ѵ�.
	*	@pre	����.
	*	@post	writer ������ �Ҵ�ȴ�.
	*	@param	writer	������ �ۻ簡�� �̸�.
	*/
	void SetWriter(const string& writer);

	/**
	*	@brief	��ǥ���� �����Ѵ�.
	*	@pre	����.
	*	@post	date ������ �Ҵ�ȴ�.
	*	@param	date	������ ��ǥ����� (YYYYMMDD ����. ex: 20180924 )
	*/
	void SetDate(unsigned int date);

	/**
	*	@brief	���� ���̸� �����Ѵ�.
	*	@pre	����.
	*	@post	length ������ �Ҵ�ȴ�.
	*	@param	length	������ ���� ����.
	*/
	void SetLength(unsigned int length);

	/**
	*	@brief	��� Ƚ���� �����Ѵ�.
	*	@pre	����.
	*	@post	time ������ �Ҵ�ȴ�.
	*	@param	time	������ ���� ����� Ƚ��.
	*/
	void SetPlayedTime(unsigned int time);

	/**
	*	@brief	��θ� �����Ѵ�.
	*	@pre	����.
	*	@post	path ������ �Ҵ�ȴ�.
	*	@param	path	������ ���� ���.
	*/
	void SetPath(const wstring& path);

	/**
	*	@brief	���Ϸκ��� ������ ������ �д´�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �Ѵ�.
	*	@post	������ ������ �Ҵ��Ѵ�.
	*	@param	fin	���� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	���Ͽ� ������ ������ �����Ѵ�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �ϸ�, ������ ����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�ش� ���Ͽ� ������ ������ ����ȴ�.
	*	@param	fout	������ �� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int WriteDataToFile(ofstream& fout);

	/**
	*	�� ������ �����͸� ���Ѵ�.
	*	@brief	������ ���� Ű�� �̿��� �� �������� ������ ���Ѵ�.
	*	@pre	�� �������� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ���� ������.
	*	@return	���� ���� ���� Ű�� ���ϴ� Ű���� �۴ٸ� LESS�� ��ȯ,
	*			���� ���� ���� Ű�� ���ϴ� Ű���� ũ�ٸ� GREATER�� ��ȯ,
	*			�׷��� ���� ��쿣 (���� Ű�� ���ٸ�) EQUAL�� ��ȯ.
	*/
	RelationType Compare(const MusicType &data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const MusicType &data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const MusicType &data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key�� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const MusicType &data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� �ٸ��� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key�� �ٸ��� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator!=(const MusicType &data) const;

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	MusicType& operator=(const MusicType &data);

	/**
	*	@brief	���� ���� ��ηκ��� ID3 �±׸� �о�� ������ �����Ѵ�.
	*	@pre	�ùٸ� ���� ��ΰ� �����Ǿ� �־�� �Ѵ�.
	*	@post	ID3 �±׷κ��� ���� ������ �����Ѵ�.
	*	@return	�����ϸ� 1, �׷��� ������ 0�� ��ȯ�Ѵ�.
	*/
	int ReadDataFromID3();

	/**
	*	@brief	SimpleMusicType���� ����ȯ�ϴ� ������.
	*	@return	����ȯ�� SimpleMusicType ��ü�� ��ȯ�Ѵ�.
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
};
#endif