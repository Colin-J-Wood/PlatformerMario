#pragma once
#include "Character.h"
class Mario :
    public Character
{
public:
	Mario(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Mario();

	void Update(float deltaTime, SDL_Event e);
};

