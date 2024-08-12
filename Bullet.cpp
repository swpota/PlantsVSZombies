#include "Bullet.h"
#include "Map.h"
#include "ui_tools.h"


Bullet::Bullet()
{
	speed = 1 * 1000 / (10 * SLEEP_TIME);
	attack = 50;
	counter = 0;
	hit = false;
}

void Bullet::setXY(int dx, int dy)
{
	x = (dx + 1) * (GRID_WIDTH + 1) - 4;
	y = dy * (GRID_HEIGHT + 1) + 1 + GRID_HEIGHT / 2;
}

void Bullet::move(Map& map)
{
	counter++;
	int dx = x / (GRID_WIDTH + 1);
	int dy = (y - 1 - GRID_HEIGHT / 2) / (GRID_HEIGHT + 1);
	//判断是否击中
	if (map.grid[dx][dy].zombies.size() > 0) {
		hitZombie(map.grid[dx][dy].zombies);
		map.grid[dx][dy].setRefresh();
		if (x % (GRID_WIDTH + 1) == 0) { //遮挡的是绘制边界处，修补边界线"#"
			Goto_XY(x, y);
			PrintWithColor("#");
		}
		hit = true;
		return;
	}
	if (counter == speed) {
		//先修补绘制子弹之前位置处格子的图案
		map.grid[dx][dy].setRefresh();
		if (x % (GRID_WIDTH + 1) == 0) { //遮挡的是绘制边界处，修补边界线"#"
			Goto_XY(x, y);
			PrintWithColor("#");
		}
		x += 2;
		dx = x / (GRID_WIDTH + 1);
		//子弹超过边界
		if (dx > GRID_NUM_X) {
			hit = true;
			return;
		}
		//判断是否击中
		if (map.grid[dx][dy].zombies.size() > 0) {
			hitZombie(map.grid[dx][dy].zombies);
			map.grid[dx][dy].setRefresh();
			if (x % (GRID_WIDTH + 1) == 0) { //遮挡的是绘制边界处，修补边界线"#"
				Goto_XY(x, y);
				PrintWithColor("#");
			}
			hit = true;
			return;
		}
		counter = 0;
	}
}

void Bullet::paint()
{
	Goto_XY(x, y);
	PrintWithColor("●", BULLET_COLOR);
}

Mix_Chunk* Bullet::hitSound = NULL;

void Bullet::hitZombie(vector<Zombie*>& zombies)
{
	for (auto& zombie : zombies) {
		zombie->hit(attack);
		//播放击中音效
		Mix_PlayChannel(-1, hitSound, 0);
	}
}

void SnowBullet::hitZombie(vector<Zombie*>& zombies)
{
	for (auto& zombie : zombies) {
		zombie->hit(attack);
		zombie->setFreezing();
		//播放击中音效
		Mix_PlayChannel(-1, hitSound, 0);
	}
}

void SnowBullet::paint()
{
	Goto_XY(x, y);
	PrintWithColor("●", SNOWBULLET_COLOR);
}
