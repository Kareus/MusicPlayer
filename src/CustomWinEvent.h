#pragma once
#ifndef __CUSTOM_WIN_EVENT__
#define __CUSTOM_WIN_EVENT__

class CustomWinEvent
{
public:
	struct IMEEvent
	{
		wchar_t code;
		bool nullCode;
	};

	enum CustomEventType
	{
		IMEComposing,
		IMEEnd
	};

	CustomEventType type;

	union
	{
		IMEEvent ime;
	};
};
#endif