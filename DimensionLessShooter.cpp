#include <iostream>
#include "SDLstuff.h"
#include "Input.h"

SDL_FPoint pf1;
SDL_FPoint pf2;
SDL_FPoint pf3;

ZHIR_LineF line1;
ZHIR_LineF line2;
ZHIR_LineF line3;
ZHIR_LineF line4;
ZHIR_LineF line5;
ZHIR_LineF line6;

ZHIR_LineF* lines;
int linesSize = 6;

SDL_FPoint playerPosition = { 0,0 };
float speed = 800;
float radius = 150;

float FOV = 90;
float lFOV = 45 + 180;
float hFOV = lFOV + FOV;

float rayPrecision = 0.1f;
float wallSize = 500.0f;
float viewDistance = 10000.0f;
float mouseSensetivity = 0.3f;

void onStart()
{
	pf1 = { WIN_CENTER.x + 200.0f, (float)WIN_CENTER.y + 100 };
	pf2 = { WIN_CENTER.x - 200.0f, (float)WIN_CENTER.y };
	pf3 = { WIN_CENTER.x + 200.0f, WIN_CENTER.y + 200.0f };

	line1 = { pf1, pf2 };
	line2 = { pf3, {(float)WIN_CENTER.x, (float)WIN_CENTER.y} };

	//line1 = { { 400,400}, { 400,400} };
	//line2 = { { 400,400 }, { 400,400 } };

	line3 = { {50, 50}, {150, 50} };
	line4 = { {150, 50}, {150, 150} };
	line5 = { {150, 150}, {50, 150} };
	line6 = { {50, 150}, {50, 50} };

	lines = (ZHIR_LineF*)malloc(sizeof(ZHIR_LineF) * linesSize);
	lines[0] = line1;
	lines[1] = line2;

	lines[2] = line3;
	lines[3] = line4;
	lines[4] = line5;
	lines[5] = line6;

}

void lineRender(const ZHIR_LineF* linesArr, int linesArrSize)
{
	//initializtion
	SDL_FPoint unitVec = ZHIR_vecSumF(playerPosition, { 1,0 });
	int numPoints = ((int)(FOV / rayPrecision) + 1);
	SDL_FPoint* linePoints = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * numPoints);
	int* lineIntersections = (int*)malloc(sizeof(int) * numPoints);
	for (int i = 0; i < numPoints; i++)
	{
		linePoints[i] = { -1, -1 };
		lineIntersections[i] = -1;
	}

	//compute cycle
	numPoints = 0;
	for (float angle = lFOV; angle <= hFOV; angle += rayPrecision)
	{
		SDL_FPoint ray = ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(ZHIR_rotateOnDegreeF(unitVec, playerPosition, angle), playerPosition)), viewDistance);
		ray = ZHIR_vecSumF(ray, playerPosition);
		float minDistance = viewDistance;
		//SDL_RenderDrawLineF(ren, playerPosition.x, playerPosition.y, ray.x, ray.y);
		for (int i = 0; i < linesArrSize; i++)
		{
			float distance = 0;
			if (ZHIR_isIntersectF({ playerPosition, ray }, linesArr[i]))
			{
				distance = ZHIR_vecLengthF(ZHIR_vecSubF(ZHIR_findIntersectF({ playerPosition, ray }, linesArr[i]), playerPosition));
				if (distance < minDistance)
				{
					minDistance = distance;
					float size = WIN_HEIGHT / 2 - WIN_HEIGHT / 2 * wallSize / minDistance;
					float x = WIN_WIDTH * (angle - lFOV) / FOV;
					linePoints[numPoints] = { x,  size };
					lineIntersections[numPoints] = i;
				}
			}
		}
		numPoints++;
	}

	//render cycle
	for (int i = 0; i < numPoints - 1; i++)
	{
		bool intersection = false;
		if (lineIntersections[i] != lineIntersections[i + 1])
		{
			intersection = true;
		}

		if (!intersection)
		{
			SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i + 1].y);
			SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
		}
		else
		{
			intersection = false;
			if (linePoints[i].x >= 0 && linePoints[i + 1].x >= 0)
			{
				SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i + 1].x, linePoints[i].y);
				SDL_RenderDrawLineF(ren, linePoints[i].x, WIN_HEIGHT - linePoints[i].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i].y);

				float y = SDL_min(linePoints[i + 1].y, linePoints[i].y);
				SDL_RenderDrawLineF(ren, linePoints[i + 1].x, y, linePoints[i + 1].x, WIN_HEIGHT - y);
			}
			else if (linePoints[i + 1].x >= 0)
			{
				SDL_RenderDrawLineF(ren, linePoints[i + 1].x, linePoints[i + 1].y, linePoints[i + 1].x, WIN_HEIGHT - linePoints[i + 1].y);
			}
			else
			{
				SDL_RenderDrawLineF(ren, linePoints[i].x, linePoints[i].y, linePoints[i].x, WIN_HEIGHT - linePoints[i].y);
			}
		}
	}

	free(linePoints);
	free(lineIntersections);
}

void lineCollide(const ZHIR_LineF& line, SDL_FPoint& newPos)
{
	SDL_FPoint proj = ZHIR_dotProjLineF(line, newPos);
	if (ZHIR_vecLengthF(ZHIR_vecSubF(proj, newPos)) < radius && ZHIR_isOnLineF(line, proj))
	{
		newPos = ZHIR_vecSumF(proj, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, proj)), radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.a, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.a)), radius));
	}
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.b, newPos)) < radius)
	{
		newPos = ZHIR_vecSumF(line.b, ZHIR_vecMultF(ZHIR_vecNormal(ZHIR_vecSubF(newPos, line.b)), radius));
	}
}

void eachFrame(float delta)
{
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);

	inputUpdate();
	lFOV += MousePosRel.x * mouseSensetivity;

	SDL_WarpMouseInWindow(win, WIN_CENTER.x, WIN_CENTER.y);

	if (lFOV < -360)
		lFOV += 360;
	if (lFOV > 360)
		lFOV -= 360;
	hFOV = lFOV + FOV;

	//for (int i = 0; i < linesSize; i++)
	//{
	//	SDL_RenderDrawLineF(ren, lines[i].a.x, lines[i].a.y, lines[i].b.x, lines[i].b.y);
	//}

	SDL_FPoint newPosition = ZHIR_vecSumF(playerPosition, ZHIR_vecMultF(ZHIR_rotateOnDegreeF(InputDir, { 0,0 }, lFOV + 180 - FOV / 2), speed * delta));

	for (int i = 0; i < linesSize; i++)
		lineCollide(lines[i], newPosition);
	playerPosition = newPosition;

	lineRender(lines, linesSize);

	//ZHIR_drawCircleF(playerPosition, radius);
}