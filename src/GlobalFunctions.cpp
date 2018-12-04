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
}

namespace Stream
{
	void IgnoreJunk(std::istream& in)
	{
		std::string temp;
		getline(in, temp);
	}
}

namespace Internet
{
	void OpenURL(const std::wstring& path)
	{
		ShellExecute(0, 0, path.c_str(), 0, 0, SW_SHOW);
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

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (!app->IsRunning()) return DefWindowProc(hWnd, iMessage, wParam, lParam);

	CustomWinEvent e;
	e.hWnd = hWnd;

	static bool mouseIn = false;
	TRACKMOUSEEVENT tme;

	static int xClick; //클릭한 x좌표
	static int yClick; //클릭한 y좌표

	int len;
	HIMC imc = NULL; // IME 핸들
	wchar_t imeCode[16] = { 0 };

	switch (iMessage)
	{
	case WM_CREATE:
		if (app->GetHandle() == hWnd) Update(hWnd, Closed);
		break;

	case WM_ACTIVATE:
		BringWindowToTop(hWnd);
		if (app->GetHandle() == hWnd && app->IsEditing()) BringWindowToTop(app->GetEditor()); //editor가 무조건 상위에 오도록 설정
		break;

	case WM_LBUTTONDOWN:
		xClick = LOWORD(lParam);
		yClick = HIWORD(lParam);

		e.type = CustomWinEvent::MouseDown;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = xClick;
		e.mouse.y = yClick;
		app->pollEvent(e);
		break;

	case WM_SYSCHAR:
		break;

	case WM_IME_STARTCOMPOSITION:
		return 0;

	case WM_IME_COMPOSITION:
		imc = ImmGetContext(hWnd); //ime 핸들을 가져온다.

		if (lParam & GCS_COMPSTR)
		{
			if ((len = ImmGetCompositionString(imc, GCS_COMPSTR, NULL, 0)) > 0)
			{
				//조립중
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

		ImmReleaseContext(hWnd, imc); //핸들 해제
		break;

	case WM_IME_ENDCOMPOSITION:
		e.type = CustomWinEvent::IMEResult;
		app->pollEvent(e);
		break;

	case WM_CHAR:
		if (GetAsyncKeyState(VK_MENU) || GetAsyncKeyState(VK_CONTROL)) break;
		e.type = CustomWinEvent::TextEntered;
		e.text.unicode = (wchar_t)wParam;
		app->pollEvent(e);
		break;

	case WM_KEYDOWN:
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

	case WM_KEYUP:
		break;

	case WM_MOUSEMOVE:
		if (!mouseIn)
		{
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_HOVER;
			tme.hwndTrack = hWnd;
			tme.dwHoverTime = 10;
			TrackMouseEvent(&tme); //mouseHover에 대한 이벤트 추적
		}
		e.type = CustomWinEvent::MouseMoved;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.x = LOWORD(lParam);
		e.mouse.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_MOUSEHOVER:
		mouseIn = true;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = 500;
		TrackMouseEvent(&tme); //mouseLeave에 대한 이벤트 추적
		break;

	case WM_MOUSELEAVE:
		mouseIn = false;
		e.type = CustomWinEvent::MouseLeft;
		app->pollEvent(e);
		break;

	case WM_LBUTTONUP:
		e.type = CustomWinEvent::MouseUp;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = LOWORD(lParam);
		e.mouse.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
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