#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

//header includes
#include "Constants.h"
#include "Texture2D.h"
#include "Commons.h"

using namespace std;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
Texture2D* g_texture = nullptr;

bool InitSDL();
void CloseSDL();

bool Update();

void Render();

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		bool quit = false;

		while (!quit)
		{
			Render();
			quit = Update();
		}
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

		g_renderer = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);

		if (g_renderer != nullptr)
		{
			//initialize PNG loading
			int imageFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imageFlags)& imageFlags))
			{
				cout << "SDL_Image could not initialize.  Error: " << SDL_GetError();
				return false;
			}
		}
		else
		{
			cout << "Renderer could not initialize.  Error: " << SDL_GetError();
			return false;
		}

		//Load the background texture
		g_texture = new Texture2D(g_renderer);
		if (!g_texture->LoadFromFile("Images/test.bmp"))
		{
			return false;
		}
	}

	return true;
}

void CloseSDL()
{
	//release the texture
	delete g_texture;
	g_texture = nullptr;

	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	SDL_Event e;

	//cout << "Ticking..." << endl;

	SDL_PollEvent(&e);

	switch (e.type)
	{
		//click the 'X' to quit
	case SDL_QUIT:
		return true;
		break;
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			return true;
			break;
		}
	}

	return false;
}

void Render()
{
	SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(g_renderer);

	g_texture->render(Vector2D(), SDL_FLIP_NONE);

	//update the screen
	SDL_RenderPresent(g_renderer);
}