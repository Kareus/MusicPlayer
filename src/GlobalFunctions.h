#pragma once
#ifndef __GLOBAL_FUNCTIONS__
#define __GLOBAL_FUNCTIONS__

#include <iostream>
#include <string>
#include <atlconv.h> //string <-> wstring 변환을 위한 헤더
#include <Windows.h>
#include <shellapi.h>
#include <functional>
#include <SFML/Graphics.hpp>
#include "MediaPlayer.h"
#include "Sprite.h"

//전역 함수 정의를 위한 헤더 파일

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
	//string에서 wstring으로 변환하는 함수
	std::wstring StrToWstr(const std::string& str);

	//wstring에서 string으로 변환하는 함수
	std::string WstrToStr(const std::wstring& str);

	//공백을 제거하는 함수 (ex: '  abc de  ' -> 'abcde')
	std::string Strip(const std::string& str);

	//좌우 공백만 제거하는 함수 (ex: '  abc de  ' -> 'abc de')
	std::string Strip_LR(const std::string& str);
}

namespace Stream
{
	//한 줄을 입력받아 더미 데이터를 무시하는 함수
	void IgnoreJunk(std::istream& in);
}

namespace Internet
{
	//url을 브라우저의 새 탭으로 여는 함수
	void OpenURL(const std::wstring& path);
}

namespace System
{
	//에러 메시지를 띄우는 함수. 사용자가 어떤 버튼을 눌렀는지를 반환한다.
	int AlertError(const std::string& mes, const std::string& caption, UINT BUTTONS);

	//에러 리포트를 작성하고 프로그램을 종료하는 함수.
	void CloseWithError();
}

//플레이어에 의해 윈도우를 업데이트하는 함수
void Update(HWND hwnd, PlayerState state);

//윈도우 프로시저를 처리할 함수
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//에디터 프로시저를 처리할 함수
LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//숫자만 입력받기 위한 프로시저 함수
LRESULT CALLBACK NumericProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

//플레이어를 실행 중일 때 처리할 이벤트
void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr);
#endif