#pragma once
#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "SDLstuff.h"

//������� ����� newPos ��� ����� ����������� �� ������������. 
//���������� ������ circPos - newPos(����������) ��� {0,0} ���� �������� �� ����
SDL_FPoint circleCollide(const SDL_FPoint& circPos, float circRadius, SDL_FPoint& newPos, float radius);

//������� ����� newPos ��� ����� ���������� �� ��� ���������� �� ���������� �������
//���������� ������, ������ ��������� � ������ ���������� ����� ����������� ���� ���������� � �������� ����� ���������� newPos
//��� {0,0} ���� �������� �� ����
SDL_FPoint lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos, float radius);

#endif