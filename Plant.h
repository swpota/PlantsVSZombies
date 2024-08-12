#pragma once
#include<string>
using namespace std;

#include"ui_tools.h"
#include<SDL_mixer.h>

class Game;
class Grid;

#ifndef PLANT_H
#define PLANT_H


/* ����ֲ��Ļ��� */
class Plant {
protected:
	//ֲ����
	string name;
	//��������
	int x, y;

	//ֲ���;öȣ�Ĭ�϶���300��
	int HP;
	int maxHP;

	//�趨ֲ�����ԣ����֡�HP��Ĭ��300��
	void set(const string& str, int ilife = DEFAULT_PLANT_HP) {
		name = str; 
		maxHP = HP = ilife; 
		eatable = true; 
		skipable = true; 
		nameColor = DEFAULT_COLOR;
	}

	friend class Grid;
public:
	//ֲ���Ƿ���Ա��ԣ��磺�ش̲�����
	bool eatable;
	//ֲ���Ƿ�ɱ��������߼��ǿ���У�
	bool skipable;

	//����ֲ������
	const string& getName() const { return name; }
	//�趨��������
	void setXY(int ix, int iy) { x = ix; y = iy; }
	//������Ϊ��Ĭ���ޣ����磺�����㶹����������
	virtual void go(Game& nowGame) {}
	//���ֲ����
	void printName();
	//ֲ��������ɫ
	int nameColor;
	//���ʣ������ֵ��Ĭ�ϸ�ʽ���������д������ش�ʣ��HP��
	virtual void printLife();
	//������Ϣ�����Ĭ���ޣ����磺�����׵�װ����Ϣ
	virtual void printExtra() {}
	//����������
	virtual ~Plant() {}
};


/* ��������ֲ�� */

//�㶹����
class PeaShooter :public Plant {
	//�����ٶ�
	int speed;
	//ʱ�Ӽ���
	int counter;
public:
	PeaShooter() {
		set("�㶹����"); //HPĬ��300
		speed = PEASHOOTER_SPEED; //2��
		counter = 0; 
		nameColor = PLANT_ATTACK_COLOR;
	}
	void go(Game& nowGame);
};

//���տ�
class SunFlower :public Plant {
	//����������ٶ�
	int speed;
	//ʱ�Ӽ���
	int counter;
public:
	SunFlower() {
		set("���տ�");//HPĬ��300
		speed = SUNFLOWER_SPEED;//8��
		counter = 0;
		nameColor = PLANT_STATIC_COLOR;
	}
	void go(Game& nowGame);
};

//�ѹ�
class Squash :public Plant {
	//������
	int attack;
	//��ֲʱ���Ӻ�ſ�ʼ����Ч��
	int speed;
	//ʱ�����
	int counter;

public:
	Squash() {
		set("�ѹ�"); nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = SQUASH_SPEED; //0.8��
		counter = 0; 
		attack = SQUASH_ATTACK;
	}
	void go(Game& nowGame);
	//�ѹϲȽ�ʬԤ����Ч
	static Mix_Chunk* squashWarning1;
	static Mix_Chunk* squashWarning2;
};

//ӣ��ը��
class CherryBomb :public Plant {
	//��ֲ�������ſ�ʼ����Ч��
	int speed;
	//ʱ�����
	int counter;
	//������
	int attack;

public:
	CherryBomb() {
		set("ӣ��ը��"); 
		nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = CHERRYBOMB_SPEED; //0.5��
		counter = 0; 
		attack = CHERRYBOMB_ATTACK;
	}
	void go(Game& nowGame);
	//ӣ��ը����ը��Ч
	static Mix_Chunk* cherryBomb;
};

//���ǽ
class WallNut :public Plant {
public:
	WallNut() {
		set("���ǽ", WALLNUT_HP); 
		nameColor = PLANT_STATIC_COLOR;
	}
};

//��������
class SnowPea :public Plant {
	//�����ٶ�
	int speed;
	//ʱ�Ӽ���
	int counter;
public:
	SnowPea() {
		set("��������"); 
		nameColor = PLANT_ATTACK_COLOR;
		speed = SNOWPEA_SPEED; //0.2��
		counter = 0;
	}
	void go(Game& nowGame);
};

//˫������
class Repeater :public Plant {
	//�����ٶ�
	int speed;
	//ʱ�Ӽ���
	int counter;
public:
	Repeater() {
		set("˫������"); 
		nameColor = PLANT_ATTACK_COLOR;
		speed = REPEATER_SPEED; //0.2��
		counter = 0;
	}
	void go(Game& nowGame);
};

//����
class Garlic :public Plant {
public:
	Garlic() {
		set("����", GARLIC_HP); 
		nameColor = PLANT_STATIC_COLOR;
	}
	void go(Game& nowGame);
};

//������
class Jalapeno :public Plant {
	//��ֲ�������ſ�ʼ����Ч��
	int speed;
	//ʱ�����
	int counter;
	//������
	int attack;

public:
	Jalapeno() {
		set("������"); 
		nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = JALAPENO_SPEED; //0.6��
		counter = 0; 
		attack = JALAPENO_ATTACK;
	}
	void go(Game& nowGame);

	//��������ը��Ч
	static Mix_Chunk* jalapeno;
};

//��������
class PotatoMine :public Plant {
	//��ֲ�������ſ�ʼ����Ч��
	int speed;
	//ʱ�����
	int counter;
	//������
	int attack;
	//��¼�Ƿ���װ�����
	bool ready;

public:
	PotatoMine() {
		set("��������"); 
		nameColor = PLANT_BOMB_COLOR;
		speed = POTATOMINE_SPEED; //7.5��
		counter = 0; 
		attack = POTATOMINE_ATTACK;
		ready = false;
	}
	void go(Game& nowGame);
	void printExtra();

	//�������ױ�ը��Ч
	static Mix_Chunk* potatoMine;
};

//�ش�
class Spikeweed :public Plant {
	//�����ٶ�
	int speed;
	//������
	int attack;
	//ʱ�Ӽ���
	int counter;
public:
	Spikeweed() {
		set(" <�ش�>");
		attack = SPIKEWEED_ATTACK; 
		eatable = false;
		speed = SPIKEWEED_SPEED; //1��
		counter = 0;
	}
	void go(Game& nowGame);
	void printLife() {}
};

//�߼��ǽ
class TallNut :public Plant {
public:
	TallNut() {
		set("�߼��ǽ", TALLNUT_HP); 
		nameColor = PLANT_STATIC_COLOR;
		skipable = false;
	}
};

#endif // !PLANT_H