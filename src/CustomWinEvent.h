#pragma once
#ifndef __CUSTOM_WIN_EVENT__
#define __CUSTOM_WIN_EVENT__

#include <windef.h>

/**
*	@brief	�̺�Ʈ ó���� ���� Ŭ����
*	@author	Kareus
*/

class CustomWinEvent
{
public:

	/**
	*	@brief	���콺 ��ư Ÿ��
	*/
	enum MouseButton
	{
		Left,
		Right
	};

	/**
	*	@brief	IME (�ѱ��� ��) �Է� �̺�Ʈ
	*/
	struct IMEEvent
	{
		wchar_t code; ///<���� �ڵ�
	};

	/**
	*	@brief	���콺 �̺�Ʈ
	*/
	struct MouseEvent
	{
		MouseButton button; ///<��ư
		int x; ///<x ��ǥ
		int y; ///<y ��ǥ
	};

	/**
	*	@brief	�ؽ�Ʈ �Է� �̺�Ʈ
	*/
	struct TextEvent
	{
		wchar_t unicode; ///<�ؽ�Ʈ �ڵ�
	};

	/**
	*	@brief	Ű���� �Է� �̺�Ʈ
	*/
	struct KeyEvent
	{
		bool shift; ///<shift Ű ����
		bool control; ///<contorl Ű ����
		bool alt; ///<alt Ű ����
		unsigned int code; ///<Ű �ڵ�
	};

	/**
	*	@brief	�̺�Ʈ Ÿ��
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

	CustomEventType type; ///<�̺�Ʈ Ÿ��

	//�� ��ü���� �� �̺�Ʈ���� �ٷ�Ƿ� union���� ����
	union
	{
		IMEEvent ime; ///<ime �̺�Ʈ 
		MouseEvent mouse; ///<���콺 �̺�Ʈ
		TextEvent text; ///<�ؽ�Ʈ �̺�Ʈ
		KeyEvent key; ///<Ű���� �̺�Ʈ
	};
};
#endif