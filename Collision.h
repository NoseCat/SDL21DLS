#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "SDLstuff.h"

//void smkLineBounce(const ZHIR_LineF& smkline, int maxbounces, int id, ZHIR_LineF* smkLines, ZHIR_LineF* lines, int linesSize);


//возвращает true если окружность пересекает окружность и false в обратном случае
bool isCircleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius);

//возвращает точку point так чтобы окурожности не пересекались. 
SDL_FPoint circleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius, bool& collided);

//возвращает точку point так чтобы окурожности не пересекались. 
//делает iterations итерации проверяя коллизию с промежуточными положениями тела
SDL_FPoint circleCircleCollideIterations(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided);


//возвращает true если отрезок пересекает окружность и false в обратном случае
bool isLineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius);

//возвращает точку point измененную так чтобы построеная на ней окружность не пересекала отрезок
SDL_FPoint lineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius, bool& collided);

//возвращает точку newPos измененную так чтобы построеная на ней окружность не пересекала отрезок
//делает iterations итерации проверяя коллизию с промежуточными положениями тела
SDL_FPoint lineCircleCollideIterations(const ZHIR_LineF& line, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided);

#endif