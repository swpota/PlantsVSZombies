#include<iostream>
#include<mutex>

std::mutex mtx;

#include "ui_tools.h"

//���ù��λ��
void Goto_XY(const int x, const int y)
{
	mtx.lock();
	COORD position;
	position.X = x;
	position.Y = y;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position);
	mtx.unlock();
}

//���ع��
void HideCursor()
{
	HANDLE fd = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cinfo;
	cinfo.bVisible = 0;
	cinfo.dwSize = 1;
	SetConsoleCursorInfo(fd, &cinfo);
}

//���ô��ڱ���ʹ�С
void SetWindowSize(int cols, int lines)
{
	system("title \"PlantsVSZombies\"");//���ô��ڱ���
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	// ���û�������С
	COORD coord = { cols, lines };
	SetConsoleScreenBufferSize(hStdout, coord);
	// ���ô��ڴ�С
	SMALL_RECT rect = { 0, 0, cols - 1, lines - 1 };
	SetConsoleWindowInfo(hStdout, TRUE, &rect);
	//���ô��ڴ�С���ֹ�������ڴ�С
	SetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE, GetWindowLongPtrA(GetConsoleWindow(), GWL_STYLE) & ~WS_SIZEBOX & ~WS_MAXIMIZEBOX & ~WS_MINIMIZEBOX);
}

//�����ı���ɫ
void SetColor(int colorID)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorID);
}

// ����ɫ���ı������Ĭ��Ϊ�ڵװ���
void PrintWithColor(const string& str, int colorID)
{
	mtx.lock();
	SetColor(colorID);
	cout << str;
	SetColor(DEFAULT_COLOR); //������������û�Ĭ��ɫ
	mtx.unlock();
}

void PrintWithColor(int num, int colorID)
{
	SetColor(colorID);
	cout << num;
	SetColor(DEFAULT_COLOR); //������������û�Ĭ��ɫ
}