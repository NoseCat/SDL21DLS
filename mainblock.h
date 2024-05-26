#pragma once
#ifndef _MAIN_BLOCK_
#define _MAIN_BLOCK_

enum GameStates { MENU, LEVELSELECT, GAME, PAUSED, EXIT, };
extern int GameState;

//Вызывается на запуске программы
void globalOnStart();

void onLevelStart(const char* levelname);

//Вызывается каждый кадр
void levelSelectEachFrame();

void mainMenuEachFrame();

void eachFrame(float delta);

//Вызывается после основного игрового цикла
void onEnd();

#endif
