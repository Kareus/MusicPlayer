#pragma once
#ifndef __ARTIST__
#define __ARTIST__

#include "Album.h"

class Artist
{
private:
	string name; ///<�̸�
	unsigned int birthDate; ///<�������
	DoublyLinkedList<Album> albumList; ///<�ٹ� ����Ʈ
	string ID; ///<���� Ű

public:
	/**
	* �⺻ ������
	*/
	Artist();

	/**
	* ���� ������
	*/
	Artist(const Artist& data);

	/**
	* �⺻ �Ҹ���
	*/
	~Artist();

	/**
	*	@brief	������ �̸��� ��ȯ�Ѵ�.
	*	@pre	name ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	������ �̸�.
	*/
	string GetName() const;

	/**
	*	@brief	������ ��������� ��ȯ�Ѵ�.
	*	@pre	birthDate ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�������.
	*/
	unsigned int GetBirthDate() const;

	/**
	*	@brief	������ �� �ٹ��� ������ ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	�ٹ��� ����.
	*/
	unsigned int GetAlbumNum() const;

	/**
	*	@brief	������ ���� Ű�� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	���� Ű.
	*/
	string GetID() const;

	/**
	*	@brief	������ �̸��� �����Ѵ�.
	*	@pre	����.
	*	@post	name ������ �Ҵ��Ѵ�.
	*	@param	name	������ ������ �̸�.
	*/
	void SetName(const string& name);

	/**
	*	@brief	��������� �����Ѵ�.
	*	@pre	����.
	*	@post	date ������ �Ҵ��Ѵ�.
	*	@param	date	������ ������ ������� (YYYYMMDD).
	*/
	void SetBirthDate(unsigned int date);

	/**
	*	@brief	Ű����κ��� ��Ƽ��Ʈ�� �̸��� �Է¹޴´�.
	*	@pre	����.
	*	@post	��Ƽ��Ʈ�� �̸��� �Ҵ��Ѵ�.
	*/
	void SetNameFromKB();

	/**
	*	@brief	Ű����κ��� ��Ƽ��Ʈ�� ��������� �Է¹޴´�.
	*	@pre	����.
	*	@post	��������� �Ҵ��Ѵ�.
	*/
	void SetBirthDateFromKB();

	/**
	*	@brief	������ ���� Ű�� �����Ѵ�.
	*	@pre	����.
	*	@post	ID ������ �Ҵ��Ѵ�.
	*	@param	id	������ ������ ���� Ű.
	*/
	void SetID(const string& id);

	/**
	*	@brief	�ٹ��� �߰��Ѵ�.
	*	@pre	����.
	*	@post	�ٹ��� ����Ʈ�� �߰��ȴ�.
	*	@param	data	�߰��� �ٹ� ������.
	*/
	int AddAlbum(const Album& data);

	/**
	*	@brief	�ٹ��� �����Ѵ�.
	*	@pre	����.
	*	@post	�־��� �����Ϳ� ���� �ٹ����� ���� �ٹ��� ã�� �����Ѵ�.
	*	@param	data	������ �ٹ� ������.
	*/
	int DeleteAlbum(const Album& data);

	/**
	*	@brief	�־��� �ٹ����� ���� �ٹ��� ã�� �����Ѵ�.
	*	@pre	����.
	*	@post	�־��� �ٹ����� ���� �ٹ��� ã�� �����Ѵ�.
	*	@param	albumName	������ �ٹ��� �̸�.
	*/
	int DeleteAlbumFrom(const string& albumName);

	/**
	*	@brief	�־��� �����Ͱ� �̸��� ���� �ٹ����� ã�� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	���� �ٹ����� �ٹ� �����Ͱ� ��ü�ȴ�.
	*	@param	data	��ü�� �ٹ� ������.
	*/
	int ReplaceAlbum(const Album& data);

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
	RelationType Compare(const Artist& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� name, birthDate�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�̸��� ���ĺ������� �տ� ������, ���ٸ� ��������� ������ �˻��Ͽ� �տ� ���� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const Artist& data) const;
	
	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� name, birthDate�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�̸��� ���ĺ������� �ڿ� ������, ���ٸ� ��������� �ʴ��� �˻��Ͽ� �տ� ���� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const Artist& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� name, birthDate�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	�̸��� ��������� �˻��Ͽ� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const Artist& data) const;

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	Artist& operator=(const Artist& data);

	/**
	*	@brief	Ű����κ��� ��Ƽ��Ʈ�� primary key�� �Է¹޴´�.
	*	@pre	����.
	*	@post	primary key�� �Ҵ��Ѵ�.
	*/
	void SetIDFromKB();

	/**
	*	@brief	primary key�� �ֿܼ� ����Ѵ�.
	*	@pre	primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	primary key�� �ֿܼ� ��µȴ�.
	*/
	void DisplayIDOnScreen();

	/**
	*	@brief	��Ƽ��Ʈ�� �̸��� �ֿܼ� ����Ѵ�.
	*	@pre	�̸��� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��Ƽ��Ʈ�� �̸��� �ֿܼ� ��µȴ�.
	*/
	void DisplayNameOnScreen();

	/**
	*	@brief	��Ƽ��Ʈ�� ��������� �ֿܼ� ����Ѵ�.
	*	@pre	��������� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��Ƽ��Ʈ�� ��������� �ֿܼ� ��µȴ�.
	*/
	void DisplayBirthDateOnScreen();

	/**
	*	@brief	��Ƽ��Ʈ�� ������ ��� �ֿܼ� ����Ѵ�.
	*	@pre	������ ������ ��� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��Ƽ��Ʈ�� ������ ��� �ֿܼ� ��µȴ�.
	*/
	void DisplayAllOnScreen();

	/**
	*	@brief	���Ϸκ��� ��Ƽ��Ʈ�� ������ �д´�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �Ѵ�.
	*	@post	��Ƽ��Ʈ�� ������ �Ҵ��Ѵ�.
	*	@param	fin	���� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int ReadDataFromFile(ifstream& fin);

	/**
	*	@brief	���Ͽ� ��Ƽ��Ʈ�� ������ �����Ѵ�.
	*	@pre	�ش� ������ ��Ʈ���� ���� �־�� �ϸ�, ������ ����Ʈ�� �ʱ�ȭ�Ǿ� �־�� �Ѵ�.
	*	@post	�ش� ���Ͽ� ��Ƽ��Ʈ�� ������ ����ȴ�.
	*	@param	fout	������ �� ������ ��Ʈ��.
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ.
	*/
	int WriteDataToFile(ofstream& fout);

	/**
	*	@brief	�ٹ� ����Ʈ�� ������ iterator�� �����Ѵ�.
	*	@pre	����.
	*	@post	iterator ��ü�� �����ȴ�.
	*	@return	�ٹ� ����Ʈ�� ���� iterator ��ü.
	*/
	DoublyIterator<Album>& GetIterator() const;
};
#endif