#pragma once
#ifndef _SDL_STUFF_H_
#define _SDL_STUFF_H_
#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
/*
* �������� VC++/���������� ��������
* �������� VC++/�������� ���������
* �����������/����/�������������� �����������

SDL2.lib
SDL2main.lib
SDL2_ttf.lib
SDL2_image.lib

int main(int argc, char* argv[])

SDL_Rect r = { x, y, w, h };
SDL_SetRenderDrawColor(ren, r, g, b, alpha); //����� �����
SDL_RenderDrawRect(ren, &r); //���������� �������� ��������������
SDL_RenderFillRect(ren, &r); //���������� ����������� �������������
SDL_Delay(ms); //�������� � ��
SDL_RenderPresent(ren); //������� �� ����� �� ��� �� ������� (������� �� ������)
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

//��������������� SDL, ���� � ���������
void deInit(int error);
//������������� SDL, ���� � ���������
void Init();
#pragma endregion //INIT

#pragma region BasicMath

//��������� ����� ����� �� a �� b
int ZHIR_random(int a, int b);

//�������� ������������ �� a �� � � ����� t (t - ������� �� ������� ����� a � b) 
//t ����� ��������� �� delta
float ZHIR_lerp(float a, float b, float t);

//������ ������� a � b
void ZHIR_swap(int& a, int& b);
void ZHIR_swapF(float& a, float& b);

//������������ ����� � ���������� �� a �� b 
int ZHIR_slap(int ch, int a, int b);
float ZHIR_slapF(float ch, float a, float b);

#pragma endregion //BasicMath

#pragma region VectorMath & Geometry

//������� ������� ����� �������
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

//��������� ����� ������� �������� ����� ������������
float ZHIR_vecLength(const SDL_Point& vec);
float ZHIR_vecLengthF(const SDL_FPoint& vec);

//���������� ������ ������������ � ���������� �������� ���� �������
SDL_Point ZHIR_vecSum(const SDL_Point& a, const SDL_Point& b);
SDL_FPoint ZHIR_vecSumF(const SDL_FPoint& a, const SDL_FPoint& b);

//���������� ������ ������������ � ���������� ��������� ���� �������
SDL_Point ZHIR_vecSub(const SDL_Point& a, const SDL_Point& b);
SDL_FPoint ZHIR_vecSubF(const SDL_FPoint& a, const SDL_FPoint& b);

//���������� ���������� ������
SDL_FPoint ZHIR_vecNormal(const SDL_FPoint& a);

//�������� ������ �� �����
SDL_Point ZHIR_vecMult(const SDL_Point& a, int b);
SDL_FPoint ZHIR_vecMultF(const SDL_FPoint& a, float b);

//��������� ������������ ���� ��������
int ZHIR_vecMultScalar(const SDL_Point& a, const SDL_Point& b);
float ZHIR_vecMultScalarF(const SDL_FPoint& a, const SDL_FPoint& b);

//������������ ����� point �� ���� ������������ rotationPoint
SDL_Point ZHIR_rotateOnDegree(const SDL_Point& point, const SDL_Point& rotationPoint, const float angle);
SDL_FPoint ZHIR_rotateOnDegreeF(const SDL_FPoint& point, const SDL_FPoint& rotationPoint, const float angle);

//��������� ����� �� ����� � �������
bool ZHIR_isOnLine(const ZHIR_Line& line, const SDL_Point& p);
bool ZHIR_isOnLineF(const ZHIR_LineF& line, const SDL_FPoint& p);

//��������� 2 ����� �� �����������
bool ZHIR_isIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2);
bool ZHIR_isIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//���������� ����� ����������� ���� ��������
SDL_Point ZHIR_findIntersect(const ZHIR_Line& line1, const ZHIR_Line& line2);
SDL_FPoint ZHIR_findIntersectF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//������� ���� ����� ��������� (������������� ��� ������ ����� �� 0 �� 180)
int ZHIR_vecFindAngle(const SDL_Point& vec1, const SDL_Point& vec2);
float ZHIR_vecFindAngleF(const SDL_FPoint& vec1, const SDL_FPoint& vec2);

//������� ���� ����� ������� (������ ZHIR_vecFindAngle)
int ZHIR_findIntersectAngle(const ZHIR_Line& line1, const ZHIR_Line& line2);
float ZHIR_findIntersectAngleF(const ZHIR_LineF& line1, const ZHIR_LineF& line2);

//������� ������ ���� ����� ������� (�� 0 �� 360)
int ZHIR_vecFindAngleFull(const SDL_Point& vec1, const SDL_Point& vec2);
float ZHIR_vecFindAngleFullF(const SDL_FPoint& vec1, const SDL_FPoint& vec2);

//���������� �������� ����� p �� ����� line
SDL_Point ZHIR_dotProjLine(const ZHIR_Line& line, const SDL_Point& p);
SDL_FPoint ZHIR_dotProjLineF(const ZHIR_LineF& line, const SDL_FPoint& p);

//���������� ������� ����������� ��������������� ��� ���� ���� �� ������������
int ZHIR_rectOverlap(const SDL_Rect& rect1, const SDL_Rect& rect2);

//���������� ����������� (����������) �� SDL_Frect � SDL_Rect 
SDL_Rect ZHIR_FRectToRect(const SDL_FRect& rect);

#pragma endregion //VectorMath & Geometry 

#pragma region Color
//��������� ����� � (float) ����������
struct ZHIR_ColorF
{
	float r;
	float g;
	float b;
	float a;
};

//��������� ���������
struct ZHIR_Gradient
{
	const SDL_Color color1;
	const SDL_Color color2;
	bool rback = false;
	bool gback = false;
	bool bback = false;
	bool aback = false;
};

//����������� �� SDL_Color � ZHIR_ColorF ��� ������� ������������� ���������� ����� ���������
ZHIR_ColorF ZHIR_colorToColorF(const SDL_Color& c);

//�������� ������������ ����� ����-�������
//t ����� ��������� �� delta
void ZHIR_colorTransition(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t);

//�������� ������������ �����
//t ����� ��������� �� delta
void ZHIR_colorOneWay(ZHIR_ColorF& color, ZHIR_Gradient& grad, const float t);

#pragma endregion //Color

#pragma region Draw
//��� ���� ��� ������ ������. 0.1 - ����� ����, 2 - ����� �����
const double CIRCLE_PRECISION = 1;

//������ ����, ���� r = 0 �����, ���� r < 0 ������ �� ������ 
void ZHIR_drawCircle(const SDL_Point& center, const float r);
void ZHIR_drawCircleF(const SDL_FPoint& center, const float r);

//������ ����������� �� ���� ������
void ZHIR_drawTriangle(const SDL_Point& p1, const SDL_Point& p2, const SDL_Point& p3);

//������ ����� ��������� ZHIR_Line
void ZHIR_drawLine(const ZHIR_Line& line);
void ZHIR_drawLineF(const ZHIR_LineF& line);

void ZHIR_drawLineBoldF(const ZHIR_LineF& line, int thickness, float prec);

#pragma endregion //Draw

#endif