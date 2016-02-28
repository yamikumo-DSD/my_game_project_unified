//main.cpp

#include <DxLib.h>
#include "base_class.h"
#include <exception>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	try
	{
		MyGameProject::GameMain game(true);
		game.game_main();
	}
	catch (const std::exception& e)
	{
		MessageBox(NULL, TEXT(e.what()), TEXT("Exception"), MB_OK);
	}
	return 0;
}

