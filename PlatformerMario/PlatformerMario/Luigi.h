#pragma once
#include "Character.h"
class Luigi :
    public Character
{
public:
	Luigi();
	Luigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Luigi();

	virtual void Update(float deltaTime, SDL_Event e, LevelMap* map);
	
};

