#pragma once
#ifndef _MAIN_BLOCK_
#define _MAIN_BLOCK_

enum GameStates { MENU, LEVELSELECT, GAME, PAUSED, EXIT, LEVELWIN, LEVELLOST, AUTHOR};
extern int GameState;

//���������� �� ������� ���������
void globalOnStart();

void onLevelStart(const char* levelname);

//���������� ������ ����
void levelSelectEachFrame();

void mainMenuEachFrame();

void winLevelEachFrame();

void lostLevelEachFrame();

void authorEachFrame();

void eachFrame(float delta);

//���������� ����� ��������� �������� �����
void onLevelEnd();

void onEnd();

#endif
