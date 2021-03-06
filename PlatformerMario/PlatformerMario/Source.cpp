#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <iostream>

//header includes
#include "Constants.h"
#include "Texture2D.h"
#include "Commons.h"
#include "GameScreen.h"
#include "GameScreenManager.h"

using namespace std;

SDL_Window* g_window = nullptr;
SDL_Renderer* g_renderer = nullptr;
GameScreenManager* game_screen_manager;

Uint32 g_old_time;

void LoadMusic(string file);

bool InitSDL();
void CloseSDL();

bool Update();
void Render();

int main(int argc, char* args[])
{
	if (InitSDL())
	{
		TTF_Init();
		game_screen_manager = new GameScreenManager(g_renderer, SCREEN_MENU);

		g_old_time = SDL_GetTicks();
		
		bool quit = false;

		while (!quit)
		{
			Render();
			quit = Update();
		}

		TTF_Quit();
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
			if (!(IMG_Init(imageFlags) & imageFlags))
			{
				cout << "SDL_Image could not initialize.  Error: " << SDL_GetError();
			}
		}
		else
		{
			cout << "Renderer could not initialize.  Error: " << SDL_GetError();
		}
	}

	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
	{
		cout << "Mixer could not init.  Error: " << Mix_GetError();
		return false;
	}

	return true;
}

void CloseSDL()
{
	SDL_DestroyWindow(g_window);
	g_window = nullptr;

	SDL_DestroyRenderer(g_renderer);
	g_renderer = nullptr;

	delete(game_screen_manager);
	game_screen_manager = nullptr;

	IMG_Quit();
	SDL_Quit();
}

bool Update()
{
	Uint32 new_time = SDL_GetTicks();

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

	float delta = (float)new_time - g_old_time;
	g_old_time = new_time;

	//cap to framerate, avoiding rounding errors with floating point.
	if (delta < TARGET_MILLISEC)
	{
		SDL_Delay(TARGET_MILLISEC - delta);
		game_screen_manager->Update(TARGET_MILLISEC / 1000.0f, e);
	}
	else
	{
		game_screen_manager->Update(delta / 1000.0f, e);
	}	


	return false;
}

void Render()
{
	SDL_SetRenderDrawColor(g_renderer, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderClear(g_renderer);

	game_screen_manager->Render();

	//update the screen
	SDL_RenderPresent(g_renderer);
}