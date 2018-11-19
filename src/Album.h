#pragma once
#ifndef __ALBUM__
#define __ALBUM__

#include "DoublyLinkedList.h"
#include "SimpleMusicType.h"
#include "MusicType.h"
#include <string>

using namespace std;

/**
*	�ٹ� ������ �����ϴ� Ŭ����
*	@author	�輺��
*	@date	2018.09.24
*/

class Album
{
private:
	string album; ///<�ٹ���
	string record; ///<���� ȸ��
	string artist; ///<����
	string ID; ///<���� Ű
	DoublyLinkedList<SimpleMusicType> list; ///<���ϰ� ����Ʈ
	unsigned int date; ///<��ǥ ��¥

public:

	/**
	* �⺻ ������
	*/
	Album();

	/**
	* ���� ������
	*/
	Album(const Album& data);

	/**
	* �⺻ �Ҹ���
	*/
	~Album();

	/**
	*	@brief	�ٹ����� ��ȯ�Ѵ�.
	*	@pre	album ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�ٹ���.
	*/
	string GetAlbumName() const;

	/**
	*	@brief	���� ȸ����� ��ȯ�Ѵ�.
	*	@pre	record ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� ȸ���.
	*/
	string GetRecord() const;

	/**
	*	@brief	������ �̸��� ��ȯ�Ѵ�.
	*	@pre	artist ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	������ �̸�.
	*/
	string GetArtist() const;

	/**
	*	@brief	���ϰ� ���� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	���ϰ� ��.
	*/
	unsigned int GetMusicNum() const;

	/**
	*	@brief	�ٹ��� ���� Ű�� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	���� Ű.
	*/
	string GetID() const;

	/**
	*	@brief	�ٹ��� ��ǥ ��¥�� ��ȯ�Ѵ�.
	*	@pre	date ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�ٹ��� ��ǥ ��¥.
	*/
	unsigned int GetDate() const;

	/**
	*	@brief	�ٹ����� �����Ѵ�.
	*	@pre	����.
	*	@post	album ������ �Ҵ��Ѵ�.
	*	@param	name	������ �ٹ��� �̸�.
	*/
	void SetAlbumName(const string& name);

	/**
	*	@brief	���� ȸ�� �̸��� �����Ѵ�.
	*	@pre	����.
	*	@post	record ������ �Ҵ��Ѵ�.
	*	@param	record	������ ���� ȸ���� �̸�.
	*/
	void SetRecord(const string& record);

	/**
	*	@brief	������ �̸��� �����Ѵ�.
	*	@pre	����.
	*	@post	artist ������ �Ҵ��Ѵ�.
	*	@param	artist	������ ������ �̸�.
	*/
	void SetArtist(const string& artist);

	/**
	*	@brief	�ٹ��� ���� Ű�� �����Ѵ�.
	*	@pre	����.
	*	@post	ID ������ �Ҵ��Ѵ�.
	*	@param	id	������ �ٹ��� ���� Ű.
	*/
	void SetID(const string& id);

	/**
	*	@brief	�ٹ��� ��ǥ ��¥�� �����Ѵ�.
	*	@pre	����.
	*	@post	date ������ �Ҵ��Ѵ�.
	*	@param	date	������ �ٹ��� ��ǥ ��¥.
	*/
	void SetDate(unsigned int date);

	/**
	*	@brief	���ϰ� ����Ʈ�� �ش� ���� �����͸� �߰��Ѵ�.
	*	@pre	data�� ���, primary key, ����ð� ������ ������ �־�� �Ѵ�.
	*	@post	data�� ���ϰ� ����Ʈ�� �߰��ȴ�.
	*	@param	data	�߰��� ���� ������
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	���ϰ� ����Ʈ���� �־��� ���� �������� Ű�� ������ Ű�� ���� ������ �����Ѵ�.
	*	@pre	data�� primary key�� ������ �־�� �Ѵ�.
	*	@post	ã�� ���� �����͸� �����Ѵ�.
	*	@param	data	������ ���� �������� Ű�� ���� ������.
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	���ϰ� ����Ʈ���� �־��� Ű�� ���� ���� �����͸� ã�� �����Ѵ�.
	*	@pre	����.
	*	@post	ã�� ���� �����͸� �����Ѵ�.
	*	@param	key	������ �������� primary key.
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ.
	*/
	int DeleteMusicFrom(const string& key);

	/**
	*	@brief	���ϰ� ����Ʈ���� �־��� ���� �������� Ű�� ������ Ű�� ���� ������ ã�� ��ü�Ѵ�.
	*	@pre	data�� ���, primary key, ����ð� ������ ������ �־�� �Ѵ�.
	*	@post	ã�� ���� �����͸� �־��� �����ͷ� ��ü�Ѵ�.
	*	@param	data	��ü�� ���� ������
	*	@return	�����ϸ� 1, �����ϸ� 0�� ��ȯ.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	�� ������ �����͸� ���Ѵ�.
	*	@brief	�� �������� ������ ���Ѵ�.
	*	@pre	�� �����ۿ��� �ʿ��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������.
	*	@return	���� �������� �� �����ۺ��� �۴ٸ� LESS�� ��ȯ,
	*			���� �������� �� �����ۺ��� ũ�ٸ� GREATER�� ��ȯ,
	*			�׷��� ���� ��쿣 (���ٸ�) EQUAL�� ��ȯ.
	*/
	RelationType Compare(const Album& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� album, record, artist�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�ٹ���, ������, ����ȸ����� ���ĺ������� �տ� �������� ������� �˻��Ͽ� �տ� ���� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const Album& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� album, record, artist�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�ٹ���, ������, ����ȸ����� ���ĺ������� �ڿ� �������� ������� �˻��Ͽ� �ڿ� ���� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const Album& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� album, record, artist�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�ٹ���, ������, ����ȸ����� ������� �˻��Ͽ� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const Album& data) const;

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	Album& operator=(const Album& data);

	/**
	*	@brief	Ű����κ��� �ٹ��� primary key�� �Է¹޴´�.
	*	@pre	����.
	*	@post	primary key�� �Ҵ��Ѵ�.
	*/
	void SetIDFromKB();

	/**
	*	@brief	Ű����κ��� �ٹ��� �̸��� �Է¹޴´�.
	*	@pre	����.
	*	@post	�ٹ��� �̸� �Ҵ��Ѵ�.
	*/
	void SetAlbumNameFromKB();

	/**
	*	@brief	Ű����κ��� �ٹ��� ��Ƽ��Ʈ�� �Է¹޴´�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� �Ҵ��Ѵ�.
	*/
	void SetArtistFromKB();

	/**
	*	@brief	Ű����κ��� �ٹ��� ���ڵ�(���� ȸ��)�� �Է¹޴´�.
	*	@pre	����.
	*	@post	���ڵ带 �Ҵ��Ѵ�.
	*/
	void SetRecordFromKB();

	/**
	*	@brief	Ű����κ��� �ٹ��� ��ǥ ��¥�� �Է¹޴´�.
	*	@pre	����.
	*	@post	��ǥ ��¥�� �Ҵ��Ѵ�.
	*/
	void SetDateFromKB();

	/**
	*	@brief	primary key�� �ֿܼ� ����Ѵ�.
	*	@pre	primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	primary key�� �ֿܼ� ��µȴ�.
	*/
	void DisplayIDOnScreen();

	/**
	*	@brief	�ٹ��� �̸��� �ֿܼ� ����Ѵ�.
	*	@pre	�ٹ� �̸��� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	�ٹ��� �̸��� �ֿܼ� ��µȴ�.
	*/
	void DisplayAlbumNameOnScreen();

	/**
	*	@brief	�ٹ��� ��Ƽ��Ʈ�� �ֿܼ� ����Ѵ�.
	*	@pre	��Ƽ��Ʈ�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	�ٹ��� ��Ƽ��Ʈ�� �ֿܼ� ��µȴ�.
	*/
	void DisplayArtistOnScreen();

	/**
	*	@brief	�ٹ��� ���� ȸ�縦 �ֿܼ� ����Ѵ�.
	*	@pre	���ڵ尡 �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	�ٹ��� ���� ȸ�簡 �ֿܼ� ��µȴ�.
	*/
	void DisplayRecordOnScreen();

	/**
	*	@brief	�ٹ��� ������ ��� �ֿܼ� ����Ѵ�.
	*	@pre	������ ������ ��� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	�ٹ��� ������ ��� �ֿܼ� ��µȴ�.
	*/
	void DisplayAllOnScreen();

	/**
	*	@brief	�ٹ��� ��ǥ ��¥�� �ֿܼ� ����Ѵ�.
	*	@pre	��ǥ ��¥�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	�ٹ��� ��ǥ ��¥�� �ֿܼ� ��µȴ�.
	*/
	void DisplayDateOnScreen();

	/**
	*	@brief	���Ϸκ��� �ٹ��� ������ �д´�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �Ѵ�.
	*	@post	�ٹ��� ������ �Ҵ��Ѵ�.
	*	@param	fin	���� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	���Ͽ� �ٹ��� ������ �����Ѵ�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �ϸ�, ������ ����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�ش� ���Ͽ� �ٹ��� ������ ����ȴ�.
	*	@param	fout	������ �� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int WriteDataToFile(ofstream& fout);

	/**
	*	@brief	�ٹ��� ���ϰ� ����Ʈ�� ������ iterator�� �����Ѵ�.
	*	@pre	����.
	*	@post	iterator ��ü�� �����ȴ�.
	*	@return	�ٹ��� ���ϰ� ����Ʈ�� ���� iterator ��ü.
	*/
	DoublyIterator<SimpleMusicType>& GetIterator() const;

};
#endif