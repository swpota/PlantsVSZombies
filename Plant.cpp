#include "Plant.h"
#include "Game.h"
#include<iostream>
using namespace std;

Mix_Chunk* CherryBomb::cherryBomb = NULL;
Mix_Chunk* Jalapeno::jalapeno = NULL;
Mix_Chunk* PotatoMine::potatoMine = NULL;
Mix_Chunk* Squash::squashWarning1 = NULL;
Mix_Chunk* Squash::squashWarning2 = NULL;

void PeaShooter::go(Game& nowGame)
{
	//���ж��Ҳ��Ƿ���ڽ�ʬ���������򲻹���
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//������ڣ�����ݼ�����Сѡ���Ƿ񹥻�
	if (isAttacking) {
		counter++;
		if (counter >= speed) {
			counter = 0;
			Bullet* p = new Bullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
	}
}

void SunFlower::go(Game& nowGame)
{
	counter++;
	//���ݼ�����������
	if (counter >= speed) {
		counter = 0;
		nowGame.store.addSun(50);
	}
}

void Squash::go(Game& nowGame)
{
	if (counter == 0) {
		nowGame.map.grid[x][y].setBombFlag();
	}

	if (counter < speed) {
		counter++;
		nowGame.map.grid[x][y].flipBombFlag();
	}
	//ֻҪ���乥����Χ���н�ʬ���͹���
	else {
		int random = RANDOM(2);
		if (nowGame.map.grid[x][y].zombies.size() != 0) {
			//�ѹϲȽ�ʬԤ����Ч
			if(random == 0)
				Mix_PlayChannel(-1, squashWarning1, 0);
			else
				Mix_PlayChannel(-1, squashWarning2, 0);
			nowGame.map.grid[x][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
		}
		else if (x + 1 <= GRID_NUM_X && nowGame.map.grid[x + 1][y].zombies.size() != 0) {
			//�ѹϲȽ�ʬԤ����Ч
			if (random == 0)
				Mix_PlayChannel(-1, squashWarning1, 0);
			else
				Mix_PlayChannel(-1, squashWarning2, 0);
			nowGame.map.grid[x + 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
		}
		else if (x - 1 >= 0 && nowGame.map.grid[x - 1][y].zombies.size() != 0) {
			//�ѹϲȽ�ʬԤ����Ч
			if (random == 0)
				Mix_PlayChannel(-1, squashWarning1, 0);
			else
				Mix_PlayChannel(-1, squashWarning2, 0);
			nowGame.map.grid[x - 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
		}
	}
}

void CherryBomb::go(Game& nowGame)
{
	if (counter == 0) {
		//������ը��Ч
		//�ϲ�һ�е���������
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].setBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].setBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].setBombFlag();
		}
		//�м�һ�е���������
		nowGame.map.grid[x][y].setBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].setBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].setBombFlag();
		//�²�һ�е���������
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].setBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].setBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].setBombFlag();
		}
	}

	if (counter < speed) {
		counter++;
		//��˸
		//�ϲ�һ�е���������
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].flipBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].flipBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].flipBombFlag();
		}
		//�м�һ�е���������
		nowGame.map.grid[x][y].flipBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].flipBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].flipBombFlag();
		//�²�һ�е���������
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].flipBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].flipBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].flipBombFlag();
		}
	}
	else {
		//�ϲ�һ�е���������
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].hitZombies(attack);
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].hitZombies(attack);
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].hitZombies(attack);
		}
		//�м�һ�е���������
		nowGame.map.grid[x][y].hitZombies(attack);
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].hitZombies(attack);
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].hitZombies(attack);
		//�²�һ�е���������
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].hitZombies(attack);
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].hitZombies(attack);
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].hitZombies(attack);
		}
		//���ű�ը��Ч
		Mix_PlayChannel(-1, cherryBomb, 0);
		//ȡ����ը��Ч
		//�ϲ�һ�е���������
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].clearBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].clearBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].clearBombFlag();
		}
		//�м�һ�е���������
		nowGame.map.grid[x][y].clearBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].clearBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].clearBombFlag();
		//�²�һ�е���������
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].clearBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].clearBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].clearBombFlag();
		}

		nowGame.delPlant(x, y);
	}
}

void SnowPea::go(Game& nowGame)
{
	//���ж��Ҳ��Ƿ���ڽ�ʬ���������򲻹���
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//������ڽ�ʬ������
	if (isAttacking) {
		counter++;
		//��������������ٶȣ�������������������
		if (counter >= speed) {
			counter = 0;
			Bullet* p = new SnowBullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
	}
}

void Repeater::go(Game& nowGame)
{
	//���ж��Ҳ��Ƿ���ڽ�ʬ���������򲻹���
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//������ڽ�ʬ������
	if (isAttacking) {
		counter++;
		//��������������ٶȣ�������������������
		if (counter >= speed) {
			counter = 0;
			Bullet* p = new Bullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
		//��������������ٶȵ�75%���ٴι���������˫��Ч��
		else if (counter == speed - speed * 25 / 100) {
			Bullet* p = new Bullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
	}
}

void Garlic::go(Game& nowGame)
{
	//�������ڵĽ�ʬ����ƶ������ڸ�
	vector<Zombie*>& zombie = nowGame.map.grid[x][y].zombies;
	for (auto& var : zombie) {
		var->setScape();
	}
}

void Jalapeno::go(Game& nowGame)
{
	if (counter == 0) {
		//������ը��Ч
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].setBombFlag();
		}
	}

	if (counter < speed) {
		counter++;
		//��˸
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].flipBombFlag();
		}
	}
	else {
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].hitZombies(attack);
			//���ű�ը��Ч
			Mix_PlayChannel(-1, jalapeno, 0);
			//ȡ����ը��Ч
			nowGame.map.grid[i][y].clearBombFlag();
		}
		nowGame.delPlant(x, y);
	}
}

void PotatoMine::go(Game& nowGame)
{
	//�������С�����ٶȣ�����Ϊ��Ҫˢ��
	if (counter < speed) {
		counter++;
		nowGame.map.grid[x][y].setRefresh();
	}
	//�����������ٶ�
	else {
		//���δ׼���ã�����Ϊ׼���ã������ñ�ը��־
		if (!ready) {
			ready = true;
			eatable = false;
			nowGame.map.grid[x][y].setBombFlag();
		}
		//���׼���ã�����ը��־ȡ��
		else {
			nowGame.map.grid[x][y].flipBombFlag();
		}
		//����ø������н�ʬ��������ʬ�������ը��־��ɾ��ֲ��
		if (nowGame.map.grid[x][y].zombies.size() != 0) {
			nowGame.map.grid[x][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//���ű�ը��Ч
			Mix_PlayChannel(-1, potatoMine, 0);
		}
		//�����Χ�н�ʬ��������ʬ�������ը��־��ɾ��ֲ��
		else if (x + 1 <= GRID_NUM_X && nowGame.map.grid[x + 1][y].zombies.size() != 0) {
			nowGame.map.grid[x + 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//���ű�ը��Ч
			Mix_PlayChannel(-1, potatoMine, 0);
		}
		//�����Χ�н�ʬ��������ʬ�������ը��־��ɾ��ֲ��
		else if (x - 1 >= 0 && nowGame.map.grid[x - 1][y].zombies.size() != 0) {
			nowGame.map.grid[x - 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//���ű�ը��Ч
			Mix_PlayChannel(-1, potatoMine, 0);
		}
	}
}

void PotatoMine::printExtra()
{
	if (!ready) {
		cout << "(��" << counter * 100 / speed << "%)";
	}
	else {
		cout << " (Ready)";
	}

}

void Plant::printName()
{
	PrintWithColor(name, nameColor);
}

void Plant::printLife()
{
	//���HP
	cout << "<HP" << HP * 100 / maxHP << "%>";
}

void Spikeweed::go(Game& nowGame)
{
	//�������Ϊ0��ȡ����ը��־
	if (counter == 0) {
		nowGame.map.grid[x][y].clearBombFlag();
	}
	counter++;
	//��������������ٶȣ����ñ�ը��־����������ʬ
	if (counter >= speed) {
		counter = 0;
		nowGame.map.grid[x][y].setBombFlag();
		nowGame.map.grid[x][y].hitZombies(attack);
	}
}
