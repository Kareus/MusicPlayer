#pragma once
#ifndef __CUSTOM_WIN_EVENT__
#define __CUSTOM_WIN_EVENT__

class CustomWinEvent
{
public:
	struct IMEEvent
	{
		wchar_t code;
	};

	struct MouseOverEvent
	{
		int x;
		int y;
	};

	enum CustomEventType
	{
		IMEComposing,
		IMEResult,
		IMEEnd,
		MouseOver
	};

	CustomEventType type;

	union
	{
		IMEEvent ime;
		MouseOverEvent mouseOver;
	};
};
#endif