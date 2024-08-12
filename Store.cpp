#include "Store.h"
#include "Map.h"

#include<conio.h>
#include<iostream>
#include<thread>
using namespace std;


void PlantCard::print()
{
	//�ο�λ���趨
	int Dx = 2 + index * PLANT_CARD_WIDTH;
	int Dy = STORE_UP_BOUND + STORE_HEIGHT - 4;//��һ��
	//ÿ��4��ֲ�����ƣ����index>=8�� �������
	if (index >= 8) {
		Dx = 2 + (index - 8) * PLANT_CARD_WIDTH;
		Dy = Dy + 2;
	}
	//ÿ��4��ֲ�����ƣ����8>index>=4�� ��ڶ���
	else if (index >= 4) {
		Dx = 2 + (index - 4) * PLANT_CARD_WIDTH;
		Dy = Dy + 1;
	}

	Goto_XY(Dx, Dy);
	string str(PLANT_CARD_WIDTH, ' ');
	cout << str;
	Goto_XY(Dx, Dy);
	if (flag) {
		SetColor(SELECTED_COLOR);
	}
	if (index <= 8)
		cout << index + 1 << "." << name << " - " << price << "$";
	else
		cout << (char)(index - 9 + 'a') << "." << name << " - " << price << "$";
	if (counter < CD) {
		cout << "(" << counter * 100 / CD << "%)";
	}
	SetColor(DEFAULT_COLOR);
}

void PlantCard::cooling()
{
	if (counter >= CD) return; //������ȴ
	else {
		counter++;
		print();
	}
}

bool PlantCard::coolingDone()
{
	if (counter >= CD)
		return true;
	else
		return false;
}

void Store::init()
{
	//һЩ�����ĳ�ʼ��
	sun = 200;
	counter = 0;
	//ÿ6����������
	speed = 60 * 1000 / 10 / SLEEP_TIME;


	//�̶������UI����
	Goto_XY(2, STORE_UP_BOUND + 1);
	PrintWithColor("���̵꡿");
	Goto_XY(LINE_X1, STORE_UP_BOUND + 1);
	PrintWithColor("|");
	Goto_XY(LINE_X2, STORE_UP_BOUND + 1);
	PrintWithColor("|");
	Goto_XY(LINE_X3, STORE_UP_BOUND + 1);
	PrintWithColor("|");
	Goto_XY(LINE_X4, STORE_UP_BOUND + 1);
	PrintWithColor("|");
	Goto_XY(0, STORE_UP_BOUND + 2);
	string str(WINDOWS_WIDTH, '-');
	PrintWithColor(str);


	//�ɱ��UI���ֵ��״����
	refreshSun();
	for (int i = 0; i < PLANT_TYPE_MAX; i++) {
		plants[i].print();
	}
}

void Store::refreshSun()
{
	//�����
	Goto_XY(LINE_X1 - 17, STORE_UP_BOUND + 1);
	string str(16, ' ');
	cout << str;
	//���������
	Goto_XY(LINE_X1 - 17, STORE_UP_BOUND + 1);
	PrintWithColor("���⣺");
	PrintWithColor(sun);
	PrintWithColor(" $");
}

bool Store::pay(int choice, int x, int y, Map& map)
{
	if (sun < plants[choice].price) {
		Goto_XY(5, STORE_UP_BOUND + 6);
		PrintWithColor("���ⲻ��!", HELP_COLOR);
	}
	else if (!plants[choice].coolingDone()) {
		Goto_XY(5, STORE_UP_BOUND + 6);
		PrintWithColor("ֲ��������ȴ�У�", HELP_COLOR);

	}
	else{
		Goto_XY(5, STORE_UP_BOUND + 6);
		string str(WINDOWS_WIDTH, ' ');
		PrintWithColor(str);
		if (map.setPlant(x, y, choice)) {
			sun -= plants[choice].price;
			refreshSun();
			plants[choice].counter = 0;
			return true;
		}
	}
	return false;
}

void Store::run()
{
	counter++;
	if (counter >= speed) {
		counter = 0;
		addSun();
	}

	//��ȴֲ��
	for (int i = 0; i < PLANT_TYPE_MAX; i++) {
		plants[i].cooling();
	}
}

void Store::renew()
{
	for (int i = 0; i < PLANT_TYPE_MAX; i++) {
		plants[i].counter = plants[i].CD;
		plants[i].print();
	}
}