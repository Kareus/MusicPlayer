#pragma once
#ifndef __SIMPLEMUSICTYPE__
#define __SIMPLEMUSICTYPE__

#include <iostream>
#include <fstream>
#include <string>

using namespace std;
/**
*	������ ���� ������ �����ϴ� Ŭ����
*	@author	�輺��
*	@date	2018.10.02
*/

class SimpleMusicType
{
private:
	string id; ///<���� primary key
	string name; ///<���
	unsigned int length; ///<����
	unsigned int time; ///<��� Ƚ��

public:
	/**
	* �⺻ ������
	*/
	SimpleMusicType();

	/**
	* �⺻ �Ҹ���
	*/
	~SimpleMusicType();

	/**
	*	@brief	����� ��ȯ�Ѵ�.
	*	@pre	��� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���.
	*/
	string GetName() const;

	/**
	*	@brief	���� primary key�� ��ȯ�Ѵ�.
	*	@pre	id ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� primary key.
	*/
	string GetID() const;

	/**
	*	@brief	���� ���̸� ��ȯ�Ѵ�.
	*	@pre	length ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� ����.
	*/
	unsigned int GetLength() const;
	
	/**
	*	@brief	���� ��� Ƚ���� ��ȯ�Ѵ�.
	*	@pre	time ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@return	���� ��� Ƚ��.
	*/
	unsigned int GetPlayedTime() const;
	
	/**
	*	@brief	����� �����Ѵ�.
	*	@pre	����.
	*	@post	����� �Ҵ��Ѵ�.
	*	@param	name	������ ���� �̸�.
	*/
	void SetName(const string& name);

	/**
	*	@brief	���� primary key�� �����Ѵ�.
	*	@pre	����.
	*	@post	���� primary key�� �Ҵ��Ѵ�.
	*	@param	id	������ ���� primary key.
	*/
	void SetID(const string& id);

	/**
	*	@brief	���� ���̸� �����Ѵ�.
	*	@pre	����.
	*	@post	���� ���̸� �Ҵ��Ѵ�.
	*	@param	length	������ ���� ����.
	*/
	void SetLength(unsigned int length);

	/**
	*	@brief	���� ���Ƚ���� �����Ѵ�.
	*	@pre	����.
	*	@post	���� ���Ƚ���� �Ҵ��Ѵ�.
	*	@param	time	������ ���� ���Ƚ��.
	*/
	void SetPlayedTime(unsigned int time);

	/**
	*	@brief	���ȣ�� �ֿܼ� ����Ѵ�.
	*	@pre	���ȣ�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	���ȣ�� �ֿܼ� ��µȴ�.
	*/
	void DisplayIDOnScreen();

	/**
	*	@brief	����� �ֿܼ� ����Ѵ�.
	*	@pre	����� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����� �ֿܼ� ��µȴ�.
	*/
	void DisplayNameOnScreen();

	/**
	*	@brief	���� ���̸� �ֿܼ� ����Ѵ�.
	*	@pre	���� ���̰� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	���� ���̰� �ֿܼ� ��µȴ�.
	*/
	void DisplayLengthOnScreen();

	/**
	*	@brief	���� ���Ƚ���� �ֿܼ� ����Ѵ�.
	*	@pre	���� ���Ƚ���� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	���� ���Ƚ���� �ֿܼ� ��µȴ�.
	*/
	void DisplayPlayedTimeOnScreen();

	/**
	*	@brief	������ ������ �ֿܼ� ����Ѵ�.
	*	@pre	������ ����(���ȣ, ���)�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	������ ������ �ֿܼ� ��µȴ�.
	*/
	void DisplayAllOnScreen();
	
	/**
	*	@brief	Ű����κ��� ���� ���� Ű�� �Է¹޴´�.
	*	@pre	����.
	*	@post	���� ���� Ű�� �Ҵ��Ѵ�.
	*/
	void SetIDFromKB();

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ū�� ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ũ�� true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator>(const SimpleMusicType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����ͺ��� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key���� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator<(const SimpleMusicType& data) const;

	/**
	*	@brief	���� �����Ͱ� ���ϴ� �����Ϳ� ������ ��ȯ�Ѵ�.
	*	@pre	���� �����Ϳ� ���ϴ� �������� primary key�� �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	����.
	*	@param	data	���� ������
	*	@return	���� �������� primary key�� ���ϴ� primary key�� ������ true, �ƴϸ� false�� ��ȯ�Ѵ�.
	*/
	bool operator==(const SimpleMusicType& data) const;

	/**
	*	@brief	���� �����Ϳ� ��� �������� ���� �����Ͽ� �����Ѵ�.
	*	@pre	��� �������� ������ �Ҵ�Ǿ� �־�� �Ѵ�.
	*	@post	��� �������� ���� ���� ���� �����Ϳ� ����ȴ�.
	*	@param	data	������ ��� ������
	*	@return	���� ������ ���� �����͸� ��ȯ�Ѵ�.
	*/
	SimpleMusicType& operator=(const SimpleMusicType& data);

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

};
#endif