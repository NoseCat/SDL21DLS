#pragma once
#ifndef _STRUCTS_
#define _STRUCTS_

#include "SDLstuff.h"

struct Sprite
{
	SDL_Texture* texture;
	int w;
	int h;
};

enum ColorCode { RED = 1, GREEN, BLUE };
struct Door
{
	ZHIR_LineF line;
	bool open = false;
	int id;
};

enum EnemyTypes { EMPTY, RUNNER, SHOOTER, PORTAL, BULLET, ENEMYBULLET, KEYCARDRED, KEYCARDGREEN, KEYCARDBLUE };

struct Entity
{
	int type;

	float radius = 100;
	SDL_FPoint position = { 0,0 };
	float friction = 1000;
	SDL_FPoint dir = { 1, 0 };
	float speed = 0;
	SDL_FPoint speedVec = { 0,0 };
	float speedLimit = 2500;
	float accel = 2000;
	SDL_FPoint accelVec = { 0,0 };

	int health = 100;

	Timer actionDelay = { true, 3 };

	Sprite* sprite;
	//real position of sprite
	ZHIR_LineF face1;
	//sprite on screen position
	ZHIR_LineF face2;
	//on screen sprite height
	float vertSize = 100;

	bool active = false;

	float fullAnimCycle = 5;
	Timer anim = { false, fullAnimCycle };
	int animFrame;
};

#endif