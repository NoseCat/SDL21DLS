#pragma once
#ifndef _ENEMY_BEHAVIOR_H_
#define _ENEMY_BEHAVIOR_H_

#include "SDLstuff.h"
#include "Structs.h"
#include "Collision.h"

void updateEnemies(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize, float delta);

#endif