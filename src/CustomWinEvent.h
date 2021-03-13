#pragma once
#ifndef __CUSTOM_WIN_EVENT__
#define __CUSTOM_WIN_EVENT__

#include <windef.h>

/**
*	@brief	이벤트 처리를 위한 클래스
*	@author	Kareus
*/

class CustomWinEvent
{
public:

	/**
	*	@brief	마우스 버튼 타입
	*/
	enum MouseButton
	{
		Left,
		Right
	};

	/**
	*	@brief	IME (한국어 등) 입력 이벤트
	*/
	struct IMEEvent
	{
		wchar_t code; ///<문자 코드
	};

	/**
	*	@brief	마우스 이벤트
	*/
	struct MouseEvent
	{
		MouseButton button; ///<버튼
		int x; ///<x 좌표
		int y; ///<y 좌표
	};

	/**
	*	@brief	텍스트 입력 이벤트
	*/
	struct TextEvent
	{
		wchar_t unicode; ///<텍스트 코드
	};

	/**
	*	@brief	키보드 입력 이벤트
	*/
	struct KeyEvent
	{
		bool shift; ///<shift 키 여부
		bool control; ///<contorl 키 여부
		bool alt; ///<alt 키 여부
		unsigned int code; ///<키 코드
	};

	/**
	*	@brief	이벤트 타입
	*/
	enum CustomEventType
	{
		IMEComposing,
		IMEResult,
		IMEEnd,
		MouseOver,
		MouseMoved,
		MouseDown,
		MouseUp,
		MouseLeft,
		TextEntered,
		KeyPressed,
	};

	CustomEventType type; ///<이벤트 타입

	//한 객체에서 한 이벤트만을 다루므로 union으로 선언
	union
	{
		IMEEvent ime; ///<ime 이벤트 
		MouseEvent mouse; ///<마우스 이벤트
		TextEvent text; ///<텍스트 이벤트
		KeyEvent key; ///<키보드 이벤트
	};
};
#endif