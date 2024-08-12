#define NOMINMAX
#include "Game.h"
#include<iostream>
#include<ctime>
#include<conio.h>
#include<fstream>
#include<algorithm>
#include<iomanip>
using namespace std;

void Game::openFocus()
{
	map.grid[x][y].setSelected();
}

void Game::closeFocus()
{
	state = NORMAL; 
	refreshHelpInfo = true;
	map.grid[x][y].setUnSelected();
}


void Game::displayPVSZ() {
	cout <<
		R"(            $$$$$$$\  $$\                       $$\                       
            $$  __$$\ $$ |                      $$ |                      
            $$ |  $$ |$$ | $$$$$$\  $$$$$$$\  $$$$$$\    $$$$$$$\         
            $$$$$$$  |$$ | \____$$\ $$  __$$\ \_$$  _|  $$  _____|        
            $$  ____/ $$ | $$$$$$$ |$$ |  $$ |  $$ |    \$$$$$$\          
            $$ |      $$ |$$  __$$ |$$ |  $$ |  $$ |$$\  \____$$\         
            $$ |      $$ |\$$$$$$$ |$$ |  $$ |  \$$$$  |$$$$$$$  |        
            \__|      \__| \_______|\__|  \__|   \____/ \_______/         
                                                                          
                              $$\    $$\  $$$$$$\                         
                              $$ |   $$ |$$  __$$\                        
                              $$ |   $$ |$$ /  \__|                       
                              \$$\  $$  |\$$$$$$\                         
                               \$$\$$  /  \____$$\                        
                                \$$$  /  $$\   $$ |                       
                                 \$  /   \$$$$$$  |                       
                                  \_/     \______/                        
                                                                          
      $$$$$$$$\                         $$\       $$\                     
      \____$$  |                        $$ |      \__|                    
          $$  /  $$$$$$\  $$$$$$\$$$$\  $$$$$$$\  $$\  $$$$$$\   $$$$$$$\ 
         $$  /  $$  __$$\ $$  _$$  _$$\ $$  __$$\ $$ |$$  __$$\ $$  _____|
        $$  /   $$ /  $$ |$$ / $$ / $$ |$$ |  $$ |$$ |$$$$$$$$ |\$$$$$$\  
       $$  /    $$ |  $$ |$$ | $$ | $$ |$$ |  $$ |$$ |$$   ____| \____$$\ 
      $$$$$$$$\ \$$$$$$  |$$ | $$ | $$ |$$$$$$$  |$$ |\$$$$$$$\ $$$$$$$  |
      \________| \______/ \__| \__| \__|\_______/ \__| \_______|\_______/    
                                                                                                                                                                                 
)" << '\n';
}

void Game::weeding()
{
	//键盘按键识别
	char ch;
	int temp = 0;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case -32:
			map.grid[x][y].setUnSelected();
			ch = _getch();
			switch (ch)
			{
			case 72://上
				if (y > 0)
					y--;
				break;
			case 80://下
				if (y < GRID_NUM_Y - 1)
					y++;
				break;
			case 75://左
				if (x > 0)
					x--;
				break;
			case 77://右
				if (x < GRID_NUM_X - 1)
					x++;
				break;
			default:
				break;
			}
			map.grid[x][y].setSelected();
			break;
		case '\r':
			delPlant(x, y);
			//这里不用break，接下来，关闭焦点退出
		case 27://ESC
			closeFocus();
			break;
			//空格键暂停
		case ' ':
			pause();
			break;
		case'q':case'Q':
			//暂停游戏，询问是否退出
			temp = state;
			state = PAUSE;
			refreshHelpInfo = true;
			helpInfo();

			Goto_XY(2 + 10, STORE_UP_BOUND);
			PrintWithColor("你是否确定要退出游戏？如果确定，请按下回车键，如果取消，请按下Esc键", HELP_COLOR);
			while (true)
			{
				ch = _getch();
				switch (ch)
				{
				case '\r':
					//退出游戏
					gameover();
					break;
				case 27:
					//取消退出
					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("                                                                    ");
					//恢复游戏状态
					state = temp;
					refreshHelpInfo = true;
					helpInfo();
					break;
				default:
					//按下其他按键，不做处理
					break;
				}
				if (ch == '\r' || ch == 27)
					break;
			}
			break;

		case '+'://金手指：加钱！（便于测试）
			store.addSun(1000);
		case '*'://金手指：瞬间完成所有冷却（便于测试）
			store.renew(); 
			break;
        default:
			break;
		}
	}
	//按键识别结束
}

void Game::shopping()
{
	//键盘按键识别
	char ch;
	int temp = 0;
	vector<int> availablePlants = levelAvailablePlants[level];
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case -32:
			map.grid[x][y].setUnSelected();
			ch = _getch();
			switch (ch)
			{
			case 72://上
				if (y > 0)
					y--;
				break;
			case 80://下
				if (y < GRID_NUM_Y - 1)
					y++;
				break;
			case 75://左
				if (x > 0)
					x--;
				break;
			case 77://右
				if (x < GRID_NUM_X - 1)
					x++;
				break;
			default:
				break;
			}
			map.grid[x][y].setSelected();
			break;
		case 27://ESC
			store.plants[planting_type].setUnSelect();
			closeFocus();
			break;
		case '\r':
			if (store.pay(planting_type, x, y, map)) { //如果购买成功
				//播放种植音效
				Mix_PlayChannel(-1, PlantMusic, 0); 
				store.plants[planting_type].setUnSelect();
				closeFocus();
			}
			break;
			//空格键暂停
		case ' ':
			pause();
			break;
		case'q':case'Q':
			//暂停游戏，询问是否退出
			temp = state;
			state = PAUSE;
			refreshHelpInfo = true;
			helpInfo();

			Goto_XY(2 + 10, STORE_UP_BOUND);
			PrintWithColor("你是否确定要退出游戏？如果确定，请按下回车键，如果取消，请按下Esc键", HELP_COLOR);
			while (true)
			{
				ch = _getch();
				switch (ch)
				{
				case '\r':
					//退出游戏
					gameover();
					break;
				case 27:
					//取消退出
					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("                                                                    ");
					//恢复游戏状态
					state = temp;
					refreshHelpInfo = true;
					helpInfo();
					break;
				default:
					//按下其他按键，不做处理
					break;
				}
				if (ch == '\r' || ch == 27)
					break;
			}
			break;

		case '+'://金手指：加钱！（便于测试）
			store.addSun(1000);
		case '*'://金手指：瞬间完成所有冷却（便于测试）
			store.renew(); 
			break;

			/*购买植物的按键识别*/
		case 'a':case'A':
			//播放音效
			Mix_PlayChannel(-1, ClickMusic, 0);
			store.plants[planting_type].setUnSelect();
			planting_type = 9;
			if(find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
			{
				store.plants[planting_type].setSelect();
				Goto_XY(30, STORE_UP_BOUND + 6);
				cout << "                         ";
			}
			else {
				Goto_XY(30, STORE_UP_BOUND + 6);
				PrintWithColor("该植物尚未解锁！", HELP_COLOR);
			}
			break;
		case 'b':case'B':
			//播放音效
			Mix_PlayChannel(-1, ClickMusic, 0);
			store.plants[planting_type].setUnSelect();
			planting_type = 10;
			if(find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
			{
				store.plants[planting_type].setSelect();
				Goto_XY(30, STORE_UP_BOUND + 6);
				cout << "                         ";
			}
			else {
				Goto_XY(30, STORE_UP_BOUND + 6);
				PrintWithColor("该植物尚未解锁！", HELP_COLOR);
			}
			break;
		case 'c':case 'C':
			//播放音效
			Mix_PlayChannel(-1, ClickMusic, 0);
			store.plants[planting_type].setUnSelect();
			planting_type = 11;
			if(find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
			{
				store.plants[planting_type].setSelect();
				Goto_XY(30, STORE_UP_BOUND + 6);
				cout << "                         ";
			}
			else {
				Goto_XY(30, STORE_UP_BOUND + 6);
				PrintWithColor("该植物尚未解锁！", HELP_COLOR);
			}
			break;
		default:
			if (ch >= '1' && ch <= '9') {
				//播放音效
				Mix_PlayChannel(-1, ClickMusic, 0);
				int key = ch - '0' - 1;
				if (key >= 0 && key < PLANT_TYPE_MAX) {
					store.plants[planting_type].setUnSelect();
					planting_type = key;
					if(find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("该植物尚未解锁！", HELP_COLOR);
					}
				}
			}
			break;
		}
	}
	//按键识别结束
}

void Game::delPlant(int ix, int iy)
{
	if (ix<0 || ix>GRID_NUM_X || iy<0 || iy>GRID_NUM_Y - 1) return;
	map.grid[ix][iy].delPlant();
}

void Game::refreshScore()
{
	//先清空
	Goto_XY(LINE_X1 + 4, STORE_UP_BOUND + 1);
	string str(15, ' ');
	cout << str;
	//输出得分
	Goto_XY(LINE_X1 + 4, STORE_UP_BOUND + 1);
	cout << "[分数：" << score << "]";
	//输出当前分数
	Goto_XY(LINE_X3 + 4, STORE_UP_BOUND + 1);
	cout << "[目标：" << targetScore << "]";
	//输出目标关卡
	Goto_XY(LINE_X4 + 4, STORE_UP_BOUND + 1);
	cout << "[关卡：" << level << "]";
	//输出剩余时间
	int leftTime = maxDuration - difftime(time(nullptr), startTime);
	Goto_XY(LINE_X4 - 17, STORE_UP_BOUND + 1);
	cout << "[时间：" << leftTime << "]  ";
}

void Game::calScore()
{
	score_counter++;
	if (score_counter >= score_speed) {
		score_counter = 0;
		score++;
		//检查并更新最高得分
		highScore[currentAccount] = max(highScore[currentAccount], score);
		refreshScore();
	}
}

void Game::helpInfo()
{
	if (refreshHelpInfo) {
		//清空
		string str(WINDOWS_WIDTH, ' ');
		Goto_XY(0, HELP_INFO_Y);
		cout << str;
		Goto_XY(0, HELP_INFO_Y);
		if (state == NORMAL) {
			PrintWithColor(" <游戏中> ", STATE_COLOR);
			PrintWithColor(" 种植：\"1-9\"数字键、\"a-c\"键 | 铲除：\"x\"键 | 添加下一波僵尸：\"z\"键 | 暂停：空格键 | 退出游戏：\"q\"键", HELP_COLOR);
		}
		else if (state == STORE) {
			PrintWithColor(" <种植> ", STATE_COLOR);
			PrintWithColor(" 选择：\"1-9\"数字键、\"a-c\"键 | 确认：回车键 | 返回：Esc键 | 暂停：空格键 | 退出游戏：\"q\"键", HELP_COLOR);
		}
		else if (state == SHOVEL) {
			PrintWithColor(" <铲除> ", STATE_COLOR);
			PrintWithColor(" 确认：回车键 | 返回：Esc键 | 暂停：空格键 | 退出游戏：\"q\"键", HELP_COLOR);
		}
		else if (state == PAUSE) {
			PrintWithColor(" <暂停中> ", STATE_COLOR);
			PrintWithColor(" 退出暂停：空格键 | 退出游戏：\"q\"键", HELP_COLOR);
		}

		refreshHelpInfo = false;
	}
}

void Game::checkGameState() {
	time_t now = time(nullptr);
	//检查是否满足过关条件
	if (score >= targetScore && difftime(now, startTime) == maxDuration) {
		//进入下一关
		level++;
		//播放背景音乐
		playMusic();
		if (level > levelGoals.size()) {
			//通关
	        state = OVER;
			//播放胜利音乐
			//停止当前音乐
			Mix_HaltMusic();
			//播放胜利音乐一次
			Mix_PlayMusic(winMusic, 0);
			system("cls");
			Goto_XY(WINDOWS_WIDTH / 2 - 6, WINDOWS_HEIGHT / 2 - 1);
			PrintWithColor("【恭喜通关】", STATE_COLOR);
			//输出分数
			Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 2);
			cout << "你的分数是： " << score << endl;
			Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 3);
			cout << "你的最高分数是：" << highScore[currentAccount] << endl;
			Goto_XY(0, WINDOWS_HEIGHT - 1);
			SetColor(HELP_COLOR);
			system("pause");
		}
		else {
			targetScore = levelGoals[level - 1].first;
			maxDuration = levelGoals[level - 1].second;
			//设置新关卡的开始时间
			startTime = time(nullptr);
			//提示用户通过此关卡
			Goto_XY(0, HELP_INFO_Y - 1);
			PrintWithColor("【恭喜过关，进入下一关】", STATE_COLOR);
			//清空僵尸，植物保持不变
			for (auto itList = zombies.begin(); itList != zombies.end(); )
			{
				map.grid[(*itList)->x][(*itList)->y].delZombie(*itList);
				delete (*itList);
				itList = zombies.erase(itList);
			}
			//等待一段时间后删除恭喜信息
			Sleep(2000);
			Goto_XY(0, HELP_INFO_Y - 1);
			string str(WINDOWS_WIDTH, ' ');
			cout << str;
		}
	}
	else if (difftime(now, startTime) > maxDuration) {
		//时间到，游戏结束
		gameover();
	}	
}

void Game::setDifficulty() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cout << "请选择难度：";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		cout << "1.简单";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cout << "2.普通";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		cout << "3.困难";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		cout << "4.返回";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "输入错误，请重新输入";
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
			continue;
		}
		switch (userChoice)
		{
		case 1:
			levelGoals = { {600, 60}, {2000, 92}, {4000, 92}, {10000, 92}, {20000, 122}, {30000, 122} };
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cout << "设置成功！";
			system("pause");
			system("cls");
			return;
		case 2:
			levelGoals = { {700, 60}, {3000, 92}, {6000, 92}, {15000, 92}, {30000, 122}, {50000, 122} };
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cout << "设置成功！";
			system("pause");
			system("cls");
			return;
		case 3:
			levelGoals = { {800, 60}, {4000, 92}, {8000, 92}, {20000, 92}, {40000, 122}, {70000, 122} };
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cout << "设置成功！";
			system("pause");
			system("cls");
			return;
		case 4:
			system("cls");
			return;
		default:
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "输入错误，请重新输入";
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
			return;
		}
	}	
}

void Game::playMusic() {
	//停止当前音乐
	Mix_HaltMusic();
	//根据关卡播放背景音乐
	switch (level)
	{
	case 2:
		Mix_FreeMusic(currentMusic);
		currentMusic = Mix_LoadMUS("musics/level2.mp3");
		Mix_PlayChannel(-1, AlarmMusic, 0);
		break;
	case 3:
		Mix_FreeMusic(currentMusic);
		currentMusic = Mix_LoadMUS("musics/level3.mp3");
		Mix_PlayChannel(-1, LaughMusic, 0);
		break;
	case 4:
		Mix_FreeMusic(currentMusic);
		currentMusic = Mix_LoadMUS("musics/level4.mp3");
		break;
	case 5:
		Mix_FreeMusic(currentMusic);
		currentMusic = Mix_LoadMUS("musics/level5.mp3");
		break;
	case 6:
		Mix_FreeMusic(currentMusic);
		currentMusic = Mix_LoadMUS("musics/level6.mp3");
		Mix_PlayChannel(-1, DaveVoice, 0);
		break;
	case 7:
		return;
	default:
		break;
	}
	//播放背景音乐
	Mix_PlayMusic(currentMusic, -1);
}

void Game::pause()
{
	int temp = state;
	state = PAUSE;
	pauseTime = time(nullptr);
	refreshHelpInfo = true;
	helpInfo();
	while (true) {
		char ch;
		if (_kbhit())
		{
			ch = _getch();
			switch (ch)
			{
			case ' '://空格结束暂停
				state = temp; 
				startTime += difftime(time(nullptr), pauseTime);
				refreshHelpInfo = true;
				helpInfo();
				return;
				break;
			case'q':case'Q':
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("你是否确定要退出游戏？如果确定，请按下回车键，如果取消，请按下Esc键", HELP_COLOR);
				while (true)
				{
					ch = _getch();
					switch (ch)
					{
					case '\r':
						//退出游戏
						gameover();
						break;
					case 27:
						//取消退出
						Goto_XY(2 + 10, STORE_UP_BOUND);
						PrintWithColor("                                                                    ");
						break;
					default:
						//按下其他按键，不做处理
						break;
					}
					if (ch == '\r')
						return;
					else if(ch == 27)
						break;
				}
			default:;
			}
		}
		Sleep(SLEEP_TIME);
	}

}

void Game::gameover()
{
	system("cls");
	state = OVER;
	//播放失败音乐
	Mix_HaltMusic();
	Mix_PlayMusic(loseMusic, 0);
	Goto_XY(WINDOWS_WIDTH / 2 - 6, WINDOWS_HEIGHT / 2 - 1);
	PrintWithColor("【游戏结束】", STATE_COLOR);
	//输出分数
	Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 2);
	cout << "你的分数是： " << score << endl;
	Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 3);
	cout << "你的最高分数是：" << highScore[currentAccount] << endl;
	Goto_XY(0, WINDOWS_HEIGHT - 1);
	SetColor(HELP_COLOR);
	system("pause");
}

Game::Game()
{
	HideCursor();
	SetWindowSize(WINDOWS_WIDTH + 1, WINDOWS_HEIGHT + 1);
	srand(time(0));
}

void Game::init()
{
	//初始化地图、商店
	map.init();
	store.init();

	//初始化音乐
	winMusic = Mix_LoadMUS("musics/Win.mp3");
	loseMusic = Mix_LoadMUS("musics/Lose.mp3");
	ClickMusic = Mix_LoadWAV("musics/Clickcard.mp3");
	PlantMusic = Mix_LoadWAV("musics/Plant.mp3");
	DaveVoice = Mix_LoadWAV("musics/DaveVoice.mp3");
	AlarmMusic = Mix_LoadWAV("musics/alarm.mp3");
	LaughMusic = Mix_LoadWAV("musics/evillaugh.mp3");
    
	Mix_VolumeChunk(ClickMusic, MIX_MAX_VOLUME / 2);
	Mix_VolumeChunk(PlantMusic, MIX_MAX_VOLUME / 2);

	state = NORMAL; 
	refreshHelpInfo = true;
	x = y = 0;
	planting_type = 0;

	score = 0;
	score_speed = 1 * 1000 / SLEEP_TIME;
	score_counter = 0;
	level = 1; //初始关卡为1
	//设定关卡可用植物种类
	levelAvailablePlants = { 
	{1, {0, 1, 2, 3}},
	{2, {0, 1, 2, 3, 4, 5, 6, 7}},
	{3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	{4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
	{5, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
	{6, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}} };
	//设定游戏关卡中每关的目标分数与游戏时间
	if (levelGoals.size() == 0){
		levelGoals = { {700, 60}, {3000, 92}, {6000, 92}, {15000, 92}, {30000, 122}, {50000, 122} };//默认为普通难度
	}
	targetScore = levelGoals[0].first;
	maxDuration = levelGoals[0].second;
	startTime = time(nullptr);
	refreshScore();

	//13秒产生一次僵尸
	make_speed = 130 * 1000 / 10 / SLEEP_TIME;
	make_counter = 0;
}

void Game::loop()
{
	//TEST:（一开始就生成特定的僵尸，用于调试）
	//addZombie(GRID_NUM_X, 0, POLE_ZOMBIE);	
	//addZombie(GRID_NUM_X, 2, DANCING_ZOMBIE);
	addZombie(GRID_NUM_X, 2, BOX_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, NEWSPAPER_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, FLAG_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, ZOMBIE);
	int temp = 0;
	while (true) {
		//检查游戏状态
		checkGameState();
		if (state == OVER) {
			break;
		}
		else if (state == NORMAL) {
			vector<int> availablePlants = levelAvailablePlants[level];
			//键盘按键识别
	        char ch;
			if (_kbhit())
			{
				ch = _getch();
				switch (ch)
				{
				case 'a':case 'A':
					//播放音效
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 9;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//开启商店选择
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("该植物尚未解锁！", HELP_COLOR);
					}
					break;
				case 'b':case 'B':
					//播放音效
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 10;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//开启商店选择
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("该植物尚未解锁！", HELP_COLOR);
					}
					break;
				case 'c':case 'C':
					//播放音效
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 11;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//开启商店选择
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("该植物尚未解锁！", HELP_COLOR);
					}
					break;
				//按x/X进入铲除植物
				case 'x':case 'X':
					state = SHOVEL; 
					refreshHelpInfo = true;
					openFocus();
					break;
				//按z/Z添加下一波僵尸
				case 'z':case 'Z':
					addZombieByMyself();
					break;
				//空格键暂停
				case ' ':
					pause();
					break;
				case'q':case'Q':
					//暂停游戏，询问是否退出
					temp = state;
					state = PAUSE; 
					refreshHelpInfo = true;
					helpInfo();

					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("你是否确定要退出游戏？如果确定，请按下回车键，如果取消，请按下Esc键", HELP_COLOR);
					while (true)
					{
						ch = _getch();
						switch (ch)
						{
						case '\r':
							//退出游戏
							gameover();
							break;
						case 27:
							//取消退出
							Goto_XY(2 + 10, STORE_UP_BOUND);
							PrintWithColor("                                                                    ");
							//恢复游戏状态
							state = temp;
							refreshHelpInfo = true;
							helpInfo();
							break;
						default:
							//按下其他按键，不做处理
							break;
						}
						if(ch == '\r' || ch == 27)
							break;
					}
					break;

				case '+'://金手指：加钱！（便于测试）
					store.addSun(1000);
				case '*'://金手指：瞬间完成所有冷却（便于测试）
					store.renew(); break;

				default:
					//直接按下数字键的情况
					if (ch >= '1' && ch <= '9') {
						//播放音效
						Mix_PlayChannel(-1, ClickMusic, 0);
						planting_type = ch - '0' - 1;
						if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
						{
							store.plants[planting_type].setSelect();
							Goto_XY(30, STORE_UP_BOUND + 6);
							cout << "                         ";
							//开启商店选择
							state = STORE;
							refreshHelpInfo = true;
							openFocus();
						}
						else {
							Goto_XY(30, STORE_UP_BOUND + 6);
							PrintWithColor("该植物尚未解锁！", HELP_COLOR);
						}
						break;
					}
				}
			}
			//按键识别结束
		}
		else if (state == STORE) {
			shopping();
		}
		else if (state == SHOVEL) {
			weeding();
		}

		//执行僵尸产生逻辑
		makeZombies();

		//遍历：植物
		if (!map.travGrid(*this)) {
			break;
		}

		//僵尸队列 - 行动
		if (moveZombie()) {
			gameover();//僵尸攻破，游戏结束
			break;
		}

		//商店运行
		store.run();

		//子弹移动
		moveBullet();

		//清除无效僵尸
		clearZombie();

		//刷新显示
		map.refresh();//草地的每个格子
		printBullet();//子弹

		//计分逻辑
		calScore();

		//下方帮助信息的输出显示
		helpInfo();

		Sleep(SLEEP_TIME);
	}

}



void Game::addBullet(Bullet* p)
{
	bullets.push_back(p);
}


void Game::moveBullet()
{
	//全部遍历，执行移动
	for (auto& var : bullets) {
		var->move(map);
	}
	//删除无效的子弹
	for (auto itList = bullets.begin(); itList != bullets.end(); )
	{
		if ((*itList)->hit)
		{
			delete (*itList);
			itList = bullets.erase(itList);
		}
		else {
			itList++;
		}
	}
}

void Game::printBullet()
{
	//全部遍历，执行输出
	for (auto& bullet : bullets) {
		bullet->paint();
	}
}

void Game::addZombie(int x, int y, int type)
{
	if (x<0 || x>GRID_NUM_X || y<0 || y>GRID_NUM_Y - 1) return;
	Zombie* newZombie = nullptr;
	switch (type)
	{
    //根据僵尸类型不同，创建不同的僵尸对象
	case ZOMBIE:
		newZombie = new Zombie();
		break;
	case CONEHEAD_ZOMBIE:
		newZombie = new Conehead_Zombie();
		break;
	case BUCKET_ZOMBIE:
		newZombie = new Bucket_Zombie();
		break;
	case FLAG_ZOMBIE:
		newZombie = new Flag_Zombie();
		break;
	case FOOTBALL_ZOMBIE:
		newZombie = new Football_Zombie();
		break;
	case DOOR_ZOMBIE:
		newZombie = new Door_Zombie();
		break;
	case NEWSPAPER_ZOMBIE:
		newZombie = new Newspaper_Zombie();
		break;
	case POLE_ZOMBIE:
		newZombie = new Pole_Zombie();
		break;
	case DANCING_ZOMBIE:
		newZombie = new Dancing_Zombie();
		break;
	case BACKUP_DANCER:
		newZombie = new Backup_Dancer();
		break;
	case BOX_ZOMBIE:
		newZombie = new Box_Zombie();
		break;


	default:
		break;
	}
	if (newZombie != nullptr) {
		newZombie->setXY(x, y);
		map.grid[x][y].addZombie(newZombie);
		zombies.push_back(newZombie);
	}
}

void Game::makeZombies()
{
	make_counter++;
	if (make_counter >= make_speed) {
		//摇旗僵尸首先出现
		addZombie(GRID_NUM_X, RANDOM(GRID_NUM_Y), FLAG_ZOMBIE);
		//难度随分数增加，分数越高，僵尸产生速度越快
		for (int i = 0; i <= score / 2000; i++) {
			int type = RANDOM(ZOMBIE_TYPE_MAX - 2); //伴舞、摇旗僵尸不随机刷出
			int y = RANDOM(GRID_NUM_Y);
			addZombie(GRID_NUM_X, y, type);
		}
		make_counter = 0;
	}
}

void Game::addZombieByMyself() {
	//将僵尸产生的计数归零
	make_counter = 0;
	//直接添加一波僵尸
	//摇旗僵尸首先出现
	addZombie(GRID_NUM_X, RANDOM(GRID_NUM_Y), FLAG_ZOMBIE);
	//难度随分数增加，分数越高，僵尸产生速度越快
	for (int i = 0; i <= score / 2000; i++) {
		int type = RANDOM(ZOMBIE_TYPE_MAX - 2); //伴舞、摇旗僵尸不随机刷出
		int y = RANDOM(GRID_NUM_Y);
		addZombie(GRID_NUM_X, y, type);
	}
}

bool Game::moveZombie()
{
	//全部遍历，执行移动
	for (auto& var : zombies) {
		if (var->move(map))
			return true;
		//部分僵尸存在的特殊效果
		var->go(*this);
	}
	return false;
}

void Game::clearZombie()
{
	//删除血量为零的僵尸
	for (auto itList = zombies.begin(); itList != zombies.end(); )
	{
		if ((*itList)->HP <= 0)
		{
			//击杀计分
			score += (*itList)->score;
			map.grid[(*itList)->x][(*itList)->y].delZombie(*itList);
			delete (*itList);
			itList = zombies.erase(itList);
		}
		else {
			//检测是否快被攻破
			if ((*itList)->x == 0) {
				//输出信息，提示玩家
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("【CHEATING】输入'+'号：增加阳光且刷新植物冷却时间", HELP_COLOR);
			}
			else {
				//覆盖信息
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("                                                                    ");
			}
			itList++;
		}
	}
}

Game::~Game()
{
	//清空内存，释放资源
	for (auto& bullet : bullets) {
		delete bullet;
	}
	for (auto& zombie : zombies) {
		delete zombie;
	}
	SetColor(DEFAULT_COLOR);

	//释放音乐资源
	Mix_FreeMusic(winMusic);
	Mix_FreeMusic(loseMusic);
	Mix_FreeMusic(currentMusic);
	Mix_FreeChunk(ClickMusic);
	Mix_FreeChunk(PlantMusic);
	Mix_FreeChunk(DaveVoice);
	Mix_FreeChunk(AlarmMusic);
	Mix_FreeChunk(LaughMusic);
}

void Game::loadHighScore(const string& filename) {
	ifstream ifs(filename, ios::in);
	if(!ifs.is_open()) {
		cerr << "Error: open file " << filename << " failed." << endl;
		return;
	}
	string name;
	int score;
	while (ifs >> name >> score) {
		if(name == currentAccount)
		highScore[name] = score;
	}
	ifs.close();
}

void Game::saveHighScore(const string& filename) {
	//读取文件中所有用户的最高分
	ifstream ifs(filename, ios::in);
	if (!ifs.is_open()) {
		cerr << "Error: open file " << filename << " failed." << endl;
		return;
	}
	string name;
	int score;
	while (ifs >> name >> score) {
		highScore[name] = score;
	}
	ifs.close();
	//当当前用户得分大于历史最高分时，更新
	highScore[currentAccount] = max(highScore[currentAccount], this->score);
    //将所有用户的最高分保存到文件中
	ofstream ofs(filename, ios::out);
	if (!ofs.is_open()) {
		cerr << "Error: open file " << filename << " failed." << endl;
		return;
	}
	//保存
	for (auto& it : highScore) {
		ofs << it.first << " " << it.second << endl;
	}
	ofs.close();
}

void Game::setCurrentAccount(const string& account) {
	currentAccount = account;
}

//注册界面
void Game::Register(const string& filename) {
	displayPVSZ();
	string name;
	string password;
	string password2;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "请输入你要创建的用户名：" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.length() > 0) {  // 如果用户按下退格键，并且用户名不为空
			name.pop_back();  // 删除最后一个字符
			cout << "\b \b";  // 删除控制台上的最后一个字符
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//检查用户名是否为空
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "用户名不能为空！请重新操作！" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}
	//检查用户名是否已存在
	ifstream ifs;
	ifs.open(filename, ios::in);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "文件打开失败" << endl;
		ifs.close();
		return;
	}
	string existName;
	string existPassword;
	while (ifs >> existName && ifs >> existPassword) {
		//如果用户名已存在，提示用户重新输入
		if (name == existName) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
			cerr << "该用户名已存在！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
			system("pause");
			system("cls");
			ifs.close();
			return;
		}
	}
	do {
		displayPVSZ();
		password.clear();
		password2.clear();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cout << "请输入该用户名的密码：" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		while ((ch = _getch()) != '\r') {
			if (ch == 8 && password.length() > 0) {  // 如果用户按下退格键，并且用户名不为空
				password.pop_back();  // 删除最后一个字符
				cout << "\b \b";  // 删除控制台上的最后一个字符
			}
			else if (ch != 8) {
				password.push_back(ch);
				cout << '*';
			}
		}
		cout << endl;
		//检查密码是否为空
		if (password.empty()) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			cerr << "密码不能为空！请重新操作！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			system("pause");
			system("cls");
			return;
		}
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		cout << "请再次输入该用户名的密码：" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		while ((ch = _getch()) != '\r') {
			if (ch == 8 && password2.length() > 0) {  // 如果用户按下退格键，并且用户名不为空
				password2.pop_back();  // 删除最后一个字符
				cout << "\b \b";  // 删除控制台上的最后一个字符
			}
			else if (ch != 8) {
				password2.push_back(ch);
				cout << '*';
			}
		}
		cout << endl;
		//检查密码是否为空
		if (password2.empty()) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "密码不能为空！请重新操作！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
			return;
		}
		//检查两次密码是否相等
		if (password != password2) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "两次输入的密码不一致！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
		}
	} while (password != password2);
	ofstream ofs;
	ofs.open(filename, ios::app);
	//将用户名和密码写入文件
	ofs << name << " " << password << " " << endl;
	ofs.close();
	//将用户名和初始最高分0写入文件
	ofstream ofs2("userHighScore.txt", ios::app);
	ofs2 << name << " " << 0 << " " << endl;
	ofs2.close();
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	cout << "注册成功!" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
	system("pause");
	system("cls");
}

//登录界面
bool Game::LoginIn(const string& filename) {
	displayPVSZ();
	ifstream ifs;
	ifs.open(filename, ios::in);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cerr << "文件打开失败" << endl;
		ifs.close();
		return false;
	}
	string name;
	string password;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "请输入用户名：" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.size() > 0) {
			cout << "\b \b"; //删除控制台上最后一个字符
			name.pop_back(); //删除最后一个字符
			continue;
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//检查用户名是否为空
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "用户名不能为空！请重新操作！" << endl;
		return false;
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
	cout << "请输入密码：" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && password.size() > 0) {
			cout << "\b \b"; //删除控制台上最后一个字符
			password.pop_back(); //删除最后一个字符
			continue;
		}
		else if (ch != 8) {
			password.push_back(ch);
			cout << '*';
		}
	}
	cout << endl;
	if (password.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		cerr << "密码不能为空！请重新操作！" << endl;
		return false;
	}
	string fName;
	string fPassword;
	while (ifs >> fName && ifs >> fPassword && ifs) {
		if (name == fName && password == fPassword) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			cout << "登录成功!" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			system("pause");
			system("cls");
			ifs.close();
			//设置当前账户
			setCurrentAccount(name);
			return true;
		}
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
	cerr << "用户名或密码错误！请重新操作！" << endl;
	ifs.close();
	return false;
}

//重置密码
void Game::ResetPassword(const string& filename) {
	displayPVSZ();
	string name;
	string newPassword;
	string confirmPassword;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "请输入你要重置密码的用户名：" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.size() > 0) {
			cout << "\b \b"; //删除控制台上最后一个字符
			name.pop_back(); //删除最后一个字符
			continue;
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//检查用户名是否为空
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "用户名不能为空！请重新操作！" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}
	// 读取所有的用户信息
	ifstream ifs(filename);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "文件打开失败" << endl;
		ifs.close();
		return;
	}
	vector<pair<string, string>> users;
	string existingName;
	string existingPassword;
	bool found = false;//标记是否找到该用户
	while (ifs >> existingName >> existingPassword) {
		if (name == existingName) {
			do {
				displayPVSZ();
				newPassword.clear();
				confirmPassword.clear();
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
				cout << "请输入新密码：" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
				while ((ch = _getch()) != '\r') {
					if (ch == 8 && newPassword.size() > 0) {
						cout << "\b \b"; //删除控制台上最后一个字符
						newPassword.pop_back(); //删除最后一个字符
						continue;
					}
					else if (ch != 8) {
						newPassword.push_back(ch);
						cout << '*';
					}
				}
				cout << endl;
				//检查密码是否为空
				if (newPassword.empty()) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
					cerr << "密码不能为空！请重新操作！" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
					system("pause");
					system("cls");
					return;
				}
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
				cout << "请再次输入新密码：" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
				while ((ch = _getch()) != '\r') {
					if (ch == 8 && confirmPassword.size() > 0) {
						cout << "\b \b"; //删除控制台上最后一个字符
						confirmPassword.pop_back(); //删除最后一个字符
						continue;
					}
					else if (ch != 8) {
						confirmPassword.push_back(ch);
						cout << '*';
					}
				}
				cout << endl;
				//检查密码是否为空
				if (confirmPassword.empty()) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
					cerr << "密码不能为空！请重新操作！" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
					system("pause");
					system("cls");
					return;
				}
				//检查两次密码是否相同
				if (newPassword != confirmPassword) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
					cerr << "两次输入的密码不一致！请重新输入！" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
					system("pause");
					system("cls");
				}
			} while (newPassword != confirmPassword);
			users.push_back(make_pair(existingName, newPassword));
			found = true;//找到该用户
		}
		else {
			users.push_back(make_pair(existingName, existingPassword));
		}
	}
	ifs.close();

	//如果没有找到该用户，提示用户不存在，返回
	if (!found) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "该用户不存在！请重新操作！" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}

	// 将所有的用户信息写回到文件中
	ofstream ofs(filename);
	if (!ofs.is_open()) {
		cerr << "文件打开失败" << endl;
		ofs.close();
		return;
	}
	for (const auto& user : users) {
		ofs << user.first << ' ' << user.second << '\n';
	}
	ofs.close();
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	cout << "重置密码成功!" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
	system("pause");
	system("cls");
}

//查看历史最高分,仅查看所有用户中排名前三的最高分（若用户数小于3时展示所有）
void Game::showHighScore(const string& filename) {
	ifstream ifs(filename);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cerr << "文件打开失败" << endl;
		system("pause");
		system("cls");
		ifs.close();
		return;
	}
	vector<pair<string, int>> users;
	string name;
	int highScore;
	while (ifs >> name >> highScore) {
		users.push_back(make_pair(name, highScore));
	}
	ifs.close();
	sort(users.begin(), users.end(), [](const pair<string, int>& a, const pair<string, int>& b) {
		return a.second > b.second;
		});
	displayPVSZ();
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "历史最高分排行榜：" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	cout << "排名\t   用户名\t最高分" << endl;
	for (int i = 0; i < 3 && i < users.size(); ++i) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2 + i);
		cout << i + 1 << "\t   " << setw(10) << left << users[i].first << "\t" << users[i].second << endl;
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	system("pause");
	system("cls");
}

//设置游戏声音音量
void Game::setVolume() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("音量设置", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("1.设置背景音乐音量", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("2.设置音效音量", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("3.返回", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			continue;
		}
		switch (userChoice) {
		case 1:
			int bgmVolume;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cout << "请输入背景音乐音量（0-128）：" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cin >> bgmVolume;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "输入错误！请重新输入！" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			if (bgmVolume > 128 || bgmVolume < 0) {
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "输入错误！请重新输入！" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			Mix_VolumeMusic(bgmVolume);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			cout << "设置成功！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			system("pause");
			system("cls");
			return;
		case 2:
			int effectVolume;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cout << "请输入音效音量（0-128）：" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cin >> effectVolume;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "输入错误！请重新输入！" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			if (effectVolume > 128 || effectVolume < 0) {
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "输入错误！请重新输入！" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			Mix_Volume(-1, effectVolume);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			cout << "设置成功！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			system("pause");
			system("cls");
			return;
		case 3:
			system("cls");
			return;
		default:
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			break;
		}
	}

}

//设置菜单显示
void Game::showSettingMenu() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("游戏难度及音量设置", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("1.设置游戏难度", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("2.设置音量", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("3.返回", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			continue;
		}
		switch (userChoice) {
		case 1:
			setDifficulty();
			return;
		case 2:
			setVolume();
			return;
		case 3:
			system("cls");
			return;
		default:
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			break;
		}
	}
}

//开始界面
void Game::Start(const string& filename) {

	while (true)
	{
		system("cls");
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("欢迎来到植物大战僵尸！", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("请选择你的操作：", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("1.登录", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("2.注册", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		PrintWithColor("3.重置用户密码", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		PrintWithColor("4.查看历史最高分", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
		PrintWithColor("5.游戏难度及音量设置", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
		PrintWithColor("6.退出游戏", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
		int choice;
		cin >> choice;
		//检查输入是否合法
		if (cin.fail()) {
			cin.clear();//清除错误标志
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 5);
			system("pause");
			system("cls");
			continue;
		}
		system("cls");
		switch (choice) {
		case 1:
			if (LoginIn(filename)) {
				return;
			}
			else {
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
				system("pause");
				system("cls");
			}
			break;
		case 2:
			Register(filename);
			break;
		case 3:
			ResetPassword(filename);
			break;
		case 4:
			showHighScore("userHighScore.txt");
			break;
		case 5:
			showSettingMenu();
			break;
		case 6:
			exit(0);
			break;
		default:
			displayPVSZ();
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
			PrintWithColor("欢迎来到植物大战僵尸！", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
			PrintWithColor("请选择你的操作：", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
			PrintWithColor("1.登录", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
			PrintWithColor("2.注册", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			PrintWithColor("3.重置用户密码", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			PrintWithColor("4.查看历史最高分", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			PrintWithColor("5.退出游戏", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			cerr << "输入错误！请重新输入！" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 5);
			system("pause");
			system("cls");
			break;
		}
	}
}