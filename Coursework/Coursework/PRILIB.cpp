#include "PRILIB.h"

void My_SetTitle(char* title)
{
	SetConsoleTitle(title);
}

void My_SetColor(int forecolor, int backcolor)
{
	HANDLE winHandle;
	winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(winHandle, forecolor + backcolor * 0x10);
}

void My_SetPosition(int x, int y)
{
	HANDLE winHandle;
	COORD pos;
	pos.X = x;
	pos.Y = y;
	winHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(winHandle, pos);
}