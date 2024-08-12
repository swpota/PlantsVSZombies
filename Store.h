#pragma once
#include"ui_tools.h"
#include<string>
using namespace std;

class Game;
class Store;
class Map;

#ifndef STORE_H
#define STORE_H


//�����̵�չʾ��ֲ�￨��
class PlantCard final {
	//ֲ����
	int index;
	//ֲ������
	string name;
	//�۸�
	int price;
	//��ȴʱ��
	int CD;
	//�Ƿ�ѡ��
	bool flag;

	//ʱ�����
	int counter;

	friend class Store;
public:
	//�趨����
	void set(int i, const string& iname, int iprice, int iCD) {
		index = i;
		name = iname;
		price = iprice;
		CD = iCD * 1000 / 10 / SLEEP_TIME; //(iCD/10)��ֵ��ʾ�������CDʱ��
		counter = CD;
		flag = false;
	}
	//���ֲ�ﹺ����Ϣ�����֡��۸��Լ���ȴ���ȣ�
	void print();
	//�л�ѡ��/��ѡ��
	void setSelect() { flag = true; print(); }
	void setUnSelect() { flag = false; print(); }
	//��ȴ
	void cooling();
	//�Ƿ���ȴ����
	bool coolingDone();
};

class Store final {
	//��������
	int sun;
	//��Ȼ����������ٶ�
	int speed;
	//����ֲ��ֲ��
	PlantCard plants[PLANT_TYPE_MAX];

	//ʱ�����
	int counter;
public:
	Store() {
		plants[0].set(0, "���տ�  ", SUNFLOWER_PRICE, SUNFLOWER_CD);
		plants[1].set(1, "�㶹����", PEASHOOTER_PRICE, PEASHOOTER_CD);
		plants[2].set(2, "�ѹ�    ", SQUASH_PRICE, SQUASH_CD);
		plants[3].set(3, "���ǽ  ", WALLNUT_PRICE, WALLNUT_CD);

		plants[4].set(4, "��������", POTATOMINE_PRICE, POTATOMINE_CD);
		plants[5].set(5, "ӣ��ը��", CHERRYBOMB_PRICE, CHERRYBOMB_CD);
		plants[6].set(6, "��������", SNOWPEA_PRICE, SNOWPEA_CD);
		plants[7].set(7, "�ش�    ", SPIKEWEED_PRICE, SPIKEWEED_CD);

		plants[8].set(8, "����    ", GARLIC_PRICE, GARLIC_CD);
		plants[9].set(9, "������", JALAPENO_PRICE, JALAPENO_CD);
		plants[10].set(10, "˫������", REPEATER_PRICE, REPEATER_CD);
		plants[11].set(11, "�߼��ǽ", TALLNUT_PRICE, TALLNUT_CD);
	}



	//�̵��ʼ��
	void init();
	//ˢ���������������
	void refreshSun();
	//��������
	void addSun(int isun = 50) { sun += isun; refreshSun(); }
	//���򣬿۳����⣬�����Ƿ���ɹ�
	bool pay(int choice, int x, int y, Map& map);
	//�̵�����(�����������⡢��ȴֲ���)
	void run();
	//����ָ��˲�����������ȴ�����ڲ��ԣ�
	void renew();

	friend class Game;
};


#endif // !STORE_H
