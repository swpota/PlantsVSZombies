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

//ǰ������
class Plant;

#ifndef GAME_H
#define GAME_H

class Game final {
	Map map;
	Store store;

	//�ӵ����У��㶹���ֵȷ����ģ�
	list<Bullet*> bullets;
	//��ʬ����
	list<Zombie*> zombies;

	//��ǰ����״̬����ѡ���ֵ�����������ö��������
	int state;
	//״̬������������״̬������ֲ���Ϸ��ͣ����Ϸ����
	enum GAME_STATE { NORMAL, STORE, SHOVEL, PAUSE, OVER };
	//ѡ�����ֲ����
	void weeding();
	//ѡ����ֲ����
	void shopping();

	//��ǰ����λ��
	int x, y;
	//��ǰ׼����ֲ��ֲ������
	int planting_type;
	//����ѡ�񽹵�
	void openFocus();
	//�ر�ѡ�񽹵�
	void closeFocus();

	//����ĳ��λ�õ�ֲ��
	void delPlant(int ix, int iy);
	//�ӵ��ƶ�
	void moveBullet();
	//�ӵ�����ʾ���
	void printBullet();
	//ָ��λ������ĳ�����͵Ľ�ʬ
	void addZombie(int x, int y, int type);
	//��ʬ�����ٶ�
	int make_speed;
	int make_counter;
	//��ʬ�����߼�
	void makeZombies();
	//��ʬ�ƶ�
	bool moveZombie();
	//�����Ч��ʬ��Ѫ��<=0��
	void clearZombie();
	//������ӽ�ʬ
	void addZombieByMyself();

	//�÷�
	int score;
	//Ŀ��÷�
	int targetScore;
	//ÿ���û���Ӧÿ���û�����ߵ÷�
	std::map<string, int> highScore;
	//ÿ���ؿ�����ʹ�õ�ֲ������
	std::map<int, vector<int>> levelAvailablePlants;
	//��ǰ�ؿ�
	int level;
	//ÿ�ص�Ŀ���������Ϸʱ��
	vector<pair<int, int>> levelGoals;
	//��Ϸ��ʼʱ��
	time_t startTime;
	//��ͣʱ��
	time_t pauseTime;
	//������Ϸ���ʱ��
	int maxDuration;
	//��Ϸ��ǰ��¼���˻�
	string currentAccount;
	//ˢ�µ÷���ʾ
	void refreshScore();
	//���������ٶ�
	int score_speed;
	int score_counter;
	//�Ʒ�
	void calScore();

	//������Ϣ�����
	void helpInfo();
	//ָʾ�Ƿ���Ҫ���°�����Ϣ��״̬�л�����Ҫ��
	bool refreshHelpInfo;

	//����Ƿ���Խ�����һ��
	void checkGameState();
	//���ݹؿ����ű�������
	void playMusic();

	//��ͣ
	void pause();
	//��Ϸ����
	void gameover();
	//ʤ������
	Mix_Music* winMusic;
	//ʧ������
	Mix_Music* loseMusic;
	//ѡ�п�Ƭ��Ч
	Mix_Chunk* ClickMusic;
	//��ֲֲ����Ч
	Mix_Chunk* PlantMusic;
	//����˵����Ч
	Mix_Chunk* DaveVoice;
	//������Ч
	Mix_Chunk* AlarmMusic;
	//Ц����Ч
	Mix_Chunk* LaughMusic;
	//��ǰ�ؿ��ı�������
	Mix_Music* currentMusic;

public:
	Game();


	//��Ϸ������ʼ��
	void init();
	//��Ϸ��ѭ��
	void loop();

	//�ӵ�����������һ���ӵ�
	void addBullet(Bullet* p);

	//��ʾPlantsVSZombies
	void displayPVSZ();

	//�����û���߷�
	void loadHighScore(const string& filename);

	//�����û���߷�
	void saveHighScore(const string& filename);

	//���õ�ǰ��¼���˻�
	void setCurrentAccount(const string& account);

	//�����Ѷ�
	void setDifficulty();

	//ע�����
	void Register(const string& filename);

	//��¼����
	bool LoginIn(const string& filename);

	//��������
	void ResetPassword(const string& filename);

	//�鿴��ʷ��߷�,���鿴�����û�������ǰ������߷֣����û���С��3ʱչʾ���У�
	void showHighScore(const string& filename);

	//������Ϸ��������
	void setVolume();

	//���ò˵���ʾ
	void showSettingMenu();

	//��ʼ����
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