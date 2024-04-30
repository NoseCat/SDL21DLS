#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "SDLstuff.h"

//Изменят точку newPos так чтобы окурожности не пересекались. 
//Возвращает вектор circPos - newPos(измененный) или {0,0} если коллизий не было
SDL_FPoint circleCollide(const SDL_FPoint& circPos, float circRadius, SDL_FPoint& newPos, float radius);

//Изменят точку newPos так чтобы построеная на ней окрудность не пересекала отрезок
//Возвращает вектор, равный ближайшей к центру окружности точки пересечения этой окружности с отрезком минус измененный newPos
//или {0,0} если коллизий не было
SDL_FPoint lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos, float radius);

#endif