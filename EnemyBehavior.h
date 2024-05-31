#pragma once
#ifndef _ENEMY_BEHAVIOR_H_
#define _ENEMY_BEHAVIOR_H_

#include "SDLstuff.h"
#include "Structs.h"
#include "Collision.h"

void spawnExplosion(Entity* entityArr, int entityArrSize, SDL_FPoint pos, SDL_FPoint speedVec);

void updateEnemies(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize, Door* doorsArr, int doorsArrSize, float delta);

void spawnBullet(Entity* entityArr, int entityArrSize, SDL_FPoint pos, SDL_FPoint speedVec);

#endif