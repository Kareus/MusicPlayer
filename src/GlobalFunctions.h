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
#include "Sprite.h"

//���� �Լ� ���Ǹ� ���� ��� ����

namespace Resource
{
	static sf::Font* defaultFont = nullptr;
	static Sprite* addSprite = nullptr;
	static Sprite* editSprite = nullptr;
	static Sprite* removeSprite = nullptr;
	static Sprite* backSprite = nullptr;
	static Sprite* playSprite = nullptr;
	static Sprite* tabSprite = nullptr;
	static Sprite* detailSprite = nullptr;
	static Sprite* tolistSprite = nullptr;

	void ReleaseResource();
}

namespace String
{
	//string���� wstring���� ��ȯ�ϴ� �Լ�
	std::wstring StrToWstr(const std::string& str);

	//wstring���� string���� ��ȯ�ϴ� �Լ�
	std::string WstrToStr(const std::wstring& str);

	//������ �����ϴ� �Լ� (ex: '  abc de  ' -> 'abcde')
	std::string Strip(const std::string& str);

	//�¿� ���鸸 �����ϴ� �Լ� (ex: '  abc de  ' -> 'abc de')
	std::string Strip_LR(const std::string& str);
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

namespace System
{
	//���� �޽����� ���� �Լ�. ����ڰ� � ��ư�� ���������� ��ȯ�Ѵ�.
	int AlertError(const std::string& mes, const std::string& caption, UINT BUTTONS);

	//���� ����Ʈ�� �ۼ��ϰ� ���α׷��� �����ϴ� �Լ�.
	void CloseWithError();
}

//�÷��̾ ���� �����츦 ������Ʈ�ϴ� �Լ�
void Update(HWND hwnd, PlayerState state);

//������ ���ν����� ó���� �Լ�
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//������ ���ν����� ó���� �Լ�
LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//���ڸ� �Է¹ޱ� ���� ���ν��� �Լ�
LRESULT CALLBACK NumericProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//�÷��̾ ���� ���� �� ó���� �̺�Ʈ
void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr);
#endif