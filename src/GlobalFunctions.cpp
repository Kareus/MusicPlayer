#include "GlobalFunctions.h"
#include "Application.h"

extern MediaPlayer* player;
extern Application* app;

namespace String
{
	std::wstring StrToWstr(const std::string& str)
	{
		USES_CONVERSION;
		return A2W_CP(str.c_str(), CP_UTF8); //utf8로 인코딩하여 반환
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
	int AlertError(const std::wstring& mes, const std::wstring& caption, UINT BUTTONS)
	{
		return MessageBox(NULL, mes.c_str(), caption.c_str(), BUTTONS);
	}

	void CloseWithError()
	{
		//error crash report
		app->Close();
		return;
	}
}

void Update(HWND hwnd, PlayerState state)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	if (!app->IsRunning()) return DefWindowProc(hWnd, iMessage, wParam, lParam);

	sf::Event e;
	CustomWinEvent e2;
	static bool mouseIn = false;
	TRACKMOUSEEVENT tme;

	static int xClick; //클릭한 x좌표
	static int yClick; //클릭한 y좌표

	int len;
	HIMC imc = NULL; // IME 핸들
	wchar_t imeCode[16] = { 0 };

	switch (iMessage) {
	case WM_CREATE:
		Update(hWnd, Closed);
		break;

	case WM_LBUTTONDOWN:
		xClick = LOWORD(lParam);
		yClick = HIWORD(lParam);

		e.type = sf::Event::MouseButtonPressed;
		e.mouseButton = sf::Event::MouseButtonEvent();
		e.mouseButton.button = sf::Mouse::Button::Left;
		e.mouseButton.x = xClick;
		e.mouseButton.y = yClick;
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
				imeCode[len-1] = 0;
				
				e2.type = CustomWinEvent::IMEComposing;
				e2.ime.code = *imeCode;
				app->pollEvent(e2);
			}
		}
		else if (lParam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(imc, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				ImmGetCompositionString(imc, GCS_RESULTSTR, imeCode, len);
				imeCode[len - 1] = 0;

				e2.type = CustomWinEvent::IMEEnd;
				e2.ime.code = *imeCode;
				app->pollEvent(e2);
			}
		}

		ImmReleaseContext(hWnd, imc); //핸들 해제
		break;

	case WM_IME_ENDCOMPOSITION:
		e2.type = CustomWinEvent::IMEResult;
		app->pollEvent(e2);
		break;

	case WM_CHAR:
		if (GetAsyncKeyState(VK_MENU) || GetAsyncKeyState(VK_CONTROL)) break;
		e.type = sf::Event::TextEntered;
		e.text.unicode = (wchar_t)wParam;
		app->pollEvent(e);
		break;

	case WM_KEYDOWN:
		e.type = sf::Event::KeyPressed;
		e.key = sf::Event::KeyEvent();
		e.key.code = (sf::Keyboard::Key)wParam;
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
		e.type = sf::Event::MouseMoved;
		e.mouseMove = sf::Event::MouseMoveEvent();
		e.mouseMove.x = LOWORD(lParam);
		e.mouseMove.y = HIWORD(lParam);
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
		e.type = sf::Event::MouseLeft;
		app->pollEvent(e);
		break;

	case WM_LBUTTONUP:
		e.type = sf::Event::MouseButtonReleased;
		e.mouseButton = sf::Event::MouseButtonEvent();
		e.mouseButton.button = sf::Mouse::Left;
		e.mouseButton.x = LOWORD(lParam);
		e.mouseButton.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_APP_PLAYER_EVENT:
		OnPlayerEvent(hWnd, wParam);
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