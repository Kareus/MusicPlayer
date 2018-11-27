#include "Application.h"
#include <Windows.h>

/**
*	@mainpage	music player project.
*
*	@date	2018.09.23
*	@author	�輺��
*/

MediaPlayer* player; ///<�̵� ����ϴ� �÷��̾� ���� ������.
Application* app; ///<�̵�� �÷��̾� ���ø����̼�
//������ �޽��� ó���� ���� ���������� ������ ������

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	app = new Application();
	app->Run(hInstance);
	delete app;
	return 0;
}