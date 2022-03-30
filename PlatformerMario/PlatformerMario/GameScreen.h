#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Commons.h"

class GameScreen
{
public:
	GameScreen(SDL_Renderer* renderer);
	GameScreen();
	~GameScreen();

	virtual void Render();
	virtual SCREENS Update(float deltaTime, SDL_Event e);
protected:
	SDL_Renderer* m_renderer;
};

