#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "SDLstuff.h"

//void smkLineBounce(const ZHIR_LineF& smkline, int maxbounces, int id, ZHIR_LineF* smkLines, ZHIR_LineF* lines, int linesSize);


//���������� true ���� ���������� ���������� ���������� � false � �������� ������
bool isCircleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius);

//���������� ����� point ��� ����� ����������� �� ������������. 
SDL_FPoint circleCircleCollide(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& point, float radius, bool& collided);

//���������� ����� point ��� ����� ����������� �� ������������. 
//������ iterations �������� �������� �������� � �������������� ����������� ����
SDL_FPoint circleCircleCollideIterations(const SDL_FPoint& constCircPos, float circRadius, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided);


//���������� true ���� ������� ���������� ���������� � false � �������� ������
bool isLineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius);

//���������� ����� point ���������� ��� ����� ���������� �� ��� ���������� �� ���������� �������
SDL_FPoint lineCircleCollide(const ZHIR_LineF& line, const SDL_FPoint& point, float radius, bool& collided);

//���������� ����� newPos ���������� ��� ����� ���������� �� ��� ���������� �� ���������� �������
//������ iterations �������� �������� �������� � �������������� ����������� ����
SDL_FPoint lineCircleCollideIterations(const ZHIR_LineF& line, const SDL_FPoint& oldPos, SDL_FPoint newPos, float radius, int iterations, bool& collided);

#endif