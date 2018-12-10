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
		Sleep(100); //음악 완전히 종료 대기
		app->Next();
		break;
	}
}

LRESULT CALLBACK NumericProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CHAR:
		if (wParam < 20) break; //특수 코드는 제외
		if (GetWindowTextLength(hWnd) >= 8) return 0;
		if (wParam < 48 || wParam > 57) return 0; //숫자가 아닌 경우 처리하지 않음
	}

	return app->InputNumeric(hWnd, iMessage, wParam, lParam);
}

LRESULT CALLBACK EditProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	switch (iMessage)
	{
	case WM_CTLCOLORSTATIC:
	{
		//배경색을 투명으로 한다.
		HDC hdcStatic = (HDC)wParam;
		SetBkColor(hdcStatic, RGB(0xbd, 0xbd, 0xbd));

		return (INT_PTR)CreateSolidBrush(RGB(0xbd, 0xbd, 0xbd));
	}

	case WM_COMMAND:
		//커맨드 중 버튼 클릭 처리
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

	static int xClick; //클릭한 x좌표
	static int yClick; //클릭한 y좌표

	int len;
	HIMC imc = NULL; // IME 핸들
	wchar_t imeCode[16] = { 0 };

	switch (iMessage)
	{
	case WM_CREATE: //윈도우 생성
		Update(hWnd, Closed);
		break;

	case WM_APP_PLAYER_EVENT: //미디어 플레이어
		OnPlayerEvent(hWnd, wParam);
		break;

	case WM_ACTIVATE: //창 활성화
		BringWindowToTop(hWnd);
		if (app->GetHandle() == hWnd && app->IsEditing()) app->SwapEditor(); //editor가 무조건 상위에 오도록 설정
		break;

	case WM_LBUTTONDOWN: //마우스 좌클릭
		xClick = LOWORD(lParam);
		yClick = HIWORD(lParam);

		e.type = CustomWinEvent::MouseDown;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = xClick;
		e.mouse.y = yClick;
		app->pollEvent(e);
		break;

	case WM_SYSCHAR: //시스템 키보드 입력 이벤트
		break;

	case WM_IME_STARTCOMPOSITION: //IME 시작
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

	case WM_IME_ENDCOMPOSITION: //IME 종료
		e.type = CustomWinEvent::IMEResult;
		app->pollEvent(e);
		break;

	case WM_CHAR: //텍스트 입력
		if (GetAsyncKeyState(VK_MENU) || GetAsyncKeyState(VK_CONTROL)) break;
		e.type = CustomWinEvent::TextEntered;
		e.text.unicode = (wchar_t)wParam;
		app->pollEvent(e);
		break;

	case WM_KEYDOWN: //키보드 입력 시작
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

	case WM_KEYUP: //키보드 입력 종료
		break;

	case WM_MOUSEMOVE: //마우스 움직임
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

	case WM_MOUSEHOVER: //마우스가 윈도우 위에 있음
		mouseIn = true;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_LEAVE;
		tme.hwndTrack = hWnd;
		tme.dwHoverTime = 500;
		TrackMouseEvent(&tme); //mouseLeave에 대한 이벤트 추적
		break;

	case WM_MOUSELEAVE: //마우스가 윈도우 바깥으로 나감
		mouseIn = false;
		e.type = CustomWinEvent::MouseLeft;
		app->pollEvent(e);
		break;

	case WM_LBUTTONUP: //마우스 좌클릭 종료
		e.type = CustomWinEvent::MouseUp;
		e.mouse = CustomWinEvent::MouseEvent();
		e.mouse.button = CustomWinEvent::MouseButton::Left;
		e.mouse.x = LOWORD(lParam);
		e.mouse.y = HIWORD(lParam);
		app->pollEvent(e);
		break;

	case WM_CLOSE: //윈도우 종료
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY: //윈도우 해제
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