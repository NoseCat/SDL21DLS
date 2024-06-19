#include <iostream>
#include "SDLstuff.h"
#include "mainblock.h"
#include "Input.h"

int main(int argc, char* argv[])
{
	Init();
	srand(time(0));
	system("chcp 1251 > nul");
	SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
	float newtime = 0, lasttime = 0, delta = 1.0f / 1000;
	bool RUN = true;
	SDL_Event ev;

	onStart();

	while (RUN)
	{
		MousePosRel = { 0,0 };
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

			case SDL_MOUSEWHEEL:
				if (ev.wheel.direction > 0)
				{
					input_MOUSEWHEELUP = true;
					input_MOUSEWHEELDOWN = false;
				}
				if (ev.wheel.direction < 0)
				{
					input_MOUSEWHEELUP = false;
					input_MOUSEWHEELDOWN = true;
				}
				if (ev.wheel.direction == 0)
				{
					input_MOUSEWHEELUP = false;
					input_MOUSEWHEELDOWN = false;
				}
				break;

			case SDL_MOUSEBUTTONDOWN:
				if (ev.button.button == SDL_BUTTON_RIGHT)
					input_MOUSERIGHTCLICK = true;
				if (ev.button.button == SDL_BUTTON_LEFT)
					input_MOUSELEFTCLICK = true;
				if (ev.button.button == SDL_BUTTON_MIDDLE)
					input_MOUSEMIDDLECLICK = true;
				break;

			case SDL_MOUSEBUTTONUP:
				if (ev.button.button == SDL_BUTTON_RIGHT)
					input_MOUSERIGHTCLICK = false;
				if (ev.button.button == SDL_BUTTON_LEFT)
					input_MOUSELEFTCLICK = false;
				if (ev.button.button == SDL_BUTTON_MIDDLE)
					input_MOUSEMIDDLECLICK = false;
				break;

			case SDL_KEYDOWN:
				switch (ev.key.keysym.scancode)
				{

				case SDL_SCANCODE_W:
					break;

				case SDL_SCANCODE_1:
					mode = LINES;
					break;

				case SDL_SCANCODE_2:
					mode = ENEMYTYPE1;
					break;

				case SDL_SCANCODE_3:
					mode = ENEMYTYPE2;
					break;

				case SDL_SCANCODE_4:
					mode = ENEMYTYPE3;
					break;

				case SDL_SCANCODE_5:
					mode = DOORRED;
					break;

				case SDL_SCANCODE_6:
					mode = DOORGREEN;
					break;

				case SDL_SCANCODE_7:
					mode = DOORBLUE;
					break;

				case SDL_SCANCODE_8:
					mode = KEYCARDREDm;
					break;

				case SDL_SCANCODE_9:
					mode = KEYCARDGREENm;
					break;

				case SDL_SCANCODE_0:
					mode = KEYCARDBLUEm;
					break;
				}
				break;

			case SDL_KEYUP:
				switch (ev.key.keysym.scancode)
				{

				case SDL_SCANCODE_W:

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

	onEnd();

	deInit(0);
	return 0;
}