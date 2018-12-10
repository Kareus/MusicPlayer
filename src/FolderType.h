#pragma once
#ifndef __FOLDER_TYPE__
#define __FOLDER_TYPE__

#include "SimpleMusicType.h"
#include "DoublyLinkedList.h"
#include <string>

/**
* ���� ���� ������ �����ϱ� ���� Ŭ����.
* @date 2018.11.26
* @author �輺��
*/
class FolderType
{
private:
	std::wstring dirPath; ///<directory path
	DoublyLinkedList<SimpleMusicType> list; ///<music list

public:

	/**
	*	�⺻ ������
	*/
	FolderType();

	/**
	*	���� ������
	*/
	FolderType(const FolderType& data);

	/**
	*	�⺻ �Ҹ���
	*/
	~FolderType();

	/**
	*	@brief	���� ��θ� ��ȯ�Ѵ�.
	*	@return	���� ���
	*/
	std::wstring GetPath() const;

	/**
	*	@brief	���� ��θ� �����Ѵ�.
	*	@param	path	���� ���
	*/
	void SetPath(const std::wstring& path);

	/**
	*	@brief	����Ʈ�� ������ �߰��Ѵ�.
	*	@param	data	�߰��� ���� ������
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int AddMusic(const SimpleMusicType& data);

	/**
	*	@brief	����Ʈ���� ������ �����Ѵ�.
	*	@param	data	������ ���� ������
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int DeleteMusic(const SimpleMusicType& data);

	/**
	*	@brief	����Ʈ���� ������ ��ü�Ѵ�.
	*	@param	data	��ü�� ���� ������
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int ReplaceMusic(const SimpleMusicType& data);

	/**
	*	@brief	����Ʈ���� ������ ã�´�.
	*	@param	data	�߰��� ���� ������
	*	@return	�����ϸ� 1, �ƴϸ� 0�� ��ȯ�Ѵ�.
	*/
	int GetMusic(SimpleMusicType& data);

	/**
	*	@brief	����Ʈ�� ���� ���� ��ȯ�Ѵ�.
	*	@return	���� ��
	*/
	int GetLength() const;

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	FolderType& operator=(const FolderType& data);

	/**
	*	@brief	���� ����Ʈ�� ������ iterator�� �����Ѵ�.
	*	@pre	����.
	*	@post	iterator ��ü�� �����ȴ�.
	*	@return	���� ����Ʈ�� ���� iterator ��ü.
	*/
	DoublyIterator<SimpleMusicType> GetIterator();

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const FolderType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ͱ� �� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const FolderType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �����Ϳ� ���ϴ� �����Ͱ� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const FolderType& data) const;
};
#endif