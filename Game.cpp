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
	//���̰���ʶ��
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
			case 72://��
				if (y > 0)
					y--;
				break;
			case 80://��
				if (y < GRID_NUM_Y - 1)
					y++;
				break;
			case 75://��
				if (x > 0)
					x--;
				break;
			case 77://��
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
			//���ﲻ��break�����������رս����˳�
		case 27://ESC
			closeFocus();
			break;
			//�ո����ͣ
		case ' ':
			pause();
			break;
		case'q':case'Q':
			//��ͣ��Ϸ��ѯ���Ƿ��˳�
			temp = state;
			state = PAUSE;
			refreshHelpInfo = true;
			helpInfo();

			Goto_XY(2 + 10, STORE_UP_BOUND);
			PrintWithColor("���Ƿ�ȷ��Ҫ�˳���Ϸ�����ȷ�����밴�»س��������ȡ�����밴��Esc��", HELP_COLOR);
			while (true)
			{
				ch = _getch();
				switch (ch)
				{
				case '\r':
					//�˳���Ϸ
					gameover();
					break;
				case 27:
					//ȡ���˳�
					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("                                                                    ");
					//�ָ���Ϸ״̬
					state = temp;
					refreshHelpInfo = true;
					helpInfo();
					break;
				default:
					//����������������������
					break;
				}
				if (ch == '\r' || ch == 27)
					break;
			}
			break;

		case '+'://����ָ����Ǯ�������ڲ��ԣ�
			store.addSun(1000);
		case '*'://����ָ��˲�����������ȴ�����ڲ��ԣ�
			store.renew(); 
			break;
        default:
			break;
		}
	}
	//����ʶ�����
}

void Game::shopping()
{
	//���̰���ʶ��
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
			case 72://��
				if (y > 0)
					y--;
				break;
			case 80://��
				if (y < GRID_NUM_Y - 1)
					y++;
				break;
			case 75://��
				if (x > 0)
					x--;
				break;
			case 77://��
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
			if (store.pay(planting_type, x, y, map)) { //�������ɹ�
				//������ֲ��Ч
				Mix_PlayChannel(-1, PlantMusic, 0); 
				store.plants[planting_type].setUnSelect();
				closeFocus();
			}
			break;
			//�ո����ͣ
		case ' ':
			pause();
			break;
		case'q':case'Q':
			//��ͣ��Ϸ��ѯ���Ƿ��˳�
			temp = state;
			state = PAUSE;
			refreshHelpInfo = true;
			helpInfo();

			Goto_XY(2 + 10, STORE_UP_BOUND);
			PrintWithColor("���Ƿ�ȷ��Ҫ�˳���Ϸ�����ȷ�����밴�»س��������ȡ�����밴��Esc��", HELP_COLOR);
			while (true)
			{
				ch = _getch();
				switch (ch)
				{
				case '\r':
					//�˳���Ϸ
					gameover();
					break;
				case 27:
					//ȡ���˳�
					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("                                                                    ");
					//�ָ���Ϸ״̬
					state = temp;
					refreshHelpInfo = true;
					helpInfo();
					break;
				default:
					//����������������������
					break;
				}
				if (ch == '\r' || ch == 27)
					break;
			}
			break;

		case '+'://����ָ����Ǯ�������ڲ��ԣ�
			store.addSun(1000);
		case '*'://����ָ��˲�����������ȴ�����ڲ��ԣ�
			store.renew(); 
			break;

			/*����ֲ��İ���ʶ��*/
		case 'a':case'A':
			//������Ч
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
				PrintWithColor("��ֲ����δ������", HELP_COLOR);
			}
			break;
		case 'b':case'B':
			//������Ч
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
				PrintWithColor("��ֲ����δ������", HELP_COLOR);
			}
			break;
		case 'c':case 'C':
			//������Ч
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
				PrintWithColor("��ֲ����δ������", HELP_COLOR);
			}
			break;
		default:
			if (ch >= '1' && ch <= '9') {
				//������Ч
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
						PrintWithColor("��ֲ����δ������", HELP_COLOR);
					}
				}
			}
			break;
		}
	}
	//����ʶ�����
}

void Game::delPlant(int ix, int iy)
{
	if (ix<0 || ix>GRID_NUM_X || iy<0 || iy>GRID_NUM_Y - 1) return;
	map.grid[ix][iy].delPlant();
}

void Game::refreshScore()
{
	//�����
	Goto_XY(LINE_X1 + 4, STORE_UP_BOUND + 1);
	string str(15, ' ');
	cout << str;
	//����÷�
	Goto_XY(LINE_X1 + 4, STORE_UP_BOUND + 1);
	cout << "[������" << score << "]";
	//�����ǰ����
	Goto_XY(LINE_X3 + 4, STORE_UP_BOUND + 1);
	cout << "[Ŀ�꣺" << targetScore << "]";
	//���Ŀ��ؿ�
	Goto_XY(LINE_X4 + 4, STORE_UP_BOUND + 1);
	cout << "[�ؿ���" << level << "]";
	//���ʣ��ʱ��
	int leftTime = maxDuration - difftime(time(nullptr), startTime);
	Goto_XY(LINE_X4 - 17, STORE_UP_BOUND + 1);
	cout << "[ʱ�䣺" << leftTime << "]  ";
}

void Game::calScore()
{
	score_counter++;
	if (score_counter >= score_speed) {
		score_counter = 0;
		score++;
		//��鲢������ߵ÷�
		highScore[currentAccount] = max(highScore[currentAccount], score);
		refreshScore();
	}
}

void Game::helpInfo()
{
	if (refreshHelpInfo) {
		//���
		string str(WINDOWS_WIDTH, ' ');
		Goto_XY(0, HELP_INFO_Y);
		cout << str;
		Goto_XY(0, HELP_INFO_Y);
		if (state == NORMAL) {
			PrintWithColor(" <��Ϸ��> ", STATE_COLOR);
			PrintWithColor(" ��ֲ��\"1-9\"���ּ���\"a-c\"�� | ������\"x\"�� | �����һ����ʬ��\"z\"�� | ��ͣ���ո�� | �˳���Ϸ��\"q\"��", HELP_COLOR);
		}
		else if (state == STORE) {
			PrintWithColor(" <��ֲ> ", STATE_COLOR);
			PrintWithColor(" ѡ��\"1-9\"���ּ���\"a-c\"�� | ȷ�ϣ��س��� | ���أ�Esc�� | ��ͣ���ո�� | �˳���Ϸ��\"q\"��", HELP_COLOR);
		}
		else if (state == SHOVEL) {
			PrintWithColor(" <����> ", STATE_COLOR);
			PrintWithColor(" ȷ�ϣ��س��� | ���أ�Esc�� | ��ͣ���ո�� | �˳���Ϸ��\"q\"��", HELP_COLOR);
		}
		else if (state == PAUSE) {
			PrintWithColor(" <��ͣ��> ", STATE_COLOR);
			PrintWithColor(" �˳���ͣ���ո�� | �˳���Ϸ��\"q\"��", HELP_COLOR);
		}

		refreshHelpInfo = false;
	}
}

void Game::checkGameState() {
	time_t now = time(nullptr);
	//����Ƿ������������
	if (score >= targetScore && difftime(now, startTime) == maxDuration) {
		//������һ��
		level++;
		//���ű�������
		playMusic();
		if (level > levelGoals.size()) {
			//ͨ��
	        state = OVER;
			//����ʤ������
			//ֹͣ��ǰ����
			Mix_HaltMusic();
			//����ʤ������һ��
			Mix_PlayMusic(winMusic, 0);
			system("cls");
			Goto_XY(WINDOWS_WIDTH / 2 - 6, WINDOWS_HEIGHT / 2 - 1);
			PrintWithColor("����ϲͨ�ء�", STATE_COLOR);
			//�������
			Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 2);
			cout << "��ķ����ǣ� " << score << endl;
			Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 3);
			cout << "�����߷����ǣ�" << highScore[currentAccount] << endl;
			Goto_XY(0, WINDOWS_HEIGHT - 1);
			SetColor(HELP_COLOR);
			system("pause");
		}
		else {
			targetScore = levelGoals[level - 1].first;
			maxDuration = levelGoals[level - 1].second;
			//�����¹ؿ��Ŀ�ʼʱ��
			startTime = time(nullptr);
			//��ʾ�û�ͨ���˹ؿ�
			Goto_XY(0, HELP_INFO_Y - 1);
			PrintWithColor("����ϲ���أ�������һ�ء�", STATE_COLOR);
			//��ս�ʬ��ֲ�ﱣ�ֲ���
			for (auto itList = zombies.begin(); itList != zombies.end(); )
			{
				map.grid[(*itList)->x][(*itList)->y].delZombie(*itList);
				delete (*itList);
				itList = zombies.erase(itList);
			}
			//�ȴ�һ��ʱ���ɾ����ϲ��Ϣ
			Sleep(2000);
			Goto_XY(0, HELP_INFO_Y - 1);
			string str(WINDOWS_WIDTH, ' ');
			cout << str;
		}
	}
	else if (difftime(now, startTime) > maxDuration) {
		//ʱ�䵽����Ϸ����
		gameover();
	}	
}

void Game::setDifficulty() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cout << "��ѡ���Ѷȣ�";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		cout << "1.��";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cout << "2.��ͨ";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		cout << "3.����";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		cout << "4.����";
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "�����������������";
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
			cout << "���óɹ���";
			system("pause");
			system("cls");
			return;
		case 2:
			levelGoals = { {700, 60}, {3000, 92}, {6000, 92}, {15000, 92}, {30000, 122}, {50000, 122} };
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cout << "���óɹ���";
			system("pause");
			system("cls");
			return;
		case 3:
			levelGoals = { {800, 60}, {4000, 92}, {8000, 92}, {20000, 92}, {40000, 122}, {70000, 122} };
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cout << "���óɹ���";
			system("pause");
			system("cls");
			return;
		case 4:
			system("cls");
			return;
		default:
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "�����������������";
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
			return;
		}
	}	
}

void Game::playMusic() {
	//ֹͣ��ǰ����
	Mix_HaltMusic();
	//���ݹؿ����ű�������
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
	//���ű�������
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
			case ' '://�ո������ͣ
				state = temp; 
				startTime += difftime(time(nullptr), pauseTime);
				refreshHelpInfo = true;
				helpInfo();
				return;
				break;
			case'q':case'Q':
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("���Ƿ�ȷ��Ҫ�˳���Ϸ�����ȷ�����밴�»س��������ȡ�����밴��Esc��", HELP_COLOR);
				while (true)
				{
					ch = _getch();
					switch (ch)
					{
					case '\r':
						//�˳���Ϸ
						gameover();
						break;
					case 27:
						//ȡ���˳�
						Goto_XY(2 + 10, STORE_UP_BOUND);
						PrintWithColor("                                                                    ");
						break;
					default:
						//����������������������
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
	//����ʧ������
	Mix_HaltMusic();
	Mix_PlayMusic(loseMusic, 0);
	Goto_XY(WINDOWS_WIDTH / 2 - 6, WINDOWS_HEIGHT / 2 - 1);
	PrintWithColor("����Ϸ������", STATE_COLOR);
	//�������
	Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 2);
	cout << "��ķ����ǣ� " << score << endl;
	Goto_XY(WINDOWS_WIDTH / 2 - 8, WINDOWS_HEIGHT / 2 + 3);
	cout << "�����߷����ǣ�" << highScore[currentAccount] << endl;
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
	//��ʼ����ͼ���̵�
	map.init();
	store.init();

	//��ʼ������
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
	level = 1; //��ʼ�ؿ�Ϊ1
	//�趨�ؿ�����ֲ������
	levelAvailablePlants = { 
	{1, {0, 1, 2, 3}},
	{2, {0, 1, 2, 3, 4, 5, 6, 7}},
	{3, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}},
	{4, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
	{5, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}},
	{6, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}} };
	//�趨��Ϸ�ؿ���ÿ�ص�Ŀ���������Ϸʱ��
	if (levelGoals.size() == 0){
		levelGoals = { {700, 60}, {3000, 92}, {6000, 92}, {15000, 92}, {30000, 122}, {50000, 122} };//Ĭ��Ϊ��ͨ�Ѷ�
	}
	targetScore = levelGoals[0].first;
	maxDuration = levelGoals[0].second;
	startTime = time(nullptr);
	refreshScore();

	//13�����һ�ν�ʬ
	make_speed = 130 * 1000 / 10 / SLEEP_TIME;
	make_counter = 0;
}

void Game::loop()
{
	//TEST:��һ��ʼ�������ض��Ľ�ʬ�����ڵ��ԣ�
	//addZombie(GRID_NUM_X, 0, POLE_ZOMBIE);	
	//addZombie(GRID_NUM_X, 2, DANCING_ZOMBIE);
	addZombie(GRID_NUM_X, 2, BOX_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, NEWSPAPER_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, FLAG_ZOMBIE);
	//addZombie(GRID_NUM_X, 2, ZOMBIE);
	int temp = 0;
	while (true) {
		//�����Ϸ״̬
		checkGameState();
		if (state == OVER) {
			break;
		}
		else if (state == NORMAL) {
			vector<int> availablePlants = levelAvailablePlants[level];
			//���̰���ʶ��
	        char ch;
			if (_kbhit())
			{
				ch = _getch();
				switch (ch)
				{
				case 'a':case 'A':
					//������Ч
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 9;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//�����̵�ѡ��
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("��ֲ����δ������", HELP_COLOR);
					}
					break;
				case 'b':case 'B':
					//������Ч
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 10;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//�����̵�ѡ��
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("��ֲ����δ������", HELP_COLOR);
					}
					break;
				case 'c':case 'C':
					//������Ч
					Mix_PlayChannel(-1, ClickMusic, 0);
					planting_type = 11;
					if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
					{
						store.plants[planting_type].setSelect();
						Goto_XY(30, STORE_UP_BOUND + 6);
						cout << "                         ";
						//�����̵�ѡ��
						state = STORE;
						refreshHelpInfo = true;
						openFocus();
					}
					else {
						Goto_XY(30, STORE_UP_BOUND + 6);
						PrintWithColor("��ֲ����δ������", HELP_COLOR);
					}
					break;
				//��x/X�������ֲ��
				case 'x':case 'X':
					state = SHOVEL; 
					refreshHelpInfo = true;
					openFocus();
					break;
				//��z/Z�����һ����ʬ
				case 'z':case 'Z':
					addZombieByMyself();
					break;
				//�ո����ͣ
				case ' ':
					pause();
					break;
				case'q':case'Q':
					//��ͣ��Ϸ��ѯ���Ƿ��˳�
					temp = state;
					state = PAUSE; 
					refreshHelpInfo = true;
					helpInfo();

					Goto_XY(2 + 10, STORE_UP_BOUND);
					PrintWithColor("���Ƿ�ȷ��Ҫ�˳���Ϸ�����ȷ�����밴�»س��������ȡ�����밴��Esc��", HELP_COLOR);
					while (true)
					{
						ch = _getch();
						switch (ch)
						{
						case '\r':
							//�˳���Ϸ
							gameover();
							break;
						case 27:
							//ȡ���˳�
							Goto_XY(2 + 10, STORE_UP_BOUND);
							PrintWithColor("                                                                    ");
							//�ָ���Ϸ״̬
							state = temp;
							refreshHelpInfo = true;
							helpInfo();
							break;
						default:
							//����������������������
							break;
						}
						if(ch == '\r' || ch == 27)
							break;
					}
					break;

				case '+'://����ָ����Ǯ�������ڲ��ԣ�
					store.addSun(1000);
				case '*'://����ָ��˲�����������ȴ�����ڲ��ԣ�
					store.renew(); break;

				default:
					//ֱ�Ӱ������ּ������
					if (ch >= '1' && ch <= '9') {
						//������Ч
						Mix_PlayChannel(-1, ClickMusic, 0);
						planting_type = ch - '0' - 1;
						if (find(availablePlants.begin(), availablePlants.end(), planting_type) != availablePlants.end())
						{
							store.plants[planting_type].setSelect();
							Goto_XY(30, STORE_UP_BOUND + 6);
							cout << "                         ";
							//�����̵�ѡ��
							state = STORE;
							refreshHelpInfo = true;
							openFocus();
						}
						else {
							Goto_XY(30, STORE_UP_BOUND + 6);
							PrintWithColor("��ֲ����δ������", HELP_COLOR);
						}
						break;
					}
				}
			}
			//����ʶ�����
		}
		else if (state == STORE) {
			shopping();
		}
		else if (state == SHOVEL) {
			weeding();
		}

		//ִ�н�ʬ�����߼�
		makeZombies();

		//������ֲ��
		if (!map.travGrid(*this)) {
			break;
		}

		//��ʬ���� - �ж�
		if (moveZombie()) {
			gameover();//��ʬ���ƣ���Ϸ����
			break;
		}

		//�̵�����
		store.run();

		//�ӵ��ƶ�
		moveBullet();

		//�����Ч��ʬ
		clearZombie();

		//ˢ����ʾ
		map.refresh();//�ݵص�ÿ������
		printBullet();//�ӵ�

		//�Ʒ��߼�
		calScore();

		//�·�������Ϣ�������ʾ
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
	//ȫ��������ִ���ƶ�
	for (auto& var : bullets) {
		var->move(map);
	}
	//ɾ����Ч���ӵ�
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
	//ȫ��������ִ�����
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
    //���ݽ�ʬ���Ͳ�ͬ��������ͬ�Ľ�ʬ����
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
		//ҡ�콩ʬ���ȳ���
		addZombie(GRID_NUM_X, RANDOM(GRID_NUM_Y), FLAG_ZOMBIE);
		//�Ѷ���������ӣ�����Խ�ߣ���ʬ�����ٶ�Խ��
		for (int i = 0; i <= score / 2000; i++) {
			int type = RANDOM(ZOMBIE_TYPE_MAX - 2); //���衢ҡ�콩ʬ�����ˢ��
			int y = RANDOM(GRID_NUM_Y);
			addZombie(GRID_NUM_X, y, type);
		}
		make_counter = 0;
	}
}

void Game::addZombieByMyself() {
	//����ʬ�����ļ�������
	make_counter = 0;
	//ֱ�����һ����ʬ
	//ҡ�콩ʬ���ȳ���
	addZombie(GRID_NUM_X, RANDOM(GRID_NUM_Y), FLAG_ZOMBIE);
	//�Ѷ���������ӣ�����Խ�ߣ���ʬ�����ٶ�Խ��
	for (int i = 0; i <= score / 2000; i++) {
		int type = RANDOM(ZOMBIE_TYPE_MAX - 2); //���衢ҡ�콩ʬ�����ˢ��
		int y = RANDOM(GRID_NUM_Y);
		addZombie(GRID_NUM_X, y, type);
	}
}

bool Game::moveZombie()
{
	//ȫ��������ִ���ƶ�
	for (auto& var : zombies) {
		if (var->move(map))
			return true;
		//���ֽ�ʬ���ڵ�����Ч��
		var->go(*this);
	}
	return false;
}

void Game::clearZombie()
{
	//ɾ��Ѫ��Ϊ��Ľ�ʬ
	for (auto itList = zombies.begin(); itList != zombies.end(); )
	{
		if ((*itList)->HP <= 0)
		{
			//��ɱ�Ʒ�
			score += (*itList)->score;
			map.grid[(*itList)->x][(*itList)->y].delZombie(*itList);
			delete (*itList);
			itList = zombies.erase(itList);
		}
		else {
			//����Ƿ�챻����
			if ((*itList)->x == 0) {
				//�����Ϣ����ʾ���
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("��CHEATING������'+'�ţ�����������ˢ��ֲ����ȴʱ��", HELP_COLOR);
			}
			else {
				//������Ϣ
				Goto_XY(2 + 10, STORE_UP_BOUND);
				PrintWithColor("                                                                    ");
			}
			itList++;
		}
	}
}

Game::~Game()
{
	//����ڴ棬�ͷ���Դ
	for (auto& bullet : bullets) {
		delete bullet;
	}
	for (auto& zombie : zombies) {
		delete zombie;
	}
	SetColor(DEFAULT_COLOR);

	//�ͷ�������Դ
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
	//��ȡ�ļ��������û�����߷�
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
	//����ǰ�û��÷ִ�����ʷ��߷�ʱ������
	highScore[currentAccount] = max(highScore[currentAccount], this->score);
    //�������û�����߷ֱ��浽�ļ���
	ofstream ofs(filename, ios::out);
	if (!ofs.is_open()) {
		cerr << "Error: open file " << filename << " failed." << endl;
		return;
	}
	//����
	for (auto& it : highScore) {
		ofs << it.first << " " << it.second << endl;
	}
	ofs.close();
}

void Game::setCurrentAccount(const string& account) {
	currentAccount = account;
}

//ע�����
void Game::Register(const string& filename) {
	displayPVSZ();
	string name;
	string password;
	string password2;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "��������Ҫ�������û�����" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.length() > 0) {  // ����û������˸���������û�����Ϊ��
			name.pop_back();  // ɾ�����һ���ַ�
			cout << "\b \b";  // ɾ������̨�ϵ����һ���ַ�
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//����û����Ƿ�Ϊ��
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "�û�������Ϊ�գ������²�����" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}
	//����û����Ƿ��Ѵ���
	ifstream ifs;
	ifs.open(filename, ios::in);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "�ļ���ʧ��" << endl;
		ifs.close();
		return;
	}
	string existName;
	string existPassword;
	while (ifs >> existName && ifs >> existPassword) {
		//����û����Ѵ��ڣ���ʾ�û���������
		if (name == existName) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
			cerr << "���û����Ѵ��ڣ����������룡" << endl;
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
		cout << "��������û��������룺" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		while ((ch = _getch()) != '\r') {
			if (ch == 8 && password.length() > 0) {  // ����û������˸���������û�����Ϊ��
				password.pop_back();  // ɾ�����һ���ַ�
				cout << "\b \b";  // ɾ������̨�ϵ����һ���ַ�
			}
			else if (ch != 8) {
				password.push_back(ch);
				cout << '*';
			}
		}
		cout << endl;
		//��������Ƿ�Ϊ��
		if (password.empty()) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			cerr << "���벻��Ϊ�գ������²�����" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			system("pause");
			system("cls");
			return;
		}
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		cout << "���ٴ�������û��������룺" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		while ((ch = _getch()) != '\r') {
			if (ch == 8 && password2.length() > 0) {  // ����û������˸���������û�����Ϊ��
				password2.pop_back();  // ɾ�����һ���ַ�
				cout << "\b \b";  // ɾ������̨�ϵ����һ���ַ�
			}
			else if (ch != 8) {
				password2.push_back(ch);
				cout << '*';
			}
		}
		cout << endl;
		//��������Ƿ�Ϊ��
		if (password2.empty()) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "���벻��Ϊ�գ������²�����" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
			return;
		}
		//������������Ƿ����
		if (password != password2) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cerr << "������������벻һ�£����������룡" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			system("pause");
			system("cls");
		}
	} while (password != password2);
	ofstream ofs;
	ofs.open(filename, ios::app);
	//���û���������д���ļ�
	ofs << name << " " << password << " " << endl;
	ofs.close();
	//���û����ͳ�ʼ��߷�0д���ļ�
	ofstream ofs2("userHighScore.txt", ios::app);
	ofs2 << name << " " << 0 << " " << endl;
	ofs2.close();
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	cout << "ע��ɹ�!" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
	system("pause");
	system("cls");
}

//��¼����
bool Game::LoginIn(const string& filename) {
	displayPVSZ();
	ifstream ifs;
	ifs.open(filename, ios::in);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cerr << "�ļ���ʧ��" << endl;
		ifs.close();
		return false;
	}
	string name;
	string password;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "�������û�����" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.size() > 0) {
			cout << "\b \b"; //ɾ������̨�����һ���ַ�
			name.pop_back(); //ɾ�����һ���ַ�
			continue;
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//����û����Ƿ�Ϊ��
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "�û�������Ϊ�գ������²�����" << endl;
		return false;
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
	cout << "���������룺" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && password.size() > 0) {
			cout << "\b \b"; //ɾ������̨�����һ���ַ�
			password.pop_back(); //ɾ�����һ���ַ�
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
		cerr << "���벻��Ϊ�գ������²�����" << endl;
		return false;
	}
	string fName;
	string fPassword;
	while (ifs >> fName && ifs >> fPassword && ifs) {
		if (name == fName && password == fPassword) {
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			cout << "��¼�ɹ�!" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			system("pause");
			system("cls");
			ifs.close();
			//���õ�ǰ�˻�
			setCurrentAccount(name);
			return true;
		}
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
	cerr << "�û�����������������²�����" << endl;
	ifs.close();
	return false;
}

//��������
void Game::ResetPassword(const string& filename) {
	displayPVSZ();
	string name;
	string newPassword;
	string confirmPassword;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
	cout << "��������Ҫ����������û�����" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	char ch;
	while ((ch = _getch()) != '\r') {
		if (ch == 8 && name.size() > 0) {
			cout << "\b \b"; //ɾ������̨�����һ���ַ�
			name.pop_back(); //ɾ�����һ���ַ�
			continue;
		}
		else if (ch != 8) {
			name.push_back(ch);
			cout << ch;
		}
	}
	cout << endl;
	//����û����Ƿ�Ϊ��
	if (name.empty()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "�û�������Ϊ�գ������²�����" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}
	// ��ȡ���е��û���Ϣ
	ifstream ifs(filename);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "�ļ���ʧ��" << endl;
		ifs.close();
		return;
	}
	vector<pair<string, string>> users;
	string existingName;
	string existingPassword;
	bool found = false;//����Ƿ��ҵ����û�
	while (ifs >> existingName >> existingPassword) {
		if (name == existingName) {
			do {
				displayPVSZ();
				newPassword.clear();
				confirmPassword.clear();
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
				cout << "�����������룺" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
				while ((ch = _getch()) != '\r') {
					if (ch == 8 && newPassword.size() > 0) {
						cout << "\b \b"; //ɾ������̨�����һ���ַ�
						newPassword.pop_back(); //ɾ�����һ���ַ�
						continue;
					}
					else if (ch != 8) {
						newPassword.push_back(ch);
						cout << '*';
					}
				}
				cout << endl;
				//��������Ƿ�Ϊ��
				if (newPassword.empty()) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
					cerr << "���벻��Ϊ�գ������²�����" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
					system("pause");
					system("cls");
					return;
				}
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
				cout << "���ٴ����������룺" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
				while ((ch = _getch()) != '\r') {
					if (ch == 8 && confirmPassword.size() > 0) {
						cout << "\b \b"; //ɾ������̨�����һ���ַ�
						confirmPassword.pop_back(); //ɾ�����һ���ַ�
						continue;
					}
					else if (ch != 8) {
						confirmPassword.push_back(ch);
						cout << '*';
					}
				}
				cout << endl;
				//��������Ƿ�Ϊ��
				if (confirmPassword.empty()) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
					cerr << "���벻��Ϊ�գ������²�����" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
					system("pause");
					system("cls");
					return;
				}
				//������������Ƿ���ͬ
				if (newPassword != confirmPassword) {
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
					cerr << "������������벻һ�£����������룡" << endl;
					Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
					system("pause");
					system("cls");
				}
			} while (newPassword != confirmPassword);
			users.push_back(make_pair(existingName, newPassword));
			found = true;//�ҵ����û�
		}
		else {
			users.push_back(make_pair(existingName, existingPassword));
		}
	}
	ifs.close();

	//���û���ҵ����û�����ʾ�û������ڣ�����
	if (!found) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		cerr << "���û������ڣ������²�����" << endl;
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		system("pause");
		system("cls");
		return;
	}

	// �����е��û���Ϣд�ص��ļ���
	ofstream ofs(filename);
	if (!ofs.is_open()) {
		cerr << "�ļ���ʧ��" << endl;
		ofs.close();
		return;
	}
	for (const auto& user : users) {
		ofs << user.first << ' ' << user.second << '\n';
	}
	ofs.close();
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	cout << "��������ɹ�!" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
	system("pause");
	system("cls");
}

//�鿴��ʷ��߷�,���鿴�����û�������ǰ������߷֣����û���С��3ʱչʾ���У�
void Game::showHighScore(const string& filename) {
	ifstream ifs(filename);
	if (!ifs.is_open()) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		cerr << "�ļ���ʧ��" << endl;
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
	cout << "��ʷ��߷����а�" << endl;
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
	cout << "����\t   �û���\t��߷�" << endl;
	for (int i = 0; i < 3 && i < users.size(); ++i) {
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2 + i);
		cout << i + 1 << "\t   " << setw(10) << left << users[i].first << "\t" << users[i].second << endl;
	}
	Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
	system("pause");
	system("cls");
}

//������Ϸ��������
void Game::setVolume() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("��������", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("1.���ñ�����������", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("2.������Ч����", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("3.����", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "����������������룡" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			continue;
		}
		switch (userChoice) {
		case 1:
			int bgmVolume;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cout << "�����뱳������������0-128����" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cin >> bgmVolume;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "����������������룡" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			if (bgmVolume > 128 || bgmVolume < 0) {
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "����������������룡" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			Mix_VolumeMusic(bgmVolume);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			cout << "���óɹ���" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			system("pause");
			system("cls");
			return;
		case 2:
			int effectVolume;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cout << "��������Ч������0-128����" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			cin >> effectVolume;
			if (cin.fail()) {
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "����������������룡" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			if (effectVolume > 128 || effectVolume < 0) {
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
				cerr << "����������������룡" << endl;
				Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
				system("pause");
				system("cls");
				break;
			}
			Mix_Volume(-1, effectVolume);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			cout << "���óɹ���" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			system("pause");
			system("cls");
			return;
		case 3:
			system("cls");
			return;
		default:
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "����������������룡" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			break;
		}
	}

}

//���ò˵���ʾ
void Game::showSettingMenu() {
	while (true)
	{
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("��Ϸ�Ѷȼ���������", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("1.������Ϸ�Ѷ�", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("2.��������", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("3.����", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		int userChoice;
		cin >> userChoice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			cerr << "����������������룡" << endl;
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
			cerr << "����������������룡" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			system("pause");
			system("cls");
			break;
		}
	}
}

//��ʼ����
void Game::Start(const string& filename) {

	while (true)
	{
		system("cls");
		displayPVSZ();
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 4);
		PrintWithColor("��ӭ����ֲ���ս��ʬ��", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
		PrintWithColor("��ѡ����Ĳ�����", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
		PrintWithColor("1.��¼", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
		PrintWithColor("2.ע��", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
		PrintWithColor("3.�����û�����", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
		PrintWithColor("4.�鿴��ʷ��߷�", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
		PrintWithColor("5.��Ϸ�Ѷȼ���������", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
		PrintWithColor("6.�˳���Ϸ", DEFAULT_COLOR);
		Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
		int choice;
		cin >> choice;
		//��������Ƿ�Ϸ�
		if (cin.fail()) {
			cin.clear();//��������־
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			cerr << "����������������룡" << endl;
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
			PrintWithColor("��ӭ����ֲ���ս��ʬ��", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 3);
			PrintWithColor("��ѡ����Ĳ�����", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 2);
			PrintWithColor("1.��¼", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 - 1);
			PrintWithColor("2.ע��", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2);
			PrintWithColor("3.�����û�����", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 1);
			PrintWithColor("4.�鿴��ʷ��߷�", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 2);
			PrintWithColor("5.�˳���Ϸ", DEFAULT_COLOR);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 3);
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 4);
			cerr << "����������������룡" << endl;
			Goto_XY(WINDOWS_WIDTH / 2 + 18, WINDOWS_HEIGHT / 2 + 5);
			system("pause");
			system("cls");
			break;
		}
	}
}