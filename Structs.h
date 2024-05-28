#pragma once
#ifndef _STRUCTS_
#define _STRUCTS_

#include "SDLstuff.h"

//=============================================================================================================================

#pragma region STRUCTS

struct Player 
{
	SDL_FPoint position = { 0, 0};
	float radius = 150; //player size
	SDL_FPoint speedVec = { 0,0 };
	float speed = 0;
	float speedLimit = 2500;
	float friction = 6000;
	SDL_FPoint accelVec = { 0,0 };
	float accel = 10000 + friction;

	Timer damageInv = { true, 2 };

	int health = 100;
	int bulletDamage = 20;
	Timer shotDelay = { true,  0.8 };

	//needs fix
	float FOV = 90;
	float lFOV =  0;
	float hFOV = lFOV + FOV;
};

struct Sprite
{
	SDL_Texture* texture;
	int w;
	int h;
};

enum EnemyTypes { EMPTY, RUNNER, SHOOTER, PORTAL, BULLET, ENEMYBULLET };

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

	Timer actionDelay = { false, 3 };

	Sprite* sprite;
	//real position of sprite
	ZHIR_LineF face1;
	//sprite on screen position
	ZHIR_LineF face2;
	//on screen sprite height
	float vertSize = 100;
};

#pragma endregion //STRUCTS

//=============================================================================================================================

#pragma region GLOBAL

//sprites
extern Sprite sprite1;
extern Sprite BulletSprite;

extern Player player;

//settings
extern float mouseSensetivity;
extern float rayPrecision;
extern float wallSize;
extern float viewDistance;
extern float aerialFactor;
extern float aerialLowerBorder;
extern int collisionPrecision;

#pragma endregion //GLOBAL

//=============================================================================================================================

#endif