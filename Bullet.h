#pragma once

#include "Zombie.h"
#include<vector>
#include<SDL_mixer.h>

class Map;

#ifndef BULLET_H
#define BULLET_H


//�ӵ���
class Bullet
{
protected:
	//�ӵ���ǰ��λ��
	int x, y;
	//�ƶ��ٶȣ�speed��ʱ�������ƶ�һ���ַ����ӣ�
	int speed;
	//��������ÿ�λ��н�ʬ���۳���Ѫ����
	int attack;

	//�����ƶ��ļ���
	int counter;

public:
	Bullet();
	//���㲢�����ӵ�����ʼ���꣨(dx,dy)λ�õ�ֲ�﷢���ӵ���
	void setXY(int dx, int dy);
	//��ǰ�ƶ�
	void move(Map& map);
	//�����ӵ�
	virtual void paint();
	//������ʬ
	virtual void hitZombie(vector<Zombie*>& zombie);
	//����������
	virtual ~Bullet() {}
	//�Ƿ����
	bool hit;
	//�ӵ����н�ʬ����Ч
	static Mix_Chunk* hitSound;
};


//�������ֵ��ӵ�
class SnowBullet :public Bullet {
	void hitZombie(vector<Zombie*>& zombie);
	void paint();
};

#endif // !BULLET_H

