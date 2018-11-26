#include "GlobalFunctions.h"

extern MediaPlayer* player;

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
	switch (iMessage) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

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