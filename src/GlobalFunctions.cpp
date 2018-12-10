#include "GlobalFunctions.h"
#include "Application.h"

extern MediaPlayer* player;
extern Application* app;

namespace Resource
{
	void ReleaseResource()
	{
		delete defaultFont;
		delete addSprite;
		delete editSprite;
		delete removeSprite;
		delete backSprite;
		delete playSprite;
		delete tabSprite;
		delete detailSprite;
		delete tolistSprite;
		delete addlistSprite;
		delete saveSprite;
	}
}

namespace String
{
	std::wstring StrToWstr(const std::string& str)
	{
		USES_CONVERSION;
		return A2W(str.c_str());
	}

	std::string WstrToStr(const std::wstring& str)
	{
		USES_CONVERSION;
		return W2A(str.c_str());
	}

	std::string Strip(const std::string& str)
	{
		if (str.empty()) return str;
		std::string newString = "";
		for (int i = 0; i < str.size(); i++)
			if (str.at(i) != ' ') newString += str.at(i);
		return newString;
	}

	std::string Strip_LR(const std::string& str)
	{
		if (str.empty()) return str;
		int start = 0;
		int end = str.size() - 1;
		while (str.at(start) == ' ') start++;
		while (str.at(end) == ' ') end--;
		return str.substr(start, end+1);
	}
}

namespace System
{
	int AlertError(const std::string& mes, const std::string& caption, UINT BUTTONS)
	{
		return MessageBoxA(NULL, mes.c_str(), caption.c_str(), BUTTONS);
	}

	void CloseWithError()
	{
		exit(1);
	}
}

void Update(HWND hwnd, PlayerState state)
{
	switch (state)
	{
	case PlayerState::Stopped:
		Sleep(100); //���� ������ ���� ���
		app->Next();
		break;
	}
}

LRESULT CALLBACK NumericProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam < 20) break; //Ư�� �ڵ�� ����
		if (GetWindowTextLength(hWnd) >= 8) return 0;
		if (wParam < 48 || wParam > 57) return 0; //���ڰ� �ƴ� ��� ó������ ����
	}

	return app->InputNumeric(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CTLCOLORSTATIC:
	{
		//������ �������� �Ѵ�.
		HDC hdcStatic = (HDC)wParam;
		SetBkColor(hdcStatic, RGB(0xbd, 0xbd, 0xbd));

		return (INT_PTR)CreateSolidBrush(RGB(0xbd, 0xbd, 0xbd));
	}

	case WM_COMMAND:
		//Ŀ�ǵ� �� ��ư Ŭ�� ó��
		if (HIWORD(wParam) == BN_CLICKED)
		{
			app->CloseEditor();
			return 0;
		}
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (!app->IsRunning()) return DefWindowProc(hWnd, iMessage, wParam, lParam);

	CustomWinEvent e;

	static bool mouseIn = false;
	TRACKMOUSEEVENT tme;

	static int xClick; //Ŭ���� x��ǥ
	static int yClick; //Ŭ���� y��ǥ

	int len;
	HIMC imc = NULL; // IME �ڵ�
	wchar_t imeCode[16] = { 0 };

	switch (iMessage)
	{
	case WM_CREATE: //������ ����
		Update(hWnd, Closed);
		break;

	case WM_APP_PLAYER_EVENT: //�̵�� �÷��̾�
		OnPlayerEvent(hWnd, wParam);
		break;

	case WM_ACTIVATE: //â Ȱ��ȭ
		BringWindowToTop(hWnd);
		if (app->GetHandle() == hWnd && app->IsEditing()) app->SwapEditor(); //editor�� ������ ������ ������ ����
		break;

	case WM_LBUTTONDOWN: //���콺 ��Ŭ��
		xClick = LOWORD(lParam);
		yClick = HIWORD(lParam);

		e.type = CustomWinEvent::MouseDown;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = xClick;
		e.mouse.y = yClick;
		app->pollEvent(e);
		break;

	case WM_SYSCHAR: //�ý��� Ű���� �Է� �̺�Ʈ
		break;

	case WM_IME_STARTCOMPOSITION: //IME ����
		return 0;

	case WM_IME_COMPOSITION:
		imc = ImmGetContext(hWnd); //ime �ڵ��� �����´�.

		if (lParam & GCS_COMPSTR)
		{
			if ((len = ImmGetCompositionString(imc, GCS_COMPSTR, NULL, 0)) > 0)
			{
				//������
				ImmGetCompositionString(imc, GCS_COMPSTR, imeCode, len);
				imeCode[len - 1] = 0;

				e.type = CustomWinEvent::IMEComposing;
				e.ime.code = *imeCode;
				app->pollEvent(e);
			}
		}
		else if (lParam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(imc, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				ImmGetCompositionString(imc, GCS_RESULTSTR, imeCode, len);
				imeCode[len - 1] = 0;

				e.type = CustomWinEvent::IMEEnd;
				e.ime.code = *imeCode;
				app->pollEvent(e);
			}
		}

		ImmReleaseContext(hWnd, imc); //�ڵ� ����
		break;

	case WM_IME_ENDCOMPOSITION: //IME ����
		e.type = CustomWinEvent::IMEResult;
		app->pollEvent(e);
		break;

	case WM_CHAR: //�ؽ�Ʈ �Է�
		if (GetAsyncKeyState(VK_MENU) || GetAsyncKeyState(VK_CONTROL)) break;
		e.type = CustomWinEvent::TextEntered;
		e.text.unicode = (wchar_t)wParam;
		app->pollEvent(e);
		break;

	case WM_KEYDOWN: //Ű���� �Է� ����
		e.type = CustomWinEvent::KeyPressed;
		e.key = CustomWinEvent::KeyEvent();
		e.key.shift = false;
		e.key.alt = false;
		e.key.control = false;
		e.key.code = wParam;
		if (GetAsyncKeyState(VK_SHIFT)) e.key.shift = true;
		if (GetAsyncKeyState(VK_MENU)) e.key.alt = true;
		if (GetAsyncKeyState(VK_CONTROL)) e.key.control = true;
		app->pollEvent(e);
		break;

	case WM_KEYUP: //Ű���� �Է� ����
		break;

	case WM_MOUSEMOVE: //���콺 ������
		if (!mouseIn)
		{
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = 10;
			TrackMouseEvent(&tme); //mouseHover�� ���� �̺�Ʈ ����
		}
		e.type = CustomWinEvent::MouseMoved;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.x = LOWORD(lParam);
		e.mouse.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_MOUSEHOVER: //���콺�� ������ ���� ����
		mouseIn = true;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = 500;
		TrackMouseEvent(&tme); //mouseLeave�� ���� �̺�Ʈ ����
		break;

	case WM_MOUSELEAVE: //���콺�� ������ �ٱ����� ����
		mouseIn = false;
		e.type = CustomWinEvent::MouseLeft;
		app->pollEvent(e);
		break;

	case WM_LBUTTONUP: //���콺 ��Ŭ�� ����
		e.type = CustomWinEvent::MouseUp;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = LOWORD(lParam);
		e.mouse.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_CLOSE: //������ ����
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY: //������ ����
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, iMessage, wParam, lParam);
}

void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr)
{
	HRESULT hr = player->HandleEvent(pUnkPtr);
	if (FAILED(hr)) return;
	Update(hwnd, player->GetState());
}