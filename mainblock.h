#pragma once
#ifndef _MAIN_BLOCK_
#define _MAIN_BLOCK_

enum GameStates { MENU, LEVELSELECT, GAME, PAUSED, EXIT, };
extern int GameState;

//���������� �� ������� ���������
void globalOnStart();

void onLevelStart(const char* levelname);

//���������� ������ ����
void levelSelectEachFrame();

void mainMenuEachFrame();

void eachFrame(float delta);

//���������� ����� ��������� �������� �����
void onEnd();

#endif
