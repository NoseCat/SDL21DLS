#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_
#include <iostream>
#include <SDL.h>

extern SDL_FPoint MousePos;
extern SDL_FPoint MousePosRel;

extern bool input_MOUSELEFTCLICK;
extern bool input_MOUSERIGHTCLICK;
extern bool input_MOUSEMIDDLECLICK;
extern bool input_MOUSEWHEELDOWN;
extern bool input_MOUSEWHEELUP;

enum Modes { LINES, ENEMYTYPE1, ENEMYTYPE2, ENEMYTYPE3, DOORRED, DOORGREEN, DOORBLUE , KEYCARDREDm, KEYCARDGREENm, KEYCARDBLUEm};
extern int mode;

//extern bool input_UP;
//extern bool input_DOWN;
//extern bool input_LEFT;
//extern bool input_RIGHT;

extern SDL_FPoint InputDir;

void inputUpdate();

#endif