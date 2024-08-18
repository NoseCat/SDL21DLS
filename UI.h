#pragma once
#ifndef _UI_H_
#define _UI_H_

#include "SDLstuff.h"
#include "Input.h"
#include "Structs.h"

#pragma region TOOLS

void texturefromtext(Button& button, const char* text, TTF_Font* my_font, SDL_Color fore_color, SDL_Color back_color);
void updateButton(Button& button);

#pragma endregion //TOOLS

#pragma region MENU_HARDCODE

void minimap(const ZHIR_LineF* linesArr, int linesArrSize, Entity** entityArr, int entityArrSize);

#pragma endregion //MENU-HARDCODE


#endif