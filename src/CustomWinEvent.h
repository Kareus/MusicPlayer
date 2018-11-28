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

	enum CustomEventType
	{
		IMEComposing,
		IMEResult,
		IMEEnd
	};

	CustomEventType type;

	union
	{
		IMEEvent ime;
	};
};
#endif