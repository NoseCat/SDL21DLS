#pragma once
#ifndef _INPUT_H_
#define _INPUT_H_
#include <iostream>
#include <SDL.h>
#include "SDLstuff.h"

extern SDL_FPoint MousePos;
extern SDL_FPoint MousePosRel;

extern bool input_LMB;
extern bool input_RMB;
extern bool input_MMB;
extern bool input_UP;
extern bool input_DOWN;
extern bool input_LEFT;
extern bool input_RIGHT;
extern bool input_R;

extern SDL_FPoint InputDir;

void inputUpdate();

#endif