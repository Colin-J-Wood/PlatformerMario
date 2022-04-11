#pragma once
#include "Character.h"

class Luigi :
    public Character
{
public:
	Luigi();
	Luigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Luigi();

	void Update(float deltaTime, SDL_Event e, LevelMap* map) override;
	bool GetCeilingHit() { return hit_ceiling; };
	void LoseLife() { m_lives--; };
	int GetLivesRemaining() { return m_lives; };

private:
	bool hit_ceiling = false;
	float m_death_elapsed = 0.0f;
	int m_lives = 5;
};

