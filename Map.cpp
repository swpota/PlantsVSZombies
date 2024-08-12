#include "Map.h"
#include"Zombie.h"
#include<iostream>
using namespace std;


void Grid::setXY(int x, int y)
{
	dx = x * (GRID_WIDTH + 1) + 1;
	dy = y * (GRID_HEIGHT + 1) + 1;
}

void Grid::paint()
{
	//刷新标志位复位
	flag_refresh = false;

	//先清空整个格子
	string str(GRID_WIDTH, ' ');
	for (int i = 0; i < GRID_HEIGHT; i++) {
		Goto_XY(dx, dy + i);
		PrintWithColor(str);
	}

	//如果处于爆炸状态，则输出爆炸特效
	if (bomb_flag) {
		string str(GRID_WIDTH, '*');
		Goto_XY(dx, dy);
		PrintWithColor(str, BOOM_COLOR);
		int i;
		for (i = 1; i < GRID_HEIGHT - 1; i++) {
			Goto_XY(dx, dy + i);
			PrintWithColor("*", BOOM_COLOR);
			Goto_XY(dx + GRID_WIDTH - 1, dy + i);
			PrintWithColor("*", BOOM_COLOR);
		}
		Goto_XY(dx, dy + i);
		PrintWithColor(str, BOOM_COLOR);
	}

	//如果格子被选中，则输出选择框
	if (selected) { 
		string str(GRID_WIDTH - 2, '-');
		Goto_XY(dx, dy);
		PrintWithColor("+" + str + "+");
		int i;
		for (i = 1; i < GRID_HEIGHT - 1; i++) {
			Goto_XY(dx, dy + i);
			PrintWithColor("|");
			Goto_XY(dx + GRID_WIDTH - 1, dy + i);
			PrintWithColor("|");
		}
		Goto_XY(dx, dy + i);
		PrintWithColor("+" + str + "+");
	}

	//输出植物、僵尸等信息
	//有植物无僵尸
	if (plant != nullptr && zombies.size() == 0) {
		//植物额外信息
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 - 1);
		plant->printExtra();
		//植物名字
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2);
		plant->printName();
		//植物剩余HP
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 + 1);
		plant->printLife();
	}
	//有植物有僵尸
	else if (plant != nullptr && zombies.size() != 0) {
		//植物额外信息
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 - 2);
		plant->printExtra();
		//植物名
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 - 1);
		plant->printName();
		//植物剩余HP
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2);
		plant->printLife();
		//僵尸数量
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 + 1);
		PrintWithColor("僵尸×");
		PrintWithColor(zombies.size());
	}
	//以下的都是不存在植物的情况
	//僵尸数量为0
	else if (zombies.size() == 0);
	//僵尸数量为1
	else if (zombies.size() == 1) {
		//输出额外信息
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 - 1);

		zombies[0]->printExtra();
		//输出僵尸名字
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2);
		zombies[0]->printName();
		//输出耐久度百分比
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2 + 1);
		PrintWithColor("( ");
		PrintWithColor(zombies[0]->HP * 100 / zombies[0]->maxHP);
		PrintWithColor("% )");
	}
	//格子未被选中，且僵尸数量小于等于格子高度，则要从格子的最上方位置开始输出僵尸名称
	else if (selected == false && zombies.size() <= GRID_HEIGHT) {
		for (int i = 0; i < zombies.size(); i++) {
			Goto_XY(dx + 1, dy + i);
			zombies[i]->printName();
		}
	}
	//格子被选中，且僵尸数量 + 2 小于等于格子高度，则要从格子的次上方位置开始输出僵尸名称
	else if (selected == true && zombies.size() + 2 <= GRID_HEIGHT) {
		for (int i = 0; i < zombies.size(); i++) {
			Goto_XY(dx + 1, dy + i + 1);
			zombies[i]->printName();
		}
	}
	//否则，在格子中间位置输出僵尸数量
	else {
		Goto_XY(dx + 1, dy + GRID_HEIGHT / 2);
		PrintWithColor("僵尸×");
		PrintWithColor(zombies.size());
	}
}

bool Grid::setPlant(Plant* plant)
{
	//如果格子已有植物，则返回false
	if (this->plant != nullptr) return false;
	else {
		this->plant = plant;
		//刷新格子
		flag_refresh = true;
		return true;
	}
}

void Grid::eatPlant(int attack)
{
	//处理：僵尸吃植物
	//如果格子中有植物，则让植物受到攻击
	if (plant != nullptr) {
		plant->HP -= attack;
		//如果植物HP小于等于0，则删除植物
		if (plant->HP <= 0) {
			delPlant();
		}
		//刷新格子
		flag_refresh = true;
	}
}

void Grid::judgeEating()
{
	//处理：检测僵尸是否该吃植物
	//如果格子中有植物且植物可被吃且该格子内有僵尸，则僵尸吃植物
	if (plant != nullptr && plant->eatable && zombies.size() != 0) {
		for (auto& var : zombies) {
			var->eating = true;
		}
	}
	//否则，取消僵尸吃植物的状态
	else if (plant == nullptr && zombies.size() != 0) {
		for (auto& var : zombies) {
			var->eating = false;
		}
	}
}

void Grid::addZombie(Zombie* zombie)
{
	//格子中添加僵尸
	zombies.push_back(zombie);
	flag_refresh = true;//格子需要刷新
}

void Grid::delZombie(Zombie* zombie)
{
	//删除特定的僵尸
	//遍历僵尸列表，找到特定的僵尸，删除
	for (auto itList = zombies.begin(); itList != zombies.end(); )
	{
		if ((*itList) == zombie)
		{
			itList = zombies.erase(itList);
		}
		else
			itList++;
	}
	flag_refresh = true;//格子需要刷新
}

void Grid::hitZombies(int attack)
{
	for (int i = 0; i < zombies.size(); i++) {
		zombies[i]->hit(attack);
	}
}

void Map::paintGridLine()
{
	//网格线（草地块的边界）的绘制
	for (int i = 0; i < GRID_NUM_Y; i++) {
		string str(WINDOWS_WIDTH, '#');
		Goto_XY(0, i * (GRID_HEIGHT + 1));
		PrintWithColor(str);
		for (int j = 1; j <= GRID_HEIGHT; j++) {
			for (int k = 0; k <= GRID_NUM_X + 1; k++) {
				Goto_XY(k * (GRID_WIDTH + 1), i * (GRID_HEIGHT + 1) + j);
				PrintWithColor("#");
			}
		}
	}
	string str(WINDOWS_WIDTH, '#');
	Goto_XY(0, GRID_NUM_Y * (GRID_HEIGHT + 1));
	PrintWithColor(str);
}

bool Map::travGrid(Game& game)
{
	for (int i = 0; i < GRID_NUM_X + 1; i++) {
		for (int j = 0; j < GRID_NUM_Y; j++) {
			//处理僵尸吃植物
			grid[i][j].judgeEating();
			//处理植物活动
			if (grid[i][j].plant != nullptr) {
				grid[i][j].plant->go(game);
			}
		}
	}

	return true;
}

bool Map::setPlant(int ix, int iy, int type)
{
	Plant* newPlant = nullptr;
	switch (type)
	{
    //根据植物类型创建植物对象
	case PEASHOOTER:
		newPlant = new PeaShooter;
		break;
	case SUNFLOWER:
		newPlant = new SunFlower;
		break;
	case SQUASH:
		newPlant = new Squash;
		break;
	case CHERRYBOMB:
		newPlant = new CherryBomb;
		break;
	case WALLNUT:
		newPlant = new WallNut;
		break;
	case SNOWPEA:
		newPlant = new SnowPea;
		break;
	case REPEATER:
		newPlant = new Repeater;
		break;
	case GARLIC:
		newPlant = new Garlic;
		break;
	case JALAPENO:
		newPlant = new Jalapeno;
		break;
	case POTATOMINE:
		newPlant = new PotatoMine;
		break;
	case SPIKEWEED:
		newPlant = new Spikeweed;
		break;
	case TALLNUT:
		newPlant = new TallNut;
		break;

	default:
		break;
	}
	newPlant->setXY(ix, iy);
	if (!grid[ix][iy].setPlant(newPlant)) { //种植失败（格子已有植物）
		delete newPlant;
		return false;
	}
	else
		return true;
}

void Map::refresh()
{
	for (int i = 0; i < GRID_NUM_X + 1; i++) {
		for (int j = 0; j < GRID_NUM_Y; j++) {
			//如果格子需要刷新，则刷新
			if (grid[i][j].flag_refresh) {
				grid[i][j].paint();
			}
		}
	}
}

void Map::setBombFlag(int x, int y)
{
	//越界检查
	if (x<0 || x>GRID_NUM_X || y<0 || y>GRID_NUM_Y - 1) return;
	grid[x][y].setBombFlag();
}

void Map::clearBombFlag(int x, int y)
{
	//越界检查
	if (x<0 || x>GRID_NUM_X || y<0 || y>GRID_NUM_Y - 1) return;
	grid[x][y].clearBombFlag();
}

void Map::init()
{
	system("cls");
	//网格线（草地块的边界）的绘制
	paintGridLine();
	//每个格子初始化
	for (int i = 0; i < GRID_NUM_X + 1; i++) {
		for (int j = 0; j < GRID_NUM_Y; j++) {
			grid[i][j].setXY(i, j);
		}
	}
}