#pragma once
#include"ui_tools.h"
#include<string>
using namespace std;

class Game;
class Store;
class Map;

#ifndef STORE_H
#define STORE_H


//用于商店展示的植物卡牌
class PlantCard final {
	//植物编号
	int index;
	//植物名字
	string name;
	//价格
	int price;
	//冷却时间
	int CD;
	//是否选中
	bool flag;

	//时间计数
	int counter;

	friend class Store;
public:
	//设定参数
	void set(int i, const string& iname, int iprice, int iCD) {
		index = i;
		name = iname;
		price = iprice;
		CD = iCD * 1000 / 10 / SLEEP_TIME; //(iCD/10)的值表示多少秒的CD时间
		counter = CD;
		flag = false;
	}
	//输出植物购买信息（名字、价格，以及冷却进度）
	void print();
	//切换选中/非选中
	void setSelect() { flag = true; print(); }
	void setUnSelect() { flag = false; print(); }
	//冷却
	void cooling();
	//是否冷却结束
	bool coolingDone();
};

class Store final {
	//阳光数量
	int sun;
	//自然生产阳光的速度
	int speed;
	//可种植的植物
	PlantCard plants[PLANT_TYPE_MAX];

	//时间计数
	int counter;
public:
	Store() {
		plants[0].set(0, "向日葵  ", SUNFLOWER_PRICE, SUNFLOWER_CD);
		plants[1].set(1, "豌豆射手", PEASHOOTER_PRICE, PEASHOOTER_CD);
		plants[2].set(2, "窝瓜    ", SQUASH_PRICE, SQUASH_CD);
		plants[3].set(3, "坚果墙  ", WALLNUT_PRICE, WALLNUT_CD);

		plants[4].set(4, "土豆地雷", POTATOMINE_PRICE, POTATOMINE_CD);
		plants[5].set(5, "樱桃炸弹", CHERRYBOMB_PRICE, CHERRYBOMB_CD);
		plants[6].set(6, "寒冰射手", SNOWPEA_PRICE, SNOWPEA_CD);
		plants[7].set(7, "地刺    ", SPIKEWEED_PRICE, SPIKEWEED_CD);

		plants[8].set(8, "大蒜    ", GARLIC_PRICE, GARLIC_CD);
		plants[9].set(9, "火爆辣椒", JALAPENO_PRICE, JALAPENO_CD);
		plants[10].set(10, "双发射手", REPEATER_PRICE, REPEATER_CD);
		plants[11].set(11, "高坚果墙", TALLNUT_PRICE, TALLNUT_CD);
	}



	//商店初始化
	void init();
	//刷新阳光数量的输出
	void refreshSun();
	//增加阳光
	void addSun(int isun = 50) { sun += isun; refreshSun(); }
	//购买，扣除阳光，返回是否购买成功
	bool pay(int choice, int x, int y, Map& map);
	//商店运行(周期增加阳光、冷却植物等)
	void run();
	//金手指：瞬间完成所有冷却（便于测试）
	void renew();

	friend class Game;
};


#endif // !STORE_H
