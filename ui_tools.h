#pragma once
#include<cstdlib>
#include<Windows.h>
#include<string>
using namespace std;

// UI设计 - 尺寸信息
const int GRID_WIDTH = 10; //草坪中每个格子的宽度（字符数）
const int GRID_HEIGHT = 4; //草坪中每个格子的高度（字符数）
const int GRID_NUM_X = 9;  //草坪尺寸为：GRID_NUM_X列 
const int GRID_NUM_Y = 4;  //            GRID_NUM_Y行
//商店UI界面的高度（宽度同窗口宽度）
const int STORE_HEIGHT = 7;
//窗口-宽
const int WINDOWS_WIDTH = (GRID_WIDTH + 1) * GRID_NUM_X + GRID_WIDTH * 3 / 2;
//窗口-高
const int WINDOWS_HEIGHT = (GRID_HEIGHT + 1) * GRID_NUM_Y + STORE_HEIGHT + 2;
//商店窗口的上边界 - y的值
const int STORE_UP_BOUND = (GRID_HEIGHT + 1) * GRID_NUM_Y + 1;
//商店窗口中植物展示信息的宽度
const int PLANT_CARD_WIDTH = 24;
//“阳光 | 得分”中'|'的位置
const int LINE_X1 = WINDOWS_WIDTH - 20;
//“目标 | 阳光”中'|'的位置
const int LINE_X2 = WINDOWS_WIDTH - 40;
//“关卡 | 目标”中'|'的位置
const int LINE_X3 = WINDOWS_WIDTH - 60;
//“时间 | 关卡”中'|'的位置
const int LINE_X4 = WINDOWS_WIDTH - 80;
//帮助信息的所在位置
const int HELP_INFO_Y = WINDOWS_HEIGHT - 1;

// UI设计 - 颜色信息
//默认色 - 黑底亮白字
const int DEFAULT_COLOR = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
//购买时，选中
const int SELECTED_COLOR = BACKGROUND_GREEN | BACKGROUND_BLUE | DEFAULT_COLOR;
//普通豌豆颜色
const int BULLET_COLOR = FOREGROUND_GREEN;
//寒冰豌豆颜色
const int SNOWBULLET_COLOR = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
//僵尸冷冻效果时的名字输出
const int FREEZING_COLOR = SNOWBULLET_COLOR;
//爆炸特效的颜色
const int BOOM_COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY;
//报纸僵尸没报纸后的红眼状态颜色
const int RED_EYE_COLOR = FOREGROUND_RED | FOREGROUND_INTENSITY;
//帮助信息的颜色(白底黑字)
const int HELP_COLOR = BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE;
//游戏状态
const int STATE_COLOR = BACKGROUND_GREEN | BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
//植物名字
const int PLANT_ATTACK_COLOR = BACKGROUND_GREEN | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const int PLANT_STATIC_COLOR = BACKGROUND_GREEN | BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
const int PLANT_BOMB_COLOR = BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY;


// 全局性功能函数声明
// 设置光标位置
void Goto_XY(const int x, const int y);
// 隐藏光标
void HideCursor();
// 产生0~n-1的随机数
inline int RANDOM(int n) { return rand() % n; }
// 设置窗口大小
void SetWindowSize(int cols, int lines);
// 设置文本颜色
void SetColor(int colorID);
// 带颜色的文本输出，默认为黑底白字
void PrintWithColor(const string& str, int colorID = DEFAULT_COLOR);
void PrintWithColor(int num, int colorID = DEFAULT_COLOR);



// 一些全局常量
//时钟周期（毫秒）
const int SLEEP_TIME = 100;
//植物种类数
const int PLANT_TYPE_MAX = 12;
enum PlantType { SUNFLOWER, PEASHOOTER, SQUASH, WALLNUT, POTATOMINE, CHERRYBOMB, SNOWPEA, SPIKEWEED, GARLIC, JALAPENO, REPEATER, TALLNUT };
//植物相关信息
const int SUNFLOWER_PRICE = 50;
const int PEASHOOTER_PRICE = 100;
const int SQUASH_PRICE = 50;
const int CHERRYBOMB_PRICE = 150;
const int WALLNUT_PRICE = 50;
const int SNOWPEA_PRICE = 175;
const int REPEATER_PRICE = 200;
const int GARLIC_PRICE = 50;
const int JALAPENO_PRICE = 125;
const int POTATOMINE_PRICE = 25;
const int SPIKEWEED_PRICE = 100;
const int TALLNUT_PRICE = 125;
const int SUNFLOWER_CD = 75;
const int PEASHOOTER_CD = 75;
const int SQUASH_CD = 120;
const int CHERRYBOMB_CD = 100;
const int WALLNUT_CD = 100;
const int SNOWPEA_CD = 75;
const int REPEATER_CD = 75;
const int GARLIC_CD = 75;
const int JALAPENO_CD = 150;
const int POTATOMINE_CD = 100;
const int SPIKEWEED_CD = 75;
const int TALLNUT_CD = 150;
const int DEFAULT_PLANT_HP = 300;
const int WALLNUT_HP = 1500;
const int GARLIC_HP = 800;
const int TALLNUT_HP = 4000;
const int PEASHOOTER_SPEED = 20 * 1000 / 10 / (SLEEP_TIME);
const int SUNFLOWER_SPEED = 80 * 1000 / 10 / SLEEP_TIME;
const int SQUASH_SPEED = 8 * 1000 / 10 / SLEEP_TIME;
const int CHERRYBOMB_SPEED = 5 * 1000 / 10 / SLEEP_TIME;
const int SNOWPEA_SPEED = 20 * 1000 / 10 / SLEEP_TIME;
const int REPEATER_SPEED = 20 * 1000 / 10 / SLEEP_TIME;
const int JALAPENO_SPEED = 6 * 1000 / 10 / SLEEP_TIME;
const int POTATOMINE_SPEED = 75 * 1000 / 10 / SLEEP_TIME;
const int SPIKEWEED_SPEED = 10 * 1000 / 10 / SLEEP_TIME;
const int SQUASH_ATTACK = 300;
const int CHERRYBOMB_ATTACK = 300;
const int JALAPENO_ATTACK = 300;
const int POTATOMINE_ATTACK = 300;
const int SPIKEWEED_ATTACK = 20;
//僵尸种类数
const int ZOMBIE_TYPE_MAX = 11;
enum ZombieType {
	ZOMBIE, CONEHEAD_ZOMBIE, BUCKET_ZOMBIE, FOOTBALL_ZOMBIE, DOOR_ZOMBIE, NEWSPAPER_ZOMBIE, POLE_ZOMBIE, DANCING_ZOMBIE, BOX_ZOMBIE, FLAG_ZOMBIE, BACKUP_DANCER
};
//僵尸相关信息
//得分
const int DEFAULT_ZOMBIE_SCORE = 50;
const int CONEHEAD_ZOMBIE_SCORE = 75;
const int BUCKET_ZOMBIE_SCORE = 300;
const int FOOTBALL_ZOMBIE_SCORE = 500;
const int DOOR_ZOMBIE_SCORE = 300;
const int NEWSPAPER_ZOMBIE_SCORE = 125;
const int POLE_ZOMBIE_SCORE = 100;
const int DANCING_ZOMBIE_SCORE = 350;
const int BOX_ZOMBIE_SCORE = 250;
//HP
const int DEFAULT_ZOMBIE_HP = 200;
const int FLAG_ZOMBIE_HP = 300;
const int CONEHEAD_ZOMBIE_HP = 570;
const int BUCKET_ZOMBIE_HP = 1000;
const int FOOTBALL_ZOMBIE_HP = 1300;
const int DOOR_ZOMBIE_HP = 1000;
const int DOOR_ZOMBIE_TRANSHP = 500;
const int NEWSPAPER_ZOMBIE_HP = 300;
const int NEWSPAPER_ZOMBIE_TRANSHP = 200;
const int POLE_ZOMBIE_HP = 450;
const int DANCING_ZOMBIE_HP = 500;
const int BOX_ZOMBIE_HP = 800;
//速度
const int DEFAULT_ZOMBIE_SPEED = 35;
const int FLAG_ZOMBIE_SPEED = 20;
const int CONEHEAD_ZOMBIE_SPEED = 35;
const int BUCKET_ZOMBIE_SPEED = 35;
const int FOOTBALL_ZOMBIE_SPEED = 15;
const int DOOR_ZOMBIE_SPEED = 35;
const int NEWSPAPER_ZOMBIE_SPEED_PLUS = 12 * 1000 / 10 / SLEEP_TIME;
const int POLE_ZOMBIE_SPEED = 20;
const int POLE_ZOMBIE_SPEED2 = 35 * 1000 / 10 / SLEEP_TIME;
const int DANCING_ZOMBIE_SPEED = 12;
const int DANCING_ZOMBIE_SPEED2 = 35 * 1000 / 10 / SLEEP_TIME;
const int BOX_ZOMBIE_SPEED = 12;
//攻击力
const int DEFAULT_ZOMBIE_ATTACK = 130;
const int FLAG_ZOMBIE_ATTACK = 140;
const int CONEHEAD_ZOMBIE_ATTACK = 150;
const int BUCKET_ZOMBIE_ATTACK = 150;
const int FOOTBALL_ZOMBIE_ATTACK = 150;
const int DOOR_ZOMBIE_ATTACK = 150;
const int NEWSPAPER_ZOMBIE_ATTACK_PLUS = 230;

const int DEFAULT_ZOMBIE_FREEZING_TIME = 4 * 1000 / SLEEP_TIME;
const int DEFAULT_ZOMBIE_SCAPE_TIME = 8 * 1000 / 10 / SLEEP_TIME;
const int DANCING_ZOMBIE_CALL_TIME = 40 * 1000 / 10 / SLEEP_TIME;
const int BOX_ZOMBIE_BOMB_TIME = 20 * 1000 / 10 / SLEEP_TIME;
