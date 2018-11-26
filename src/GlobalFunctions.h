#pragma once
#ifndef __GLOBAL_FUNCTIONS__
#define __GLOBAL_FUNCTIONS__

#include <iostream>
#include <string>
#include <atlconv.h> //string <-> wstring ��ȯ�� ���� ���
#include <Windows.h>
#include <shellapi.h>
#include <functional>
#include <SFML/Graphics.hpp>
#include "MediaPlayer.h"

//���� �Լ� ���Ǹ� ���� ��� ����

namespace String
{
	//string���� wstring���� ��ȯ�ϴ� �Լ�
	std::wstring StrToWstr(const std::string& str);

	//wstring���� string���� ��ȯ�ϴ� �Լ�
	std::string WstrToStr(const std::wstring& str);
}

namespace Stream
{
	//�� ���� �Է¹޾� ���� �����͸� �����ϴ� �Լ�
	void IgnoreJunk(std::istream& in);
}

namespace Internet
{
	//url�� �������� �� ������ ���� �Լ�
	void OpenURL(const std::wstring& path);
}

//�÷��̾ ���� �����츦 ������Ʈ�ϴ� �Լ�
void Update(HWND hwnd, PlayerState state);

//������ ���������� ó���� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//�÷��̾ ���� ���� �� ó���� �̺�Ʈ
void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr);
#endif