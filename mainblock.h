#pragma once
#ifndef _MAIN_BLOCK_
#define _MAIN_BLOCK_

enum GameStates { MENU, LEVELSELECT, GAME, PAUSED, EXIT, };
extern int GameState;

//Вызывается на запуске программы
void globalOnStart();

void onStart();

//Вызывается каждый кадр

void mainMenuEachFrame();

void eachFrame(float delta);

//Вызывается после основного игрового цикла
void onEnd();

#endif
