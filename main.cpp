#define NOMINMAX //防止windows.h中的min和max宏与std::min和std::max冲突
#include"Game.h"
#include<SDL.h>
#include<SDL_mixer.h>
#include<fstream>
#include<iostream>
#include<limits>
#include<conio.h>
#include<algorithm>
#include<iomanip>
using namespace std;

int main(int argc, char* argv[]) {
    // 初始化SDL
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        // 错误处理
        throw runtime_error("SDL初始化失败");
    }

    // 初始化SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        // 错误处理
        throw runtime_error("SDL_mixer初始化失败");
    }

    //加载音效
    Zombie::eatingSound = Mix_LoadWAV("musics/EatenPlant.mp3");
    Bullet::hitSound = Mix_LoadWAV("musics/BulletHitZombie.mp3");
    CherryBomb::cherryBomb = Mix_LoadWAV("musics/cherryBomb.mp3");
    Jalapeno::jalapeno = Mix_LoadWAV("musics/jalapenoBomb.mp3");
    PotatoMine::potatoMine = Mix_LoadWAV("musics/potatoMineBomb.mp3");
    Box_Zombie::bombSound = Mix_LoadWAV("musics/BoxingZombieBomb.mp3");
    Box_Zombie::laughSound1 = Mix_LoadWAV("musics/laugh1.mp3");
    Box_Zombie::laughSound2 = Mix_LoadWAV("musics/laugh2.mp3");
    Box_Zombie::laughSound3 = Mix_LoadWAV("musics/laugh3.mp3");
    Squash::squashWarning1 = Mix_LoadWAV("musics/squashhmm1.mp3");
    Squash::squashWarning2 = Mix_LoadWAV("musics/squashhmm2.mp3");
    Mix_Chunk* carSound = Mix_LoadWAV("musics/Car.mp3");

    //设置默认音效音量
    Mix_VolumeChunk(Zombie::eatingSound, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Bullet::hitSound, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(CherryBomb::cherryBomb, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Jalapeno::jalapeno, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(PotatoMine::potatoMine, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Box_Zombie::bombSound, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Box_Zombie::laughSound1, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Box_Zombie::laughSound2, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Box_Zombie::laughSound3, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Squash::squashWarning1, MIX_MAX_VOLUME / 8);
    Mix_VolumeChunk(Squash::squashWarning2, MIX_MAX_VOLUME / 8);

    // 加载音乐
    Mix_Music* startMusic = Mix_LoadMUS("musics/Startmusic.mp3");
    Mix_Music* currentMusic = Mix_LoadMUS("musics/level1.mp3");

    //设置默认音乐音量
    Mix_VolumeMusic(MIX_MAX_VOLUME / 4);

    //播放开始界面音乐
    Mix_PlayMusic(startMusic, -1);

    Game myGame;

    //显示开始界面
    myGame.Start("User.txt");

    //登录成功后加载用户最高分
    myGame.loadHighScore("userHighScore.txt");

    //停止播放开始界面音乐
    Mix_HaltMusic();

    // 播放游戏音乐
    Mix_PlayChannel(-1, carSound, 0);
    Mix_PlayMusic(currentMusic, -1);

    myGame.init();
    myGame.loop();

    // 清理
    Mix_FreeChunk(Zombie::eatingSound);
    Mix_FreeChunk(Bullet::hitSound);
    Mix_FreeChunk(CherryBomb::cherryBomb);
    Mix_FreeChunk(Jalapeno::jalapeno);
    Mix_FreeChunk(PotatoMine::potatoMine);
    Mix_FreeChunk(Box_Zombie::bombSound);
    Mix_FreeChunk(Box_Zombie::laughSound1);
    Mix_FreeChunk(Box_Zombie::laughSound2);
    Mix_FreeChunk(Box_Zombie::laughSound3);
    Mix_FreeChunk(Squash::squashWarning1);
    Mix_FreeChunk(Squash::squashWarning2);
    Mix_FreeChunk(carSound);
    Mix_FreeMusic(startMusic);
    Mix_FreeMusic(currentMusic);
    Mix_CloseAudio();
    SDL_Quit();

    //保存用户最高分
    myGame.saveHighScore("userHighScore.txt");

    return 0;
}