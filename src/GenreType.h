#pragma once
#ifndef __GENRETYPE__
#define __GENRETYPE__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"

/**
*	�帣 ������ �����ϴ� Ŭ����
*	@author	�輺��
*	@date	2018.10.02
*/
class GenreType
{
private:
	string genreName; ///<�帣��
	DoublyLinkedList<SimpleMusicType> list; ///<���� ������ ��� ����Ʈ

public:
	/**
	* �⺻ ������
	*/
	GenreType();

	/**
	* ���� ������
	*/
	GenreType(const GenreType& data);

	/**
	* �⺻ �Ҹ���
	*/
	~GenreType();

	/**
	*	@brief	�帣�� ��ȯ�Ѵ�.
	*	@pre	�帣 ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	�帣.
	*/
	string GetGenre() const;

	/**
	*	@brief	�帣���� �����Ѵ�.
	*	@pre	����.
	*	@post	�帣���� �Ҵ��Ѵ�.
	*	@param	genre	������ �帣��.
	*/
	void SetGenre(const string& genre);

	/**
	*	@brief	���� ������ �帣�� ���� ����Ʈ�� �߰��Ѵ�.
	*	@pre	parameter�� ���� ������ �ʿ��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	parameter�� ���� ������ ����Ʈ�� �߰��ȴ�.
	*	@param	music	�߰��� ���� ����.
	*	@return �߰��� �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Add(const SimpleMusicType& music);

	/**
	*	@brief	���� ������ �帣�� ���� ����Ʈ���� �����Ѵ�.
	*	@pre	����Ʈ�� ���� ������ �����ϰ�, parameter�� ���� ������ �ʿ��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	parameter�� ���� ������ ����Ʈ���� ���ŵȴ�.
	*	@param	music	������ ���� ����.
	*	@return ���ſ� �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Replace(const SimpleMusicType& music);

	/**
	*	@brief	���� ������ �帣�� ���� ����Ʈ���� �����Ѵ�.
	*	@pre	����Ʈ�� ���� ������ �����ϰ�, parameter�� ���� ������ �ʿ��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	parameter�� ������ ��ġ�ϴ� ���� ������ ����Ʈ���� �����ȴ�.
	*	@param	music	������ ���� ����.
	*	@return ������ �����ϸ� 1, �����ϸ� 0�� ��ȯ�Ѵ�.
	*/
	int Delete(const SimpleMusicType& music);

	/**
	*	@brief	parameter�� ��ġ�ϴ� ���� ������ parameter�� �����Ѵ�.
	*	@pre	����Ʈ�� ���� ������ �����ϰ�, parameter�� ���� ������ �ʿ��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	parameter�� ã�� ���� ������ �Ҵ�ȴ�.
	*	@param	music	�˻��� ���� ����.
	*	@return	ã���� 1, ã�� ���ϸ� 0�� �����Ѵ�.
	*/
	int Get(SimpleMusicType& music);

	/**
	*	@brief	�帣�� �ش��ϴ� ������ ���� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	������ ��.
	*/
	int GetMusicNum() const;

	/**
	*	@brief	�帣�� ���� ����Ʈ�� ��ȯ�Ѵ�.
	*	@pre	����.
	*	@post	����.
	*	@return	�帣�� ���� ����Ʈ.
	*/
	DoublyLinkedList<SimpleMusicType>& GetList();
	
	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const GenreType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const GenreType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key�� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const GenreType& data) const;
	
	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	GenreType& operator=(const GenreType& data);
};

#endif