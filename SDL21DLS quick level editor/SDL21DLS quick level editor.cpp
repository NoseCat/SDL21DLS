#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "SDLstuff.h"
#include "Input.h"
#include "string.h"
#include "Structs.h"
const int NAMEMAXSIZE = 50;
const int delrad = 50;
const int centerSize = 50;

float scale = 0.1;
SDL_FPoint offset = { WIN_WIDTH / 2 * 1 / scale,WIN_HEIGHT / 2 * 1 / scale };

Entity* entities;
int enemiesSize = 0;

ZHIR_LineF* lines;
int linesSize = 0;

Door* doors;
int doorsSize = 0;

float levelTime = -1;

FILE* levelRead;
FILE* levelWrite;
char* name;

void onStart()
{
	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF));
	doors = (Door*)malloc(sizeof(Door));
	entities = (Entity*)malloc(sizeof(Entity));

	printf("\nВведите имя файла. \nЕсли такого файла не существует будет создан новый файл.\n");

	name = (char*)malloc(sizeof(char) * NAMEMAXSIZE);
	gets_s(name, NAMEMAXSIZE);
	if (strchr(name, '.') == NULL)
		strcat(name, ".bin");
	char* buffer = (char*)malloc(sizeof(char) * NAMEMAXSIZE);
	strcpy(buffer, name);
	strcpy(name, "levels\\");
	strcat(name, buffer);

	if (fopen_s(&levelRead, name, "rb") == 0)
	{
		fread(&levelTime, sizeof(float), 1, levelRead);
		levelTime = -1;

		fread(&linesSize, sizeof(int), 1, levelRead);
		lines = (ZHIR_LineF*)realloc(lines, sizeof(ZHIR_LineF) * linesSize);
		fread(lines, sizeof(ZHIR_LineF), linesSize, levelRead);

		fread(&doorsSize, sizeof(int), 1, levelRead);
		doors = (Door*)realloc(doors, sizeof(Door) * doorsSize);
		fread(doors, sizeof(Door), doorsSize, levelRead);

		fread(&enemiesSize, sizeof(int), 1, levelRead);
		entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
		fread(entities, sizeof(Entity), enemiesSize, levelRead);

		fclose(levelRead);
	}

	printf("\nИспользуйе кнопки 1-0 чтобы переключаться между обьектами.\n1 - Cтены\n2 - Враг бегун\n3 - Враг стрелок\n4 - Враг портал\n5 - 7 Красная, зеланая и синяя дверь соответсвенно\n8 - 0 Красная, зеланая и синяя ключ карта соответсвенно\n");
}

bool drawEnemy = false;
SDL_FPoint tempPos = { 0,0 };
float tempRad = 25;

SDL_FPoint linePoint1 = { 0,0 };
SDL_FPoint linePoint2 = { 0,0 };
bool drawline = false;

bool del = false;

int modeToEnemyType(int mode)
{
	switch (mode)
	{
	case LINES:
		return EMPTY;

	case KEYCARDREDm:
		return KEYCARDRED;
	case DOORRED:
		return RED;
	case KEYCARDGREENm:
		return KEYCARDGREEN;
	case DOORGREEN:
		return GREEN;
	case KEYCARDBLUEm:
		return KEYCARDBLUE;
	case DOORBLUE:
		return BLUE;

	case ENEMYTYPE1:
		return RUNNER;
	case ENEMYTYPE2:
		return SHOOTER;
	case ENEMYTYPE3:
		return PORTAL;
	}
	return EMPTY;
}

void eachFrame(float delta)
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	//printf("%i\n", linesSize);
	for (int i = 0; i < doorsSize; i++)
	{
		if (doors[i].line.a.x == doors[i].line.b.x && doors[i].line.a.y == doors[i].line.b.y)
		{
			for (int j = i + 1; j < doorsSize; j++)
			{
				doors[j - 1] = doors[j];
			}
			doorsSize--;
			doors = (Door*)realloc(doors, sizeof(Door) * doorsSize);
		}
	}

	for (int i = 0; i < linesSize; i++)
	{
		if (lines[i].a.x == lines[i].b.x && lines[i].a.y == lines[i].b.y)
		{
			for (int j = i + 1; j < linesSize; j++)
			{
				lines[j - 1] = lines[j];
			}
			linesSize--;
			lines = (ZHIR_LineF*)realloc(lines, sizeof(ZHIR_LineF) * linesSize);
		}
	}

	for (int i = 0; i < enemiesSize; i++)
	{
		if (entities[i].type == EMPTY)
		{
			for (int j = i + 1; j < enemiesSize; j++)
			{
				entities[j - 1] = entities[j];
			}
			enemiesSize--;
			entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
		}
	}

	SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
	SDL_RenderDrawLineF(ren, (offset.x - centerSize) * scale, offset.y * scale, (offset.x + centerSize) * scale, offset.y * scale);
	SDL_RenderDrawLineF(ren, offset.x * scale, (offset.y - centerSize) * scale, offset.x * scale, (offset.y + centerSize) * scale);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	for (int i = 0; i < linesSize; i++)
		SDL_RenderDrawLineF(ren, (lines[i].a.x + offset.x) * scale, (lines[i].a.y + offset.y) * scale, (lines[i].b.x + offset.x) * scale, (lines[i].b.y + offset.y) * scale);
	if (drawline)
	{
		switch (mode)
		{
		case DOORRED:
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			break;
		case DOORGREEN:
			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			break;
		case DOORBLUE:
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
			break;
		}
		SDL_RenderDrawLineF(ren, (linePoint1.x + offset.x) * scale, (linePoint1.y + offset.y) * scale, MousePos.x, MousePos.y);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	}

	for (int i = 0; i < doorsSize; i++)
	{
		switch (doors[i].id)
		{
		case RED:
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			break;
		case GREEN:
			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			break;
		case BLUE:
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
			break;
		}
		SDL_RenderDrawLineF(ren, (doors[i].line.a.x + offset.x) * scale, (doors[i].line.a.y + offset.y) * scale, (doors[i].line.b.x + offset.x) * scale, (doors[i].line.b.y + offset.y) * scale);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	}

	for (int i = 0; i < enemiesSize; i++)
	{
		switch (entities[i].type)
		{
		case KEYCARDRED:
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			break;
		case KEYCARDGREEN:
			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			break;
		case KEYCARDBLUE:
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
			break;
		}
		ZHIR_drawCircleF(ZHIR_vecMultF(ZHIR_vecSumF(entities[i].position, offset), scale), entities[i].radius * scale);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	}
	if (drawEnemy)
	{
		switch (mode)
		{
		case KEYCARDREDm:
			SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
			break;
		case KEYCARDGREENm:
			SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
			break;
		case KEYCARDBLUEm:
			SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
			break;
		}
		tempPos = MousePos;
		ZHIR_drawCircleF(tempPos, tempRad);
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	}

	//printf("%i\n", (int)input_MOUSEWHEELDOWN);
	//if (input_MOUSEWHEELUP)
	//{
	//	scale = ZHIR_vecSumF(scale, { 0.01, 0.01 });
	//}
	//if (input_MOUSEWHEELDOWN)
	//{
	//	scale = ZHIR_vecSumF(scale, { -0.01, -0.01 });
	//}

	if (input_MOUSEMIDDLECLICK)
	{
		offset.x += MousePosRel.x * 1 / scale;
		offset.y += MousePosRel.y * 1 / scale;
	}
	SDL_FPoint offsetMousePos = ZHIR_vecMultF(ZHIR_vecSubF(MousePos, ZHIR_vecMultF(offset, scale)), 1 / scale);

	if (input_MOUSERIGHTCLICK && !del)
		del = true;
	if (!input_MOUSERIGHTCLICK && del)
	{
		del = false;
		ZHIR_drawCircleF(MousePos, delrad);

		for (int i = 0; i < linesSize; i++)
		{
			SDL_FPoint proj = ZHIR_dotProjLineF(lines[i], offsetMousePos);
			if ((ZHIR_vecLengthF(ZHIR_vecSubF(proj, offsetMousePos)) <= delrad && ZHIR_isOnLineF(lines[i], proj)) ||
				ZHIR_vecLengthF(ZHIR_vecSubF(lines[i].a, offsetMousePos)) <= delrad ||
				ZHIR_vecLengthF(ZHIR_vecSubF(lines[i].b, offsetMousePos)) <= delrad)
			{
				lines[i] = { {0,0}, {0,0} };
			}
		}

		for (int i = 0; i < doorsSize; i++)
		{
			SDL_FPoint proj = ZHIR_dotProjLineF(doors[i].line, offsetMousePos);
			if ((ZHIR_vecLengthF(ZHIR_vecSubF(proj, offsetMousePos)) <= delrad && ZHIR_isOnLineF(doors[i].line, proj)) ||
				ZHIR_vecLengthF(ZHIR_vecSubF(doors[i].line.a, offsetMousePos)) <= delrad ||
				ZHIR_vecLengthF(ZHIR_vecSubF(doors[i].line.b, offsetMousePos)) <= delrad)
			{
				doors[i].line = { {0,0}, {0,0} };
			}
		}


		for (int i = 0; i < enemiesSize; i++)
		{
			if (ZHIR_vecLengthF(ZHIR_vecSubF(entities[i].position, offsetMousePos)) <= entities[i].radius + delrad)
			{
				entities[i].type = EMPTY;
			}
		}

	}

	switch (mode)
	{
	case LINES:
		if (input_MOUSELEFTCLICK && !drawline)
		{
			linePoint1 = offsetMousePos;
			drawline = true;
		}
		if (!input_MOUSELEFTCLICK && drawline)
		{
			linePoint2 = offsetMousePos;
			drawline = false;

			linesSize++;
			lines = (ZHIR_LineF*)realloc(lines, sizeof(ZHIR_LineF) * linesSize);
			lines[linesSize - 1] = { linePoint1, linePoint2 };
		}
		break;
	case DOORRED:
	case DOORGREEN:
	case DOORBLUE:
		if (input_MOUSELEFTCLICK && !drawline)
		{
			linePoint1 = offsetMousePos;
			drawline = true;
		}
		if (!input_MOUSELEFTCLICK && drawline)
		{
			linePoint2 = offsetMousePos;
			drawline = false;

			doorsSize++;
			doors = (Door*)realloc(doors, sizeof(Door) * doorsSize);
			doors[doorsSize - 1].line = { linePoint1, linePoint2 };
			doors[doorsSize - 1].id = modeToEnemyType(mode);
			doors[doorsSize - 1].open = false;
		}
		break;
	case KEYCARDREDm:
	case KEYCARDGREENm:
	case KEYCARDBLUEm:
		if (input_MOUSELEFTCLICK && !drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = true;
		}
		if (!input_MOUSELEFTCLICK && drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = false;

			enemiesSize++;
			entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
			//only neccesery stats
			entities[enemiesSize - 1].type = modeToEnemyType(mode);
			entities[enemiesSize - 1].position = tempPos;
			//class stats
			entities[enemiesSize - 1].radius = 50;
			entities[enemiesSize - 1].vertSize = 100;
			//junk
			entities[enemiesSize - 1].health = 100;
			entities[enemiesSize - 1].accel = 4000;
			entities[enemiesSize - 1].speedLimit = 5000;
			entities[enemiesSize - 1].friction = 2000;
			entities[enemiesSize - 1].actionDelay.active = false;
			entities[enemiesSize - 1].actionDelay.time = 3;
			entities[enemiesSize - 1].speed = 300;
			entities[enemiesSize - 1].accelVec = { 0,0 };
			entities[enemiesSize - 1].dir = { 0,0 };
			entities[enemiesSize - 1].speedVec = { 0,0 };
			entities[enemiesSize - 1].active = false;
			entities[enemiesSize - 1].anim = { false, 0 };
			entities[enemiesSize - 1].animFrame = 0;
			entities[enemiesSize - 1].fullAnimCycle = 1;

		}
		break;
	case ENEMYTYPE1: //RUNNER
		if (input_MOUSELEFTCLICK && !drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = true;
		}
		if (!input_MOUSELEFTCLICK && drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = false;

			enemiesSize++;
			entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
			//only neccesery stats
			entities[enemiesSize - 1].type = modeToEnemyType(mode);
			entities[enemiesSize - 1].position = tempPos;
			//class stats
			entities[enemiesSize - 1].radius = 200;
			entities[enemiesSize - 1].health = 100;
			entities[enemiesSize - 1].accel = 4000;
			entities[enemiesSize - 1].speedLimit = 5000;
			entities[enemiesSize - 1].friction = 2000;
			entities[enemiesSize - 1].vertSize = 400;
			entities[enemiesSize - 1].fullAnimCycle = 5;
			entities[enemiesSize - 1].anim = { false, 5};
			entities[enemiesSize - 1].animFrame = 0;
			//junk
			entities[enemiesSize - 1].actionDelay.active = false;
			entities[enemiesSize - 1].actionDelay.time = 3;
			entities[enemiesSize - 1].speed = 300;
			entities[enemiesSize - 1].accelVec = { 0,0 };
			entities[enemiesSize - 1].dir = { 0,0 };
			entities[enemiesSize - 1].speedVec = { 0,0 };
			entities[enemiesSize - 1].active = false;
		}
		break;
	case ENEMYTYPE2: //SHOOTER
		if (input_MOUSELEFTCLICK && !drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = true;
		}
		if (!input_MOUSELEFTCLICK && drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = false;

			enemiesSize++;
			entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
			//only neccesery stats
			entities[enemiesSize - 1].type = modeToEnemyType(mode);
			entities[enemiesSize - 1].position = tempPos;
			//class stats
			entities[enemiesSize - 1].radius = 100;
			entities[enemiesSize - 1].health = 50;
			entities[enemiesSize - 1].accel = 10000;
			entities[enemiesSize - 1].speedLimit = 1000;
			entities[enemiesSize - 1].friction = 6000;
			entities[enemiesSize - 1].vertSize = 200;
			entities[enemiesSize - 1].actionDelay.active = false;
			entities[enemiesSize - 1].actionDelay.time = 3;
			entities[enemiesSize - 1].fullAnimCycle = 5;
			entities[enemiesSize - 1].anim = { false, 5 };
			entities[enemiesSize - 1].animFrame = 0;
			//junk
			entities[enemiesSize - 1].speed = 300;
			entities[enemiesSize - 1].accelVec = { 0,0 };
			entities[enemiesSize - 1].dir = { 0,0 };
			entities[enemiesSize - 1].speedVec = { 0,0 };
			entities[enemiesSize - 1].active = false;
			/*entities[enemiesSize - 1].face1 =;
			entities[enemiesSize - 1].face2 =;*/
		}
		break;
	case ENEMYTYPE3:
		if (input_MOUSELEFTCLICK && !drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = true;
		}
		if (!input_MOUSELEFTCLICK && drawEnemy)
		{
			tempPos = offsetMousePos;
			drawEnemy = false;

			enemiesSize++;
			entities = (Entity*)realloc(entities, sizeof(Entity) * enemiesSize);
			//only neccesery stats
			entities[enemiesSize - 1].type = modeToEnemyType(mode);
			entities[enemiesSize - 1].position = tempPos;
			//class stats
			entities[enemiesSize - 1].radius = 250;
			entities[enemiesSize - 1].health = 200;
			entities[enemiesSize - 1].accel = 0;
			entities[enemiesSize - 1].speedLimit = 0;
			entities[enemiesSize - 1].friction = 0;
			entities[enemiesSize - 1].vertSize = 500;
			entities[enemiesSize - 1].actionDelay.active = false;
			entities[enemiesSize - 1].actionDelay.time = 6;
			entities[enemiesSize - 1].fullAnimCycle = 2;
			entities[enemiesSize - 1].anim = { false, 5 };
			entities[enemiesSize - 1].animFrame = 0;
			//junk
			entities[enemiesSize - 1].speed = 300;
			entities[enemiesSize - 1].accelVec = { 0,0 };
			entities[enemiesSize - 1].dir = { 0,0 };
			entities[enemiesSize - 1].speedVec = { 0,0 };
			entities[enemiesSize - 1].active = false;
		}
		break;
	}
}

void onEnd()
{
	if (fopen_s(&levelWrite, name, "wb") != 0)
		exit(1);

	fwrite(&levelTime, sizeof(float), 1, levelWrite);

	fwrite(&linesSize, sizeof(int), 1, levelWrite);
	fwrite(lines, sizeof(ZHIR_LineF), linesSize, levelWrite);

	fwrite(&doorsSize, sizeof(int), 1, levelWrite);
	fwrite(doors, sizeof(Door), doorsSize, levelWrite);

	fwrite(&enemiesSize, sizeof(int), 1, levelWrite);
	fwrite(entities, sizeof(Entity), enemiesSize, levelWrite);

	fclose(levelWrite);
	free(lines);
	free(entities);
}
