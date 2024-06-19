#include "Input.h"

SDL_FPoint MousePos = { 0,0 };
SDL_FPoint MousePosRel = { 0,0 };
SDL_FPoint lastMousePosRel = { 0,0 };

bool input_MOUSELEFTCLICK = false;
bool input_MOUSERIGHTCLICK = false;
bool input_MOUSEMIDDLECLICK = false;
bool input_MOUSEWHEELDOWN = false;
bool input_MOUSEWHEELUP = false;

int mode = LINES;

//bool input_UP = false;
//bool input_DOWN = false;
//bool input_LEFT = false;
//bool input_RIGHT = false;
void inputUpdate()
{
	/*InputDir.x = (float)input_RIGHT - (float)input_LEFT;
	InputDir.y = (float)input_DOWN - (float)input_UP;
	InputDir = ZHIR_vecNormal(InputDir);*/

	//if (MousePosRel.x == lastMousePosRel.x && MousePosRel.y == lastMousePosRel.y)
	//	MousePosRel = { 0,0 };
	//lastMousePosRel = MousePosRel;
}