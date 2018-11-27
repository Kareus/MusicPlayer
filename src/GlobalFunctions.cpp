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

	case WM_CHAR:
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