#include"stdafx.h"
#include"Game.h"
#ifdef _WIN32
	#include <Windows.h>
#endif

int main()
{
	Game game;

	//HWND hWnd = GetConsoleWindow();
	//ShowWindow(hWnd, SW_HIDE);

	game.run();

	//system("pause");
	return 0;
}
