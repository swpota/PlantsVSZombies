#pragma once
#include<string>
using namespace std;

#include"ui_tools.h"
#include<SDL_mixer.h>

class Map;
class Game;

#ifndef ZOMBIE_H
#define ZOMBIE_H

//��ͨ��ʬ
class Zombie
{
protected:
	//��������
	int x, y;
	//��ʬ��
	string name;
	//�ƶ��ٶ�
	int speed;
	//Ѫ��
	int HP; //��ǰ
	int maxHP;
	//������
	int attack;
	//��ɱ�÷�
	int score;

	//���Բ����趨
	void set(const string& str, int iscore = DEFAULT_ZOMBIE_SCORE, int ilife = DEFAULT_ZOMBIE_HP, int ispeed = DEFAULT_ZOMBIE_SPEED, int iattack = DEFAULT_ZOMBIE_ATTACK) {
		//���֡���ɱ�÷֡�HP
		name = str; 
		score = iscore; 
		maxHP = HP = ilife;
		//�ٶȣ���ispeed/10����/��
		speed = ispeed * 1000 / 10 / SLEEP_TIME;
		//����������iattack) ���˺�/��
		attack = iattack * SLEEP_TIME / 1000;
	}

	//�����ƶ��ļ���
	int counter;
	//��Ǵ��ڳ�ֲ���״̬
	bool eating;

	//������Ч�����ƶ�/�����ٶȼ���
	bool freezing;
	//����Ч������ʱ��
	int freezing_time;
	//ʱ�����
	int freezing_counter;

	//��������
	bool scape_flag;
	//��������ƶ���������
	int scape_time;
	//ʱ�����
	int scape_counter;

	//�����ʬ���֣��磺����״̬���ֻ��ɫ��
	virtual void printName();
	//������Ϣ�������������/��ֽ����Ϣ��
	virtual void printExtra() {}
	//����������
	virtual ~Zombie() {}

public:
	Zombie() {
		counter = 0; 
		eating = false; 
		set("��ͨ��ʬ");
		//����Ч��������ʼ��
		freezing = false; 
		freezing_counter = 0;
		freezing_time = DEFAULT_ZOMBIE_FREEZING_TIME; //4��
		//����Ч��������ʼ��
		scape_flag = false; 
		scape_counter = 0;
		scape_time = DEFAULT_ZOMBIE_SCAPE_TIME;//0.8��
	}
	//���ؽ�ʬ����
	const string& getName() const { return name; }

	//�趨��ʼλ��
	void setXY(int ix, int iy) { x = ix; y = iy; }
	//��ʬ�ж�������λ�ơ���ֲ��ȶ�����������ֵ�����Ƿ���
	virtual bool move(Map& map);
	//��ʬ����Ч����Ĭ���ޣ�
	virtual void go(Game& nowGame) {}
	//������
	virtual void hit(int attack) { HP -= attack; }
	//���������ٵ�Ч�������Ž�ʬ�ȿɷ�����
	virtual void setFreezing() { 
		freezing = true; 
		freezing_counter = 0; 
	};
	//��������Ч����׼������е����ڵ��У�
	void setScape() {
		if (!scape_flag) {
			scape_flag = true; 
			scape_counter = 0;;
		}
	};
	//�þ�̬��Ա�����洢��ʬ��ֲ��ʱ����Ч����Ӽ�ʱ��������Ч�����ٶ�
	static Mix_Chunk* eatingSound;
    int eatingSoundCounter = 0;

	friend class Game;
	friend class Bullet;
	friend class SnowBullet;
	friend class Grid;
};

/* ��������Ľ�ʬ */

class Flag_Zombie :public Zombie {
public:
	Flag_Zombie() { set("ҡ�콩ʬ", DEFAULT_ZOMBIE_SCORE, FLAG_ZOMBIE_HP, FLAG_ZOMBIE_SPEED, FLAG_ZOMBIE_ATTACK); }
};

class Conehead_Zombie :public Zombie {
public:
	Conehead_Zombie() { set("·�Ͻ�ʬ", CONEHEAD_ZOMBIE_SCORE, CONEHEAD_ZOMBIE_HP, CONEHEAD_ZOMBIE_SPEED, CONEHEAD_ZOMBIE_ATTACK); }
};

class Bucket_Zombie :public Zombie {
public:
	Bucket_Zombie() { set("��Ͱ��ʬ", BUCKET_ZOMBIE_SCORE, BUCKET_ZOMBIE_HP, BUCKET_ZOMBIE_SPEED, BUCKET_ZOMBIE_ATTACK); }
};

class Football_Zombie :public Zombie {
public:
	Football_Zombie() { set("���ʬ", FOOTBALL_ZOMBIE_SCORE, FOOTBALL_ZOMBIE_HP, FOOTBALL_ZOMBIE_SPEED, FOOTBALL_ZOMBIE_ATTACK); }
};

class Door_Zombie :public Zombie {
	//�ٽ��
	int transHP;
public:
	Door_Zombie() { 
		set("���Ž�ʬ", DOOR_ZOMBIE_SCORE, DOOR_ZOMBIE_HP, DOOR_ZOMBIE_SPEED, DOOR_ZOMBIE_ATTACK); 
		transHP = DOOR_ZOMBIE_TRANSHP; 
	}
	void setFreezing();
	void printExtra();
};

class Newspaper_Zombie :public Zombie {
	//�ٽ��
	int transHP;
	//��ֽû�˺���ٶ� 1.2��/��
	int speed_Plus = NEWSPAPER_ZOMBIE_SPEED_PLUS;
	//��ֽû�˺�Ĺ�����
	int attack_Plus = NEWSPAPER_ZOMBIE_ATTACK_PLUS;
public:
	Newspaper_Zombie() { 
		set("������ʬ", NEWSPAPER_ZOMBIE_SCORE, NEWSPAPER_ZOMBIE_HP); 
		transHP = NEWSPAPER_ZOMBIE_TRANSHP;
	}
	void setFreezing();
	void printExtra();
	//��д��Ѫ��������ֽû��Ҫ���٣�
	void hit(int attack);
	//û��ֽ�˾ͺ�����ʾ������״̬��
	void printName();
};

class Pole_Zombie :public Zombie {
	//�ٽ�㣺�Ƿ��гŸ�
	bool hasPole;
	//����֮����ٶ� 3.5��/��
	int speed2 = POLE_ZOMBIE_SPEED2;
public:
	Pole_Zombie() { 
		set("�Ÿ˽�ʬ", POLE_ZOMBIE_SCORE, POLE_ZOMBIE_HP, POLE_ZOMBIE_SPEED); 
		hasPole = true; 
	}
	void printExtra();
	bool move(Map& map);
};

class Dancing_Zombie :public Zombie {
	//һ��ʱ��󴥷��ٻ�
	int call_time;
	//�Ա�ʱ�����
	int call_counter;
	//�ٽ�㣺�Ƿ��Ѵ����ٻ�
	bool hasCalled;
	//�ٻ�֮����ٶ� 3.5��/��
	int speed2 = DANCING_ZOMBIE_SPEED2;
public:
	Dancing_Zombie() {
		set("������ʬ", DANCING_ZOMBIE_SCORE, DANCING_ZOMBIE_HP, DANCING_ZOMBIE_SPEED);
		hasCalled = false;
		call_time = DANCING_ZOMBIE_CALL_TIME;
		call_counter = 0;
	}
	void go(Game& nowGame);
};

class Backup_Dancer :public Zombie {
public:
	Backup_Dancer() { set("���轩ʬ"); }
};

class Box_Zombie :public Zombie {
	//һ��ʱ��󴥷������Ա�
	int bomb_time;
	//�Ա�ʱ�����
	int bomb_counter;
	//�Ƿ�������ը
	bool bomb_flag;
public:
	Box_Zombie() {
		set("С��ʬ", BOX_ZOMBIE_SCORE, BOX_ZOMBIE_HP, BOX_ZOMBIE_SPEED);
		//��ըʱ��
		bomb_time = BOX_ZOMBIE_BOMB_TIME;
		bomb_counter = 0;
		bomb_flag = false;
	}
	void go(Game& nowGame);
	//��ը��Ч
	static Mix_Chunk* bombSound;
	//Ц����Ч
	static Mix_Chunk* laughSound1;
	static Mix_Chunk* laughSound2;
	static Mix_Chunk* laughSound3;
};


#endif // !ZOMBIE_H