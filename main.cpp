﻿#include <iostream>
#include "SDLstuff.h"
#include "mainblock.h"
#include "Input.h"

int main(int argc, char* argv[])
{
	Init();
	srand(time(0));
	system("chcp 1251 > nul");
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	float newtime = 0, lasttime = 0, delta = 1.0f / 1000;
	bool RUN = true;
	SDL_Event ev;

	onStart();

	while (RUN)
	{
		while (SDL_PollEvent(&ev))
		{
			switch (ev.type)
			{
			case SDL_QUIT:
				RUN = false;
				break;

			case SDL_WINDOWEVENT:
				if (ev.window.event == SDL_WINDOWEVENT_RESIZED)
				{
					WIN_WIDTH = ev.window.data1;
					WIN_HEIGHT = ev.window.data2;
					WIN_CENTER = { WIN_WIDTH / 2, WIN_HEIGHT / 2 };
				}
				break;
			case SDL_MOUSEMOTION:
				MousePos.x = ev.motion.x;
				MousePos.y = ev.motion.y;
				MousePosRel.x = ev.motion.xrel;
				MousePosRel.y = ev.motion.yrel;
				break;
			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					input_UP = true;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					input_LEFT = true;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					input_DOWN = true;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					input_RIGHT = true;
					break;
				}
				break;
			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{
				case SDL_SCANCODE_W:
				case SDL_SCANCODE_UP:
					input_UP = false;
					break;
				case SDL_SCANCODE_A:
				case SDL_SCANCODE_LEFT:
					input_LEFT = false;
					break;
				case SDL_SCANCODE_S:
				case SDL_SCANCODE_DOWN:
					input_DOWN = false;
					break;
				case SDL_SCANCODE_D:
				case SDL_SCANCODE_RIGHT:
					input_RIGHT = false;
					break;
				}
				break;
			}
		}

		newtime = SDL_GetTicks();
		delta = (newtime - lasttime) / 1000.0f;
		if (delta < FRAMES)
		{
			SDL_Delay(FRAMES - delta);
			newtime = SDL_GetTicks();
			delta = (newtime - lasttime) / 1000.0f;
		}
		lasttime = newtime;

		eachFrame(delta);

		SDL_RenderPresent(ren);
		SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
		SDL_RenderClear(ren);
	}

	deInit(0);
	return 0;
}