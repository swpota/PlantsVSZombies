#pragma once
#include "Map.h"
#include "Bullet.h"
#include "Zombie.h"
#include "Store.h"

#include<list>
#include<map>
#include<vector>
#include<SDL_mixer.h>
using namespace std;

//前置声明
class Plant;

#ifndef GAME_H
#define GAME_H

class Game final {
	Map map;
	Store store;

	//子弹队列（豌豆射手等发出的）
	list<Bullet*> bullets;
	//僵尸队列
	list<Zombie*> zombies;

	//当前动作状态，可选择的值包括在下面的枚举类型中
	int state;
	//状态：正常、购买状态、铲除植物、游戏暂停、游戏结束
	enum GAME_STATE { NORMAL, STORE, SHOVEL, PAUSE, OVER };
	//选择铲除植物中
	void weeding();
	//选择购买植物中
	void shopping();

	//当前焦点位置
	int x, y;
	//当前准备种植的植物类型
	int planting_type;
	//开启选择焦点
	void openFocus();
	//关闭选择焦点
	void closeFocus();

	//铲除某个位置的植物
	void delPlant(int ix, int iy);
	//子弹移动
	void moveBullet();
	//子弹的显示输出
	void printBullet();
	//指定位置增加某个类型的僵尸
	void addZombie(int x, int y, int type);
	//僵尸产生速度
	int make_speed;
	int make_counter;
	//僵尸产生逻辑
	void makeZombies();
	//僵尸移动
	bool moveZombie();
	//清除无效僵尸（血量<=0）
	void clearZombie();
	//自行添加僵尸
	void addZombieByMyself();

	//得分
	int score;
	//目标得分
	int targetScore;
	//每个用户对应每个用户的最高得分
	std::map<string, int> highScore;
	//每个关卡可以使用的植物种类
	std::map<int, vector<int>> levelAvailablePlants;
	//当前关卡
	int level;
	//每关的目标分数和游戏时间
	vector<pair<int, int>> levelGoals;
	//游戏开始时间
	time_t startTime;
	//暂停时间
	time_t pauseTime;
	//本关游戏最大时长
	int maxDuration;
	//游戏当前登录的账户
	string currentAccount;
	//刷新得分显示
	void refreshScore();
	//分数增长速度
	int score_speed;
	int score_counter;
	//计分
	void calScore();

	//帮助信息的输出
	void helpInfo();
	//指示是否需要更新帮助信息（状态切换后需要）
	bool refreshHelpInfo;

	//检查是否可以进入下一关
	void checkGameState();
	//根据关卡播放背景音乐
	void playMusic();

	//暂停
	void pause();
	//游戏结束
	void gameover();
	//胜利音乐
	Mix_Music* winMusic;
	//失败音乐
	Mix_Music* loseMusic;
	//选中卡片音效
	Mix_Chunk* ClickMusic;
	//种植植物音效
	Mix_Chunk* PlantMusic;
	//戴夫说话音效
	Mix_Chunk* DaveVoice;
	//警报音效
	Mix_Chunk* AlarmMusic;
	//笑声音效
	Mix_Chunk* LaughMusic;
	//当前关卡的背景音乐
	Mix_Music* currentMusic;

public:
	Game();


	//游戏参数初始化
	void init();
	//游戏主循环
	void loop();

	//子弹队列中增加一个子弹
	void addBullet(Bullet* p);

	//显示PlantsVSZombies
	void displayPVSZ();

	//加载用户最高分
	void loadHighScore(const string& filename);

	//保存用户最高分
	void saveHighScore(const string& filename);

	//设置当前登录的账户
	void setCurrentAccount(const string& account);

	//设置难度
	void setDifficulty();

	//注册界面
	void Register(const string& filename);

	//登录界面
	bool LoginIn(const string& filename);

	//重置密码
	void ResetPassword(const string& filename);

	//查看历史最高分,仅查看所有用户中排名前三的最高分（若用户数小于3时展示所有）
	void showHighScore(const string& filename);

	//设置游戏声音音量
	void setVolume();

	//设置菜单显示
	void showSettingMenu();

	//开始界面
	void Start(const string& filename);

	~Game();


	friend class SunFlower;
	friend class PeaShooter;
	friend class SnowPea;
	friend class Repeater;
	friend class Squash;
	friend class PotatoMine;
	friend class CherryBomb;
	friend class Jalapeno;
	friend class Garlic;
	friend class Spikeweed;
	friend class Dancing_Zombie;
	friend class Box_Zombie;
};

#endif // !GAME_H