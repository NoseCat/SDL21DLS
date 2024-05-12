#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "SDLstuff.h"

//void smkLineBounce(const ZHIR_LineF& smkline, int maxbounces, int id, ZHIR_LineF* smkLines, ZHIR_LineF* lines, int linesSize);

//Изменят точку newPos так чтобы окурожности не пересекались. 
//Возвращает вектор circPos - newPos(измененный) или {0,0} если коллизий не было
SDL_FPoint circleCollide(const SDL_FPoint& circPos, float circRadius, SDL_FPoint& newPos, float radius);

//возвращает точку newPos измененную так чтобы построеная на ней окружность не пересекала отрезок
SDL_FPoint lineCollide(const ZHIR_LineF& line, const SDL_FPoint& newPos, float radius);

SDL_FPoint lineCollideIterations(const ZHIR_LineF& line, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations);

#endif