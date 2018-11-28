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

void Update(HWND hwnd, PlayerState state)
{

}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	sf::Event e;
	CustomWinEvent e2;

	int len;
	HIMC imc = NULL; // IME 핸들
	wchar_t imeCode[16];

	switch (iMessage) {
	case WM_CREATE:
		Update(hWnd, Closed);
		break;

	case WM_LBUTTONDOWN:
		e.type = sf::Event::MouseButtonPressed;
		e.mouseButton = sf::Event::MouseButtonEvent();
		e.mouseButton.button = sf::Mouse::Button::Left;
		e.mouseButton.x = LOWORD(lParam);
		e.mouseButton.y = HIWORD(lParam);
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
				e2.ime.nullCode = false;
				app->pollEvent(e2);
			}
			else
			{
				e2.type = CustomWinEvent::IMEEnd;
				e2.ime.nullCode = true;
				app->pollEvent(e2);
			}
		}
		else if (lParam & GCS_RESULTSTR)
		{
			if ((len = ImmGetCompositionString(imc, GCS_RESULTSTR, NULL, 0)) > 0)
			{
				e2.type = CustomWinEvent::IMEEnd;
				e2.ime.nullCode = false;
				app->pollEvent(e2);
			}
		}

		ImmReleaseContext(hWnd, imc); //핸들 해제
		break;

	case WM_CHAR:
		if (GetAsyncKeyState(VK_SHIFT) || GetAsyncKeyState(VK_MENU) || GetAsyncKeyState(VK_CONTROL)) break;
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
	return(DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void OnPlayerEvent(HWND hwnd, WPARAM pUnkPtr)
{
	HRESULT hr = player->HandleEvent(pUnkPtr);
	if (FAILED(hr)) return;
	Update(hwnd, player->GetState());
}