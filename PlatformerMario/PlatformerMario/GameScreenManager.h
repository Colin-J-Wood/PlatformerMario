#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "GameScreen.h"
#include "Commons.h"

class GameScreenManager
{
public:
	GameScreenManager(SDL_Renderer* renderer, SCREENS startScreen);
	~GameScreenManager();

	void Render();
	void Update(float deltaTime, SDL_Event e);

	void ChangeScreen(SCREENS new_screen);
private:
	SDL_Renderer* m_renderer;
	GameScreen* m_current_screen;
	SCREENS m_screen_selected;
};

