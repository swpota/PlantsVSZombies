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
	//先判断右侧是否存在僵尸，不存在则不攻击
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//如果存在，则根据计数大小选择是否攻击
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
	//根据计数增加阳光
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
	//只要在其攻击范围内有僵尸，就攻击
	else {
		int random = RANDOM(2);
		if (nowGame.map.grid[x][y].zombies.size() != 0) {
			//窝瓜踩僵尸预警音效
			if(random == 0)
				Mix_PlayChannel(-1, squashWarning1, 0);
			else
				Mix_PlayChannel(-1, squashWarning2, 0);
			nowGame.map.grid[x][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
		}
		else if (x + 1 <= GRID_NUM_X && nowGame.map.grid[x + 1][y].zombies.size() != 0) {
			//窝瓜踩僵尸预警音效
			if (random == 0)
				Mix_PlayChannel(-1, squashWarning1, 0);
			else
				Mix_PlayChannel(-1, squashWarning2, 0);
			nowGame.map.grid[x + 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
		}
		else if (x - 1 >= 0 && nowGame.map.grid[x - 1][y].zombies.size() != 0) {
			//窝瓜踩僵尸预警音效
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
		//开启爆炸特效
		//上侧一列的三个格子
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].setBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].setBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].setBombFlag();
		}
		//中间一列的三个格子
		nowGame.map.grid[x][y].setBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].setBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].setBombFlag();
		//下侧一列的三个格子
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].setBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].setBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].setBombFlag();
		}
	}

	if (counter < speed) {
		counter++;
		//闪烁
		//上侧一列的三个格子
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].flipBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].flipBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].flipBombFlag();
		}
		//中间一列的三个格子
		nowGame.map.grid[x][y].flipBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].flipBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].flipBombFlag();
		//下侧一列的三个格子
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].flipBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].flipBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].flipBombFlag();
		}
	}
	else {
		//上侧一列的三个格子
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].hitZombies(attack);
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].hitZombies(attack);
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].hitZombies(attack);
		}
		//中间一列的三个格子
		nowGame.map.grid[x][y].hitZombies(attack);
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].hitZombies(attack);
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].hitZombies(attack);
		//下侧一列的三个格子
		if (y + 1 <= GRID_NUM_Y - 1) {
			nowGame.map.grid[x][y + 1].hitZombies(attack);
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y + 1].hitZombies(attack);
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y + 1].hitZombies(attack);
		}
		//播放爆炸音效
		Mix_PlayChannel(-1, cherryBomb, 0);
		//取消爆炸特效
		//上侧一列的三个格子
		if (y - 1 >= 0) {
			nowGame.map.grid[x][y - 1].clearBombFlag();
			if (x - 1 >= 0) nowGame.map.grid[x - 1][y - 1].clearBombFlag();
			if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y - 1].clearBombFlag();
		}
		//中间一列的三个格子
		nowGame.map.grid[x][y].clearBombFlag();
		if (x - 1 >= 0) nowGame.map.grid[x - 1][y].clearBombFlag();
		if (x + 1 <= GRID_NUM_X) nowGame.map.grid[x + 1][y].clearBombFlag();
		//下侧一列的三个格子
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
	//先判断右侧是否存在僵尸，不存在则不攻击
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//如果存在僵尸，攻击
	if (isAttacking) {
		counter++;
		//如果计数到达其速度，攻击，并将计数清零
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
	//先判断右侧是否存在僵尸，不存在则不攻击
	bool isAttacking = false;
	for (int i = x; i <= GRID_NUM_X; i++) {
		if (nowGame.map.grid[i][y].zombies.size() != 0) {
			isAttacking = true; break;
		}
	}
	//如果存在僵尸，攻击
	if (isAttacking) {
		counter++;
		//如果计数到达其速度，攻击，并将计数清零
		if (counter >= speed) {
			counter = 0;
			Bullet* p = new Bullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
		//如果计数到达其速度的75%，再次攻击，创造双发效果
		else if (counter == speed - speed * 25 / 100) {
			Bullet* p = new Bullet;
			p->setXY(x, y);
			nowGame.addBullet(p);
		}
	}
}

void Garlic::go(Game& nowGame)
{
	//将格子内的僵尸随机移动到相邻格
	vector<Zombie*>& zombie = nowGame.map.grid[x][y].zombies;
	for (auto& var : zombie) {
		var->setScape();
	}
}

void Jalapeno::go(Game& nowGame)
{
	if (counter == 0) {
		//开启爆炸特效
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].setBombFlag();
		}
	}

	if (counter < speed) {
		counter++;
		//闪烁
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].flipBombFlag();
		}
	}
	else {
		for (int i = 0; i <= GRID_NUM_X; i++) {
			nowGame.map.grid[i][y].hitZombies(attack);
			//播放爆炸音效
			Mix_PlayChannel(-1, jalapeno, 0);
			//取消爆炸特效
			nowGame.map.grid[i][y].clearBombFlag();
		}
		nowGame.delPlant(x, y);
	}
}

void PotatoMine::go(Game& nowGame)
{
	//如果计数小于其速度，设置为需要刷新
	if (counter < speed) {
		counter++;
		nowGame.map.grid[x][y].setRefresh();
	}
	//计数到达其速度
	else {
		//如果未准备好，设置为准备好，并设置爆炸标志
		if (!ready) {
			ready = true;
			eatable = false;
			nowGame.map.grid[x][y].setBombFlag();
		}
		//如果准备好，将爆炸标志取反
		else {
			nowGame.map.grid[x][y].flipBombFlag();
		}
		//如果该格子内有僵尸，攻击僵尸并清除爆炸标志，删除植物
		if (nowGame.map.grid[x][y].zombies.size() != 0) {
			nowGame.map.grid[x][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//播放爆炸音效
			Mix_PlayChannel(-1, potatoMine, 0);
		}
		//如果周围有僵尸，攻击僵尸并清除爆炸标志，删除植物
		else if (x + 1 <= GRID_NUM_X && nowGame.map.grid[x + 1][y].zombies.size() != 0) {
			nowGame.map.grid[x + 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//播放爆炸音效
			Mix_PlayChannel(-1, potatoMine, 0);
		}
		//如果周围有僵尸，攻击僵尸并清除爆炸标志，删除植物
		else if (x - 1 >= 0 && nowGame.map.grid[x - 1][y].zombies.size() != 0) {
			nowGame.map.grid[x - 1][y].hitZombies(attack);
			nowGame.map.grid[x][y].clearBombFlag();
			nowGame.delPlant(x, y);
			//播放爆炸音效
			Mix_PlayChannel(-1, potatoMine, 0);
		}
	}
}

void PotatoMine::printExtra()
{
	if (!ready) {
		cout << "(蓄" << counter * 100 / speed << "%)";
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
	//输出HP
	cout << "<HP" << HP * 100 / maxHP << "%>";
}

void Spikeweed::go(Game& nowGame)
{
	//如果计数为0，取消爆炸标志
	if (counter == 0) {
		nowGame.map.grid[x][y].clearBombFlag();
	}
	counter++;
	//如果计数到达其速度，设置爆炸标志，并攻击僵尸
	if (counter >= speed) {
		counter = 0;
		nowGame.map.grid[x][y].setBombFlag();
		nowGame.map.grid[x][y].hitZombies(attack);
	}
}
