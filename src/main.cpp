#include "Application.h"
#include <Windows.h>

/**
*	@mainpage	music player project.
*
*	@date	2018.09.23
*	@author	�輺��
*/

MediaPlayer* player; ///<�̵� ����ϴ� �÷��̾� ���� ������.

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	Application app;
	app.Run(hInstance);
	return 0;
}