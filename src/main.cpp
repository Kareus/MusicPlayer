#include "Application.h"
#include <Windows.h>

/**
*	@mainpage	music player project.
*
*	@date	2018.09.23
*	@author	김성주
*/

MediaPlayer* player; ///<미디어를 재생하는 플레이어 전역 포인터.
Application* app; ///<미디어 플레이어 애플리케이션
//윈도우 메시지 처리를 위해 전역변수로 선언한 변수들

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	app = new Application();
	app->Run(hInstance);
	delete app;
	return 0;
}