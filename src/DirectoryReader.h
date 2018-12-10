#pragma once
#ifndef __DIRECTORY_READER__
#define __DIRECTORY_READER__

#include "dirent.h" //���丮���� ���� ������ �о���� ���� ���
#include "Windows.h"
#include <string>
#include "DoublyLinkedList.h"

/**
*	@brief	���� ��ο��� ���� ����� �о���� Ŭ����
*	@author	�輺��
*/
class DirectoryReader
{
private:
	DoublyLinkedList<std::wstring> pathList; ///<���� ��� ����Ʈ

public:

	/**
	*	�⺻ ������
	*/
	DirectoryReader();

	/**
	*	�⺻ �Ҹ���
	*/
	~DirectoryReader();

	/**
	*	@brief	���� ���ڸ� ���� ���� ��θ� ���� �о���� �Լ�
	*/
	void openDirWithFileBox();

	/**
	*	@brief	��θ� �޾ƿ� ���� ����� �о���� �Լ�'
	*	@param	filepath	���� ���
	*	@return	�о���� �� �����ϸ� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool openDirectory(const std::wstring& filepath);

	/**
	*	@brief	��� ����Ʈ�� ������ iterator�� �����Ѵ�.
	*	@pre	����.
	*	@post	iterator ��ü�� �����ȴ�.
	*	@return	��� ����Ʈ�� ���� iterator ��ü.
	*/
	DoublyIterator<std::wstring> getIterator();
};
#endif