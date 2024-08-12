#include<iostream>
#include<mutex>

std::mutex mtx;

#include "ui_tools.h"

//设置光标位置
void Goto_XY(const int x, const int y)
{
	mtx.lock();
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	mtx.unlock();
}

//隐藏光标
void HideCursor()
{
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.bVisible = 0;
	cinfo.dwSize = 1;
	SetConsoleCursorInfo(fd, &cinfo);
}

//设置窗口标题和大小
void SetWindowSize(int cols, int lines)
{
	system("title \"PlantsVSZombies\"");//设置窗口标题
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// 设置缓冲区大小
	COORD coord = { cols, lines };
	SetConsoleScreenBufferSize(hStdout, coord);
	// 设置窗口大小
	SMALL_RECT rect = { 0, 0, cols - 1, lines - 1 };
	SetConsoleWindowInfo(hStdout, TRUE, &rect);
	//设置窗口大小后禁止调整窗口大小
	SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE, GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
}

//设置文本颜色
void SetColor(int colorID)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

// 带颜色的文本输出，默认为黑底白字
void PrintWithColor(const string& str, int colorID)
{
	mtx.lock();
	SetColor(colorID);
	cout << str;
	SetColor(DEFAULT_COLOR); //输出结束后设置回默认色
	mtx.unlock();
}

void PrintWithColor(int num, int colorID)
{
	SetColor(colorID);
	cout << num;
	SetColor(DEFAULT_COLOR); //输出结束后设置回默认色
}