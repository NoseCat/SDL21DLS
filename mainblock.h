#pragma once
#ifndef _MAIN_BLOCK_
#define _MAIN_BLOCK_

enum GameStates { MENU, LEVELSELECT, GAME, PAUSED, EXIT, LEVELWIN, LEVELLOST};
extern int GameState;

//���������� �� ������� ���������
void globalOnStart();

void onLevelStart(const char* levelname);

//���������� ������ ����
void levelSelectEachFrame();

void mainMenuEachFrame();

void winLevelEachFrame();

void lostLevelEachFrame();

void eachFrame(float delta);

//���������� ����� ��������� �������� �����
void onLevelEnd();

void onEnd();

#endif
