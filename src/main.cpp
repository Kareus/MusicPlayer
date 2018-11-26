#include "Application.h"
#include <Windows.h>

/**
*	@mainpage	music player project.
*
*	@date	2018.09.23
*	@author	김성주
*/

MediaPlayer* player; ///<미디어를 재생하는 플레이어 전역 포인터.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application app;
	app.Run(hInstance);
	return 0;
}