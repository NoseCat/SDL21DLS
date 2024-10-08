﻿#include "SDLstuff.h"

#pragma region INIT

int WIN_WIDTH = 1280;
int WIN_HEIGHT = 720;
SDL_Point WIN_CENTER = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };

float FRAMES = 1000 / 60; //FRAMES is in ms
SDL_Renderer* ren = NULL;
SDL_Window* win = NULL;

void deInit(int error)
{
	if (ren != NULL) SDL_DestroyRenderer(ren);
	if (win != NULL) SDL_DestroyWindow(win);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	exit(error);
}
void Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		printf("no sdl :( %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
	printf("SDL init\n");

	int res;
	res = IMG_Init(IMG_INIT_PNG);
	if (res == 0)
	{
		printf("no img :( %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
	printf("IMG init\n");
	if (res & IMG_INIT_PNG) printf("\tpng init\n"); else printf("no png init :(\n");

	if (TTF_Init())
	{
		printf("no ttf :( %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
	printf("TTF init\n");

	win = SDL_CreateWindow("SDL21DLS",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_WIDTH, WIN_HEIGHT,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
	if (win == NULL)
	{
		printf("no win :( %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
	printf("\nwindow created\n");

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (ren == NULL)
	{
		printf("no ren :( %s", SDL_GetError());
		system("pause");
		deInit(1);
	}
	printf("render created\n");
}
#pragma endregion //INIT

#pragma region BasicMath

int ZHIR_rand(int a, int b)
{
	return rand() % (b - a + 1) + a;
}

float ZHIR_lerp(float a, float b, float t)
{
	return a + t * (b - a);
}

void ZHIR_swap(int& a, int& b)
{
	a ^= b ^= a ^= b;
}
void ZHIR_swapF(float& a, float& b)
{
	float c = a;
	a = b;
	b = c;
}

int ZHIR_slap(int ch, int a, int b)
{
	if (a > b)
		ZHIR_swap(a, b);

	if (ch <= a)
		return a;
	if (ch >= b)
		return b;

	return ch;
}
float ZHIR_slapF(float ch, float a, float b)
{
	if (a > b)
		ZHIR_swapF(a, b);

	if (ch <= a)
		return a;
	if (ch >= b)
		return b;

	return ch;

}

#pragma endregion //BasicMath

#pragma region Convertation

//Конвертация из SDL_Point в SDL_FPoint
SDL_FPoint FPoint(SDL_Point p)
{
	return {(float)p.x, (float)p.y};
}

//Конвертация из SDL_FPoint в SDL_Point (с округлением)
SDL_Point Point(SDL_FPoint p)
{
	return { (int)round(p.x), (int)round(p.y) };
}

//Конвертация из SDL_FRect в SDL_Rect
SDL_Rect ZHIR_FRectToRect(const SDL_FRect& rect)
{
	return { (int)round(rect.x), (int)round(rect.y), (int)round(rect.w), (int)round(rect.h) };
}

#pragma endregion //Convertation

#pragma region VectorMath & Geometry
int ZHIR_vecLength(const SDL_Point& vec)
{
	return (int)round(sqrt(vec.x * vec.x + vec.y * vec.y));
}
float ZHIR_vecLengthF(const SDL_FPoint& vec)
{
	return sqrt(vec.x * vec.x + vec.y * vec.y);
}

SDL_Point ZHIR_vecSum(const SDL_Point& a, const SDL_Point& b)
{
	return { a.x + b.x, a.y + b.y };
}
SDL_FPoint ZHIR_vecSumF(const SDL_FPoint& a, const SDL_FPoint& b)
{
	return { a.x + b.x, a.y + b.y };
}

SDL_Point ZHIR_vecSub(const SDL_Point& a, const SDL_Point& b)
{
	return { a.x - b.x, a.y - b.y };
}
SDL_FPoint ZHIR_vecSubF(const SDL_FPoint& a, const SDL_FPoint& b)
{
	return { a.x - b.x, a.y - b.y };
}

SDL_FPoint ZHIR_vecNormal(const SDL_FPoint& a)
{
	float len = ZHIR_vecLengthF(a);
	if (len == 0)
		return { 0,0 };
	return { a.x / len, a.y / len };
}

SDL_Point ZHIR_vecMult(const SDL_Point& a, int b)
{
	return { a.x * b , a.y * b };
}
SDL_FPoint ZHIR_vecMultF(const SDL_FPoint& a, float b)
{
	return { a.x * b , a.y * b };
}

int ZHIR_vecMultScalar(const SDL_Point& a, const SDL_Point& b)
{
	return a.x * b.x + a.y * b.y;
}
float ZHIR_vecMultScalarF(const SDL_FPoint& a, const SDL_FPoint& b)
{
	return a.x * b.x + a.y * b.y;
}

SDL_Point ZHIR_rotateOnDegree(const SDL_Point& point, const SDL_Point& rotationPoint, const float angle)
{
	float rad = (float)(M_PI / 180.0) * angle;
	SDL_Point newPoint = {};

	newPoint.x = round(rotationPoint.x + (point.x - rotationPoint.x) * cos(rad) - (point.y - rotationPoint.y) * sin(rad));
	newPoint.y = round(rotationPoint.y + (point.x - rotationPoint.x) * sin(rad) + (point.y - rotationPoint.y) * cos(rad));

	return newPoint;
}
SDL_FPoint ZHIR_rotateOnDegreeF(const SDL_FPoint& point, const SDL_FPoint& rotationPoint, const float angle)
{
	float rad = (float)(M_PI / 180.0) * angle;
	SDL_FPoint newPoint = {};

	newPoint.x = rotationPoint.x + (point.x - rotationPoint.x) * cos(rad) - (point.y - rotationPoint.y) * sin(rad);
	newPoint.y = rotationPoint.y + (point.x - rotationPoint.x) * sin(rad) + (point.y - rotationPoint.y) * cos(rad);

	return newPoint;
}

SDL_Point ZHIR_findIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2)
{
	// Уравнение первой прямой
	int A1 = line1.b.y - line1.a.y;
	int B1 = line1.a.x - line1.b.x;
	int C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	int A2 = line2.b.y - line2.a.y;
	int B2 = line2.a.x - line2.b.x;
	int C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	return { (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1), (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1) };
}
SDL_FPoint ZHIR_findIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2)
{
	// Уравнение первой прямой
	float A1 = line1.b.y - line1.a.y;
	float B1 = line1.a.x - line1.b.x;
	float C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	float A2 = line2.b.y - line2.a.y;
	float B2 = line2.a.x - line2.b.x;
	float C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	return { (B1 * C2 - B2 * C1) / (A1 * B2 - A2 * B1), (C1 * A2 - C2 * A1) / (A1 * B2 - A2 * B1) };
}

SDL_Point ZHIR_dotProjLine(const ZHIR_Line& line, const SDL_Point& p)
{
	SDL_Point C = { p.x - (line.b.y - line.a.y), p.y + (line.b.x - line.a.x) };
	return ZHIR_findIntersect(line, { p, C });
}
SDL_FPoint ZHIR_dotProjLineF(const ZHIR_LineF& line, const SDL_FPoint& p)
{
	SDL_FPoint C = { p.x - (line.b.y - line.a.y), p.y + (line.b.x - line.a.x) };
	return ZHIR_findIntersectF(line, { p, C });
}

int ZHIR_vecFindAngle(const SDL_Point& vec1, const SDL_Point& vec2)
{
	if (vec1.x / vec1.y == vec2.x / vec2.y)
		return 0;
	return (int)(acos(ZHIR_vecMultScalar(vec1, vec2) / (ZHIR_vecLength(vec1) * ZHIR_vecLength(vec2))) * 180 / M_PI);
}
float ZHIR_vecFindAngleF(const SDL_FPoint& vec1, const SDL_FPoint& vec2)
{
	if (vec1.x / vec1.y == vec2.x / vec2.y)
		return 0;
	return acos(ZHIR_vecMultScalarF(vec1, vec2) / (ZHIR_vecLengthF(vec1) * ZHIR_vecLengthF(vec2))) * 180 / M_PI;
}

int ZHIR_vecFindAngleFull(const SDL_Point& vec1, const SDL_Point& vec2)
{
	int angle = ((float)(vec1.y > 0) * -2 + 1) * ZHIR_vecFindAngle(vec1, { 1,0 });

	SDL_Point nVec1 = ZHIR_rotateOnDegree(vec1, { 0,0 }, angle);
	SDL_Point nVec2 = ZHIR_rotateOnDegree(vec2, { 0,0 }, angle);

	if (nVec2.y > nVec1.y)
		return 360.0f - ZHIR_vecFindAngle(nVec1, nVec2);
	return ZHIR_vecFindAngle(nVec1, nVec2);
}
float ZHIR_vecFindAngleFullF(const SDL_FPoint& vec1, const SDL_FPoint& vec2)
{
	float angle = ((float)(vec1.y > 0) * -2.0f + 1.0f) * ZHIR_vecFindAngleF(vec1, { 1,0 });

	SDL_FPoint nVec1 = ZHIR_rotateOnDegreeF(vec1, { 0,0 }, angle);
	SDL_FPoint nVec2 = ZHIR_rotateOnDegreeF(vec2, { 0,0 }, angle);

	if (nVec2.y > nVec1.y)
		return 360.0f - ZHIR_vecFindAngleF(nVec1, nVec2);
	return ZHIR_vecFindAngleF(nVec1, nVec2);
}

int ZHIR_findIntersectAngle(const ZHIR_Line& line1, const ZHIR_Line& line2)
{
	SDL_Point vec1 = ZHIR_vecSub(line1.a, line1.b);
	SDL_Point vec2 = ZHIR_vecSub(line2.a, line2.b);
	return ZHIR_vecFindAngle(vec1, vec2);
}
float ZHIR_findIntersectAngleF(const ZHIR_LineF& line1, const ZHIR_LineF& line2)
{
	SDL_FPoint vec1 = ZHIR_vecSubF(line1.a, line1.b);
	SDL_FPoint vec2 = ZHIR_vecSubF(line2.a, line2.b);
	return ZHIR_vecFindAngleF(vec1, vec2);
}

bool ZHIR_isOnLineF(const ZHIR_LineF& line, const SDL_FPoint& p)
{
	if (ZHIR_vecLengthF(ZHIR_vecSubF(line.a, p)) + ZHIR_vecLengthF(ZHIR_vecSubF(line.b, p)) <= ZHIR_vecLengthF(ZHIR_vecSubF(line.a, line.b)))
	{
		return true;
	}
	return false;
}
bool ZHIR_isOnLine(const ZHIR_Line& line, const SDL_Point& p)
{
	if (ZHIR_vecLength(ZHIR_vecSub(line.a, p)) + ZHIR_vecLength(ZHIR_vecSub(line.b, p)) <= ZHIR_vecLength(ZHIR_vecSub(line.a, line.b)))
	{
		return true;
	}
	return false;
}

bool ZHIR_isIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2)
{
	// Уравнение первой прямой
	int A1 = line1.b.y - line1.a.y;
	int B1 = line1.a.x - line1.b.x;
	int C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	int A2 = line2.b.y - line2.a.y;
	int B2 = line2.a.x - line2.b.x;
	int C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	// Проверка пересечения отрезков
	if ((A1 * line2.a.x + B1 * line2.a.y + C1) * (A1 * line2.b.x + B1 * line2.b.y + C1) <= 0 &&
		(A2 * line1.a.x + B2 * line1.a.y + C2) * (A2 * line1.b.x + B2 * line1.b.y + C2) <= 0) {
		return true;
	}
	return false;
}
bool ZHIR_isIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2)
{
	// Уравнение первой прямой
	float A1 = line1.b.y - line1.a.y;
	float B1 = line1.a.x - line1.b.x;
	float C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	float A2 = line2.b.y - line2.a.y;
	float B2 = line2.a.x - line2.b.x;
	float C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	// Проверка пересечения отрезков
	if ((A1 * line2.a.x + B1 * line2.a.y + C1) * (A1 * line2.b.x + B1 * line2.b.y + C1) <= 0 &&
		(A2 * line1.a.x + B2 * line1.a.y + C2) * (A2 * line1.b.x + B2 * line1.b.y + C2) <= 0) {
		return true;
	}
	return false;
}
bool ZHIR_isIntersectFstrict(const ZHIR_LineF& line1, const ZHIR_LineF& line2)
{
	// Уравнение первой прямой
	float A1 = line1.b.y - line1.a.y;
	float B1 = line1.a.x - line1.b.x;
	float C1 = line1.b.x * line1.a.y - line1.a.x * line1.b.y;

	// Уравнение второй прямой
	float A2 = line2.b.y - line2.a.y;
	float B2 = line2.a.x - line2.b.x;
	float C2 = line2.b.x * line2.a.y - line2.a.x * line2.b.y;

	// Проверка пересечения отрезков
	if ((A1 * line2.a.x + B1 * line2.a.y + C1) * (A1 * line2.b.x + B1 * line2.b.y + C1) < 0 &&
		(A2 * line1.a.x + B2 * line1.a.y + C2) * (A2 * line1.b.x + B2 * line1.b.y + C2) < 0) {
		return true;
	}
	return false;
}

ZHIR_LineF cutLineInRect(ZHIR_LineF line, SDL_FRect rect)
{
	SDL_FPoint A = { 0,0 };
	SDL_FPoint B = { 0,0 };
	if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
	{
		A = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });

		if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if ((ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} })))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	else if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
	{
		A = ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });

		if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
			B = ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
		else if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
		else if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
			B = ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
	}
	return { A,B };
}

SDL_FPoint lineRectIntersection(ZHIR_LineF line, SDL_FRect rect)
{
	if (ZHIR_isIntersectFstrict(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } }))
	{
		return ZHIR_findIntersectF(line, { { rect.x, rect.y }, {rect.x + rect.w, rect.y } });
	}
	if (ZHIR_isIntersectFstrict(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { { rect.x + rect.w, rect.y }, {rect.x + rect.w, rect.y + rect.h} });
	}
	if (ZHIR_isIntersectFstrict(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { {rect.x + rect.w, rect.y + rect.h}, {rect.x, rect.y + rect.h} });
	}
	if (ZHIR_isIntersectFstrict(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} }))
	{
		return ZHIR_findIntersectF(line, { {rect.x, rect.y}, {rect.x, rect.y + rect.h} });
	}
	return { 0,0 };
}

int ZHIR_rectOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2)
{
	int left = SDL_max(rect1.x, rect2.x);
	int bottom = SDL_max(rect1.y, rect2.y);
	int right = SDL_min(rect1.x + rect1.w, rect2.x + rect2.w);
	int top = SDL_min(rect1.y + rect1.h, rect2.y + rect2.h);

	int width = right - left;
	int height = top - bottom;

	if (width < 0 || height < 0)
		return 0;

	return width * height;
}

#pragma endregion //VectorMath & Geometry 

#pragma region Color
//конвертация из SDL_Color в ZHIR_ColorF для удобной инициализации начального цвета градиента
ZHIR_ColorF ZHIR_colorToColorF(const SDL_Color& c)
{
	ZHIR_ColorF cf = { (float)c.r, (float)c.g, float(c.b), (float)c.a };
	return cf;
}

//линейная интерполяция отдельного компонента цвета туда-обратно
static void soloColorChange(float& val, float c1val, float c2val, bool& back, const float& t)
{
	c1val = ZHIR_slapF(c1val, 0, 255);
	c2val = ZHIR_slapF(c2val, 0, 255);
	if (c1val > c2val)
		ZHIR_swapF(c1val, c2val);
	if (val >= c2val)
		back = true;
	if (val <= c1val)
		back = false;
	float minus = -((int)back - 1 * (int)(!back));
	val += minus * t * (c2val - c1val);
	val = ZHIR_slapF(val, 0, 255);
}

//линейная интерполяция отдельного компонента цвета
static void soloColorOneWay(float& val, float c1val, float c2val, bool& back, const float& t)
{
	if (back)
		return;
	c1val = ZHIR_slapF(c1val, 0, 255);
	c2val = ZHIR_slapF(c2val, 0, 255);
	if (c1val > c2val)
		ZHIR_swapF(c1val, c2val);
	if (val >= c2val)
		back = true;
	val += t * (c2val - c1val);
	val = ZHIR_slapF(val, 0, 255);
}

//линейная интерполяция цвета туда-обратно
void ZHIR_colorTransition(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t)
{
	soloColorChange(color.r, grad.color1.r, grad.color2.r, grad.rback, t);
	soloColorChange(color.g, grad.color1.g, grad.color2.g, grad.gback, t);
	soloColorChange(color.b, grad.color1.b, grad.color2.b, grad.bback, t);
	soloColorChange(color.a, grad.color1.a, grad.color2.a, grad.aback, t);
}

//линейная интерполяция цвета
void ZHIR_colorOneWay(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t)
{
	soloColorOneWay(color.r, grad.color1.r, grad.color2.r, grad.rback, t);
	soloColorOneWay(color.g, grad.color1.g, grad.color2.g, grad.gback, t);
	soloColorOneWay(color.b, grad.color1.b, grad.color2.b, grad.bback, t);
	soloColorOneWay(color.a, grad.color1.a, grad.color2.a, grad.aback, t);
}
#pragma endregion //Color

#pragma region Draw

//Добавляет координаты x, y к координатам центра (смещения) и записывает результат в sx и sy соответсвенно
static void mathCoordsToScreen(float x, float y, int centerx, int centery, int& sx, int& sy)
{
	sx = round(centerx + x);
	sy = round(centery + y);
}
static void mathCoordsToScreenF(float x, float y, float centerx, float centery, float& sx, float& sy)
{
	sx = centerx + x;
	sy = centery + y;
}

//Рисует круг, если r = 0 точку, если r < 0 ничего не делает 
void ZHIR_drawCircle(const SDL_Point& center, const float r)
{
	if (r < 0)
		return;
	if (r == 0.f)
	{
		SDL_RenderDrawPoint(ren, center.x, center.y);
		return;
	}

	float alpha = 0.f;
	int point_count = r * CIRCLE_PRECISION;
	SDL_Point* points = (SDL_Point*)malloc(sizeof(SDL_Point) * (point_count + 1));

	for (int i = 0; i < point_count; i++)
	{
		alpha += 2.0 * M_PI / point_count;
		mathCoordsToScreen(r * cos(alpha), r * sin(alpha), center.x, center.y, points[i].x, points[i].y);
	}
	points[point_count] = points[0];
	SDL_RenderDrawLines(ren, points, point_count + 1);
	free(points);
}
void ZHIR_drawCircleF(const SDL_FPoint& center, const float r)
{
	if (r < 0)
		return;
	if (r == 0.f)
	{
		SDL_RenderDrawPointF(ren, center.x, center.y);
		return;
	}

	float alpha = 0.f;
	int point_count = r * CIRCLE_PRECISION;
	SDL_FPoint* points = (SDL_FPoint*)malloc(sizeof(SDL_FPoint) * (point_count + 1));

	for (int i = 0; i < point_count; i++)
	{
		alpha += 2.0 * M_PI / point_count;
		mathCoordsToScreenF(r * cos(alpha), r * sin(alpha), center.x, center.y, points[i].x, points[i].y);
	}
	points[point_count] = points[0];
	SDL_RenderDrawLinesF(ren, points, point_count + 1);
	free(points);
}

//Рисует триугольник на трех точках
void ZHIR_drawTriangle(const SDL_Point& p1, const SDL_Point& p2, const SDL_Point& p3)
{
	SDL_RenderDrawLine(ren, p1.x, p1.y, p2.x, p2.y);
	SDL_RenderDrawLine(ren, p1.x, p1.y, p3.x, p3.y);
	SDL_RenderDrawLine(ren, p2.x, p2.y, p3.x, p3.y);
}

//Рисует линию используя ZHIR_Line
void ZHIR_drawLine(const ZHIR_Line& line)
{
	SDL_RenderDrawLine(ren, line.a.x, line.a.y, line.b.x, line.b.y);
}
void ZHIR_drawLineF(const ZHIR_LineF& line)
{
	SDL_RenderDrawLineF(ren, line.a.x, line.a.y, line.b.x, line.b.y);
}

////???
//void ZHIR_drawLineBoldF(const ZHIR_LineF& line, int thickness, float prec = 0.5f)
//{
//	SDL_FPoint normVec = ZHIR_vecNormal(ZHIR_vecSubF(line.a, line.b));
//	normVec = ZHIR_rotateOnDegreeF(normVec, { 0,0 }, 90);
//	ZHIR_LineF bold = line;
//	
//	float thick = 0;
//	float flip = -1;
//	for (int i = 0; i < thickness; i++, thick += prec, flip *= -1)
//	{
//		bold.a = ZHIR_vecSumF(line.a, ZHIR_vecMultF(normVec, thick * flip));
//		bold.b = ZHIR_vecSumF(line.b, ZHIR_vecMultF(normVec, thick * flip));
//		ZHIR_drawLineF(bold);
//	}
//}
//
//void SDL_drawLineF(ZHIR_LineF line)
//{
//	SDL_RenderDrawLineF(ren, line.a.x, line.a.y, line.b.x, line.b.y);
//}
#pragma endregion //Draw

#pragma region Timer

//Возвращает правду только в момент когда кончилось время
bool ZHIR_timerTickDown(Timer& timer, float delta)
{
	if (!timer.active)
		return false;

	timer.time -= delta;
	if(timer.time <= 0)
	{ 
		timer.active = false;
		timer.time = 0;
		return true;
	}
	return false;
}

#pragma endregion //Timer

#pragma region Menu

bool ZHIR_pointInRect(const SDL_FPoint& p, const SDL_Rect& rect)
{
	if (p.x >= rect.x && p.x <= rect.x + rect.w && p.y >= rect.y && p.y <= rect.y + rect.h)
		return true;

	return false;
}

#pragma endregion //Menu