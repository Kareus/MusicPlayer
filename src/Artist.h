#pragma once
#ifndef __ARTIST__
#define __ARTIST__

#include "SimpleMusicType.h"

class Artist
{
private:
	string name; ///<�̸�
	DoublyLinkedList<SimpleMusicType> musicList; ///<���� ����Ʈ
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
	*	@brief	������ �۾��� ���� ������ ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	���� ����.
	*/
	unsigned int GetMusicNum() const;

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
	*	@brief	������ ���� Ű�� �����Ѵ�.
	*	@pre	����.
	*	@post	ID ������ �Ҵ��Ѵ�.
	*	@param	id	������ ������ ���� Ű.
	*/
	void SetID(const string& id);

	/**
	*	@brief	������ �߰��Ѵ�.
	*	@pre	����.
	*	@post	������ ����Ʈ�� �߰��ȴ�.
	*	@param	data	�߰��� ���� ������.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	������ �����Ѵ�.
	*	@pre	����.
	*	@post	�־��� �����Ϳ� ���� ���Ǹ��� ���� �ٹ��� ã�� �����Ѵ�.
	*	@param	data	������ ���� ������.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	�־��� �����Ͱ� �̸��� ���� ������ ã�� ��ü�Ѵ�.
	*	@pre	����.
	*	@post	���� �̸��� ���� �����Ͱ� ��ü�ȴ�.
	*	@param	data	��ü�� ���� ������.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

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
	DoublyIterator<SimpleMusicType>& GetIterator() const;
};
#endif