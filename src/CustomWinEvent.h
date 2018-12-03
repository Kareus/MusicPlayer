#pragma once
#ifndef __CUSTOM_WIN_EVENT__
#define __CUSTOM_WIN_EVENT__

#include <windef.h>

class CustomWinEvent
{
public:
	enum MouseButton
	{
		Left,
		Right
	};

	struct IMEEvent
	{
		wchar_t code;
	};

	struct MouseEvent
	{
		MouseButton button;
		int x;
		int y;
	};

	struct TextEvent
	{
		wchar_t unicode;
	};

	struct KeyEvent
	{
		bool shift;
		bool control;
		bool alt;
		unsigned int code;
	};

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

	CustomEventType type;
	HWND hWnd;

	union
	{
		IMEEvent ime;
		MouseEvent mouse;
		TextEvent text;
		KeyEvent key;
	};
};
#endif