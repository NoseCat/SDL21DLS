#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "Structs.h"
#include "SDLstuff.h"

void lineRender(const ZHIR_LineF* linesArr, int linesArrSize);
void renderImage(const Sprite* sprite, const SDL_FRect& fullRect, const SDL_FRect& cutRect);
void entityRender(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize);
void enemyPreRender(Entity** entityArr, int entityArrSize);

#endif