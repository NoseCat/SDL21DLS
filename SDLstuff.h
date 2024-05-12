#pragma once
#ifndef _SDL_STUFF_H_
#define _SDL_STUFF_H_
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
/*
* Каталоги VC++/Включаемые каталоги
* Каталоги VC++/Каталоги библиотек
* Компоновщик/Ввод/Дополнительные зависимости

SDL2.lib
SDL2main.lib
SDL2_ttf.lib
SDL2_image.lib

int main(int argc, char* argv[])

SDL_Rect r = { x, y, w, h };
SDL_SetRenderDrawColor(ren, r, g, b, alpha); //выбор цвета
SDL_RenderDrawRect(ren, &r); //нарисовать периметр прямоугольника
SDL_RenderFillRect(ren, &r); //нарисовать заполненный прямоугольник
SDL_Delay(ms); //задержка в мс
SDL_RenderPresent(ren); //вывести на экран то что мы сделали (вывести из буфера)
SDL_RenderDrawLine(ren, x1, y1, x2, y2);
SDL_RenderClear(ren);
*/

#pragma region INIT
extern int WIN_WIDTH;
extern int WIN_HEIGHT;
extern SDL_Point WIN_CENTER;

extern float FRAMES;
extern SDL_Window* win;
extern SDL_Renderer* ren;

//Деинициализация SDL, окна и рендерера
void deInit(int error);
//Инициализация SDL, окна и рендерера
void Init();
#pragma endregion //INIT

#pragma region BasicMath

//случайное целое число от a до b
int ZHIR_random(int a, int b);

//линейная интерполяция от a до б с шагом t (t - процент от разницы между a и b) 
//t нужно домножать на delta
float ZHIR_lerp(float a, float b, float t);

//меняет местами a и b
void ZHIR_swap(int& a, int& b);
void ZHIR_swapF(float& a, float& b);

//ограничивает число в промежутке от a до b 
int ZHIR_slap(int ch, int a, int b);
float ZHIR_slapF(float ch, float a, float b);

#pragma endregion //BasicMath

#pragma region VectorMath & Geometry

//Отрезок заданый двумя точками
struct ZHIR_Line
{
	SDL_Point a;
	SDL_Point b;
};
struct ZHIR_LineF
{
	SDL_FPoint a;
	SDL_FPoint b;
};

//возвращет длину вектора заданого двумя координатами
float ZHIR_vecLength(const SDL_Point& vec);
float ZHIR_vecLengthF(const SDL_FPoint& vec);

//Возвращает вектор получившийся в результате сложения двух веторов
SDL_Point ZHIR_vecSum(const SDL_Point& a, const SDL_Point& b);
SDL_FPoint ZHIR_vecSumF(const SDL_FPoint& a, const SDL_FPoint& b);

//Возвращает вектор получившийся в результате вычитания двух веторов
SDL_Point ZHIR_vecSub(const SDL_Point& a, const SDL_Point& b);
SDL_FPoint ZHIR_vecSubF(const SDL_FPoint& a, const SDL_FPoint& b);

//Возвращает нормальный вектор
SDL_FPoint ZHIR_vecNormal(const SDL_FPoint& a);

//Умножает вектор на число
SDL_Point ZHIR_vecMult(const SDL_Point& a, int b);
SDL_FPoint ZHIR_vecMultF(const SDL_FPoint& a, float b);

//Скалярное произведение двух векторов
int ZHIR_vecMultScalar(const SDL_Point& a, const SDL_Point& b);
float ZHIR_vecMultScalarF(const SDL_FPoint& a, const SDL_FPoint& b);

//Поворачивает точку point на угол относительно rotationPoint
SDL_Point ZHIR_rotateOnDegree(const SDL_Point& point, const SDL_Point& rotationPoint, const float angle);
SDL_FPoint ZHIR_rotateOnDegreeF(const SDL_FPoint& point, const SDL_FPoint& rotationPoint, const float angle);

//проверяет лежит ли точка в отрезке
bool ZHIR_isOnLine(const ZHIR_Line& line, const SDL_Point& p);
bool ZHIR_isOnLineF(const ZHIR_LineF& line, const SDL_FPoint& p);

//Проверяет 2 линии на пересечение
bool ZHIR_isIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2);
bool ZHIR_isIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//Возвращает точку пересечения двух отрезков
SDL_Point ZHIR_findIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2);
SDL_FPoint ZHIR_findIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//находит угол между векторами (математически это всегда число от 0 до 180)
int ZHIR_vecFindAngle(const SDL_Point& vec1, const SDL_Point& vec2);
float ZHIR_vecFindAngleF(const SDL_FPoint& vec1, const SDL_FPoint& vec2);

//находит угол между линиями (смотри ZHIR_vecFindAngle)
int ZHIR_findIntersectAngle(const ZHIR_Line& line1, const ZHIR_Line& line2);
float ZHIR_findIntersectAngleF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//находит полный угол между линиями (от 0 до 360)
int ZHIR_vecFindAngleFull(const SDL_Point& vec1, const SDL_Point& vec2);
float ZHIR_vecFindAngleFullF(const SDL_FPoint& vec1, const SDL_FPoint& vec2);

//Возвращает проекцию точки p на линию line
SDL_Point ZHIR_dotProjLine(const ZHIR_Line& line, const SDL_Point& p);
SDL_FPoint ZHIR_dotProjLineF(const ZHIR_LineF& line, const SDL_FPoint& p);

//Возвращает площадь пересечения прямоугольников или ноль если не пересекается
int ZHIR_rectOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

//Правильная конвертация (округление) из SDL_Frect в SDL_Rect 
SDL_Rect ZHIR_FRectToRect(const SDL_FRect& rect);

#pragma endregion //VectorMath & Geometry 

#pragma region Color
//структура цвета с (float) значениями
struct ZHIR_ColorF
{
	float r;
	float g;
	float b;
	float a;
};

//структура градиента
struct ZHIR_Gradient
{
	const SDL_Color color1;
	const SDL_Color color2;
	bool rback = false;
	bool gback = false;
	bool bback = false;
	bool aback = false;
};

//конвертация из SDL_Color в ZHIR_ColorF для удобной инициализации начального цвета градиента
ZHIR_ColorF ZHIR_colorToColorF(const SDL_Color& c);

//линейная интерполяция цвета туда-обратно
//t нужно домножать на delta
void ZHIR_colorTransition(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t);

//линейная интерполяция цвета
//t нужно домножать на delta
void ZHIR_colorOneWay(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t);

#pragma endregion //Color

#pragma region Draw
//Чем выше тем точнее фигуры. 0.1 - очень мало, 2 - очень много
const double CIRCLE_PRECISION = 1;

//Рисует круг, если r = 0 точку, если r < 0 ничего не делает 
void ZHIR_drawCircle(const SDL_Point& center, const float r);
void ZHIR_drawCircleF(const SDL_FPoint& center, const float r);

//Рисует триугольник на трех точках
void ZHIR_drawTriangle(const SDL_Point& p1, const SDL_Point& p2, const SDL_Point& p3);

//Рисует линию используя ZHIR_Line
void ZHIR_drawLine(const ZHIR_Line& line);
void ZHIR_drawLineF(const ZHIR_LineF& line);

void ZHIR_drawLineBoldF(const ZHIR_LineF& line, int thickness, float prec);

#pragma endregion //Draw

#endif