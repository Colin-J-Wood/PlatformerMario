#pragma once
#include "Character.h"
class Luigi :
    public Character
{
public:
	Luigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Luigi();

	void Update(float deltaTime, SDL_Event e);
};

