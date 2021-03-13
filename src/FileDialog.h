#pragma once
#ifndef __FILE_DIALOG__
#define __FILE_DIALOG__

#include <stdexcept>
#include <string>
#include <wchar.h>
#include <assert.h>
#include <windows.h>
#include <ShObjIdl.h>

//���� ���ڿ��� ��ȯ�� ���
enum DialogResult
{
	DIALOG_SUCCESS, DIALOG_CANCEL, DIALOG_ERROR
};

/**
*	@brief	���� ���ڿ��� �߻��� ������ ��ȯ�ϴ� ���� Ŭ����
*	@author	Kareus
*/

class filedialog_error : public std::exception
{
private:
	std::string mes; //�޽���
public:
	/**
	*	�⺻ ������
	*	@param	err	���� �޽���
	*/
	filedialog_error(const std::string& err) : mes(err) {};

	/**
	*	@brief	�޽��� ��ȯ �Լ�
	*	@return	�޽���
	*/
	virtual const char* what() const throw() { return mes.c_str(); }
};

/**
* ����/������ �����ϴ� ���̾�α� Ŭ����. COM Interface�� ����Ͽ���, github�� native-file-dialog�� ������. (�Ʒ��� ���� ����)
* native-file-dialog�� �״�� ������ �� strncpy���� safety ������ ������
* path�� �ѱ��� �� ���ƾ�� ���ڰ� ���ԵǾ� ���� �� ����� ǥ�õ��� �ʴ� ������ �־� �����Ͽ���.
* ���� ShellItem�� ���̾�α��� result�� �������� �������� ������ �߻��ص� success�� ��ȯ�ϴ� ������ ������.
* @date 2018.11.11
* @author Kareus
*/

class FileDialog
{
private:
	std::wstring defaultPath; ///<�⺻ ���

	/**
	*	@brief	�ش� ������ �ּ��� �޸𸮸� �����ϴ� �Լ�
	*	@param	ptr	������ �ּ�
	*/
	void free(void* ptr);

	/**
	*	@brief	�ش� ũ�� ��ŭ�� �޸� ������ ���� �Ҵ��Ͽ� ��ȯ�ϴ� �Լ�
	*	@param	bytes	�Ҵ��� �޸� ũ��
	*	@return	�Ҵ�� �޸��� �ּ�
	*/
	void* allocate(size_t bytes);

	/**
	*	@brief	�ش� ���ڰ� ���� ���� �������� ��ȯ�ϴ� �Լ�
	*	@param	ch	����
	*	@return	�ش� ���ڰ� ���� ���� �����̸� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool isFilterSegment(wchar_t ch);

	/**
	*	@brief	���� ������ Ȯ���ڸ� �����ϴ� �Լ�
	*	@pre	Ȯ���� ����� ������� �ʾƾ� �Ѵ�.
	*	@post	���� ������ Ȯ���ڸ� �����Ѵ�.
	*	@param	dialog	���� ����
	*	@param	description	Ȯ������ ���� ���
	*	@param	filter	Ȯ���� ���
	*	@return	�����ϸ� DIALOG_SUCCESS, �ƴϸ� DIALOG_ERROR�� ��ȯ�Ѵ�.
	*/
	DialogResult setFilterToDialog(IFileDialog* dialog, const std::wstring& description, const std::wstring& filter);

	/**
	*	@brief	�⺻ ��θ� �����ϴ� �Լ�
	*	@param	dialog	���� ����
	*	@return	�����ϸ� DIALOG_SUCCESS, �ƴϸ� DIALOG_ERROR�� ��ȯ�Ѵ�.
	*/
	DialogResult setDefaultPath(IFileDialog* dialog);

public:

	/**
	*	@brief	�⺻ ������
	*	@param	default_path	�⺻ ���
	*/
	FileDialog(const std::wstring& default_path = L"") : defaultPath(default_path) {};

	/**
	*	@brief	���� ��θ� ���� ���ڸ� ���� �����Ѵ�.
	*	@param	resultPath	������ ��θ� ������ ���ڿ�
	*	@return	�����ϸ� DIALOG_SUCCESS, ����ϸ� DIALOG_CANCEL, �ƴϸ� DIALOG_ERROR�� ��ȯ�Ѵ�.
	*/
	DialogResult openDirectoryDialog(std::wstring& resultPath);

	/**
	*	@brief	���� ��θ� ���� ���ڸ� ���� �����Ѵ�.
	*	@pre	description �� filter�� ���� ���� ������ ','�� �����ؾ� �ϰ� �ٸ� ���ʹ� ';'�� �����ؾ� �Ѵ�.
	*	@post	���� ���ڸ� ���� ���� ��θ� �����Ѵ�.
	*	@param	resultPath	������ ��θ� ������ ���ڿ�
	*	@param	description	Ȯ������ ���� ���
	*	@param	filter	Ȯ���� ���
	*	@return	�����ϸ� DIALOG_SUCCESS, ����ϸ� DIALOG_CANCEL, �ƴϸ� DIALOG_ERROR�� ��ȯ�Ѵ�.
	*/
	DialogResult openFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter);

	/**
	*	@brief	������ ������ ��θ� ���� ���ڸ� ���� �����Ѵ�.
	*	@pre	description �� filter�� ���� ���� ������ ','�� �����ؾ� �ϰ� �ٸ� ���ʹ� ';'�� �����ؾ� �Ѵ�.
	*	@post	���� ���ڸ� ���� ���� ��θ� �����Ѵ�.
	*	@param	resultPath	������ ��θ� ������ ���ڿ�
	*	@param	description	Ȯ������ ���� ���
	*	@param	filter	Ȯ���� ���
	*	@param	filterType	������ Ȯ���� ��ȣ�� �������� ������ ����
	*	@return	�����ϸ� DIALOG_SUCCESS, ����ϸ� DIALOG_CANCEL, �ƴϸ� DIALOG_ERROR�� ��ȯ�Ѵ�.
	*/
	DialogResult saveFileDialog(std::wstring& resultPath, const std::wstring& description, const std::wstring& filter, UINT& filterType);
};
#endif