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
	string artist; ///<����
	string ID; ///<���� Ű
	DoublyLinkedList<SimpleMusicType> list; ///<���ϰ� ����Ʈ

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
	*	@brief	�ٹ����� �����Ѵ�.
	*	@pre	����.
	*	@post	album ������ �Ҵ��Ѵ�.
	*	@param	name	������ �ٹ��� �̸�.
	*/
	void SetAlbumName(const string& name);

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