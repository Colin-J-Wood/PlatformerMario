#include "Luigi.h"

Luigi::Luigi(SDL_Renderer* renderer, string imagePath, Vector2D start_position)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(renderer);
	m_facing_direction = FACING_RIGHT;
	m_collision_radius = PLAYER_COLLISION_RADIUS;

	if (!m_texture->LoadFromFile(imagePath))
	{
		cout << "Failed to load character texture!" << endl;
	}

	m_target_velocity = PLAYER_SPEED;
}

Luigi::~Luigi()
{
	m_renderer = nullptr;
}

void Luigi::Update(float deltaTime, SDL_Event e)
{
	Character::Update(deltaTime, e);

	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = true;

			break;
		case SDLK_d:
			m_moving_right = true;

			break;
		}

		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_a:
			m_moving_left = false;

			break;
		case SDLK_d:
			m_moving_right = false;

			break;
		}
		break;
	}
}