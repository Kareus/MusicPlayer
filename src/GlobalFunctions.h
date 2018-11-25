#pragma once
#ifndef __GLOBAL_FUNCTIONS__
#define __GLOBAL_FUNCTIONS__

#include <iostream>
#include <string>
#include <atlconv.h> //string <-> wstring 변환을 위한 헤더
#include <Windows.h>
#include <shellapi.h>

//전역 함수 정의를 위한 헤더 파일

namespace String
{
	//string에서 wstring으로 변환하는 함수
	std::wstring StrToWstr(const std::string& str);

	//wstring에서 string으로 변환하는 함수
	std::string WstrToStr(const std::wstring& str);
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
#endif