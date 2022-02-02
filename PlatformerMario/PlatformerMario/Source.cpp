#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

//header includes
#include "Constants.h"

using namespace std;

#undef main

SDL_Window* g_window = nullptr;

bool InitSDL();
void CloseSDL();

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		SDL_Delay(5000);
	}

	CloseSDL();

	return 0;
}

bool InitSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)  //edit this as I need more modules.
	{
		cout << "SDL did not initialize.  Error: " << SDL_GetError();
		return false;
	}
	else
	{
		//setup passed, now create a window.
		g_window = SDL_CreateWindow("Games Engine Creation",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN); //this depicts flags for window parameters, such as full screen, being an opengl context, etc

		if (g_window == nullptr)
		{
			//window failed to open
			cout << "Window was not created.  Error: " << SDL_GetError();
			return false;
		}
	}

	return true;
}

void CloseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	IMG_Quit();
	SDL_Quit();
}