#pragma once
#ifndef _RENDER_H_
#define _RENDER_H_

#include "Structs.h"
#include "SDLstuff.h"

void lineRender(const ZHIR_LineF* linesArr, const Door* doorsArr, int linesArrSize, int doorsArrSize);
void renderImage(const Sprite* sprite, const SDL_FRect& fullRect, const SDL_FRect& cutRect);
void entityRender(Entity** entityArr, int entityArrSize, const ZHIR_LineF* linesArr, int linesArrSize);
void enemyPreRender(Entity** entityArr, int entityArrSize);

void minimap(const ZHIR_LineF* linesArr, int linesArrSize, Entity** entityArr, int entityArrSize);
void smokeRender(const ZHIR_LineF* smklines, int smklinesSize, const ZHIR_LineF* linesArr, int linesArrSize);

#endif