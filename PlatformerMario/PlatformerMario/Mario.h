#pragma once
#include "Luigi.h"

//mario is a direct copy of luigi besides his update
//no I don't mind that being backwards to canon ;p
class Mario :
    public Luigi
{
public:
	Mario(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Mario();
	void Update(float deltaTime, SDL_Event e, LevelMap* map) override;
	bool GetCeilingHit() { return hit_ceiling; };

private:
	bool hit_ceiling = false;
};

