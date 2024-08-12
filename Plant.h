#pragma once
#include<string>
using namespace std;

#include"ui_tools.h"
#include<SDL_mixer.h>

class Game;
class Grid;

#ifndef PLANT_H
#define PLANT_H


/* 所有植物的基类 */
class Plant {
protected:
	//植物名
	string name;
	//所在网格
	int x, y;

	//植物耐久度（默认都是300）
	int HP;
	int maxHP;

	//设定植物属性：名字、HP（默认300）
	void set(const string& str, int ilife = DEFAULT_PLANT_HP) {
		name = str; 
		maxHP = HP = ilife; 
		eatable = true; 
		skipable = true; 
		nameColor = DEFAULT_COLOR;
	}

	friend class Grid;
public:
	//植物是否可以被吃，如：地刺不被吃
	bool eatable;
	//植物是否可被跳过（高坚果强不行）
	bool skipable;

	//返回植物名字
	const string& getName() const { return name; }
	//设定所在网格
	void setXY(int ix, int iy) { x = ix; y = iy; }
	//特殊行为（默认无），如：发射豌豆、产生阳光
	virtual void go(Game& nowGame) {}
	//输出植物名
	void printName();
	//植物名的颜色
	int nameColor;
	//输出剩余生命值（默认格式输出，可重写，比如地刺剩余HP）
	virtual void printLife();
	//额外信息输出（默认无），如：土豆雷的装填信息
	virtual void printExtra() {}
	//虚析构函数
	virtual ~Plant() {}
};


/* 各类具体的植物 */

//豌豆射手
class PeaShooter :public Plant {
	//发射速度
	int speed;
	//时钟计数
	int counter;
public:
	PeaShooter() {
		set("豌豆射手"); //HP默认300
		speed = PEASHOOTER_SPEED; //2秒
		counter = 0; 
		nameColor = PLANT_ATTACK_COLOR;
	}
	void go(Game& nowGame);
};

//向日葵
class SunFlower :public Plant {
	//产生阳光的速度
	int speed;
	//时钟计数
	int counter;
public:
	SunFlower() {
		set("向日葵");//HP默认300
		speed = SUNFLOWER_SPEED;//8秒
		counter = 0;
		nameColor = PLANT_STATIC_COLOR;
	}
	void go(Game& nowGame);
};

//窝瓜
class Squash :public Plant {
	//攻击力
	int attack;
	//种植时间延后才开始产生效果
	int speed;
	//时间计数
	int counter;

public:
	Squash() {
		set("窝瓜"); nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = SQUASH_SPEED; //0.8秒
		counter = 0; 
		attack = SQUASH_ATTACK;
	}
	void go(Game& nowGame);
	//窝瓜踩僵尸预警音效
	static Mix_Chunk* squashWarning1;
	static Mix_Chunk* squashWarning2;
};

//樱桃炸弹
class CherryBomb :public Plant {
	//种植多少秒后才开始产生效果
	int speed;
	//时间计数
	int counter;
	//攻击力
	int attack;

public:
	CherryBomb() {
		set("樱桃炸弹"); 
		nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = CHERRYBOMB_SPEED; //0.5秒
		counter = 0; 
		attack = CHERRYBOMB_ATTACK;
	}
	void go(Game& nowGame);
	//樱桃炸弹爆炸音效
	static Mix_Chunk* cherryBomb;
};

//坚果墙
class WallNut :public Plant {
public:
	WallNut() {
		set("坚果墙", WALLNUT_HP); 
		nameColor = PLANT_STATIC_COLOR;
	}
};

//寒冰射手
class SnowPea :public Plant {
	//发射速度
	int speed;
	//时钟计数
	int counter;
public:
	SnowPea() {
		set("寒冰射手"); 
		nameColor = PLANT_ATTACK_COLOR;
		speed = SNOWPEA_SPEED; //0.2秒
		counter = 0;
	}
	void go(Game& nowGame);
};

//双发射手
class Repeater :public Plant {
	//发射速度
	int speed;
	//时钟计数
	int counter;
public:
	Repeater() {
		set("双发射手"); 
		nameColor = PLANT_ATTACK_COLOR;
		speed = REPEATER_SPEED; //0.2秒
		counter = 0;
	}
	void go(Game& nowGame);
};

//大蒜
class Garlic :public Plant {
public:
	Garlic() {
		set("大蒜", GARLIC_HP); 
		nameColor = PLANT_STATIC_COLOR;
	}
	void go(Game& nowGame);
};

//火爆辣椒
class Jalapeno :public Plant {
	//种植多少秒后才开始产生效果
	int speed;
	//时间计数
	int counter;
	//攻击力
	int attack;

public:
	Jalapeno() {
		set("火爆辣椒"); 
		nameColor = PLANT_BOMB_COLOR;
		eatable = false;
		speed = JALAPENO_SPEED; //0.6秒
		counter = 0; 
		attack = JALAPENO_ATTACK;
	}
	void go(Game& nowGame);

	//火爆辣椒爆炸音效
	static Mix_Chunk* jalapeno;
};

//土豆地雷
class PotatoMine :public Plant {
	//种植多少秒后才开始产生效果
	int speed;
	//时间计数
	int counter;
	//攻击力
	int attack;
	//记录是否已装填完成
	bool ready;

public:
	PotatoMine() {
		set("土豆地雷"); 
		nameColor = PLANT_BOMB_COLOR;
		speed = POTATOMINE_SPEED; //7.5秒
		counter = 0; 
		attack = POTATOMINE_ATTACK;
		ready = false;
	}
	void go(Game& nowGame);
	void printExtra();

	//土豆地雷爆炸音效
	static Mix_Chunk* potatoMine;
};

//地刺
class Spikeweed :public Plant {
	//攻击速度
	int speed;
	//攻击力
	int attack;
	//时钟计数
	int counter;
public:
	Spikeweed() {
		set(" <地刺>");
		attack = SPIKEWEED_ATTACK; 
		eatable = false;
		speed = SPIKEWEED_SPEED; //1秒
		counter = 0;
	}
	void go(Game& nowGame);
	void printLife() {}
};

//高坚果墙
class TallNut :public Plant {
public:
	TallNut() {
		set("高坚果墙", TALLNUT_HP); 
		nameColor = PLANT_STATIC_COLOR;
		skipable = false;
	}
};

#endif // !PLANT_H