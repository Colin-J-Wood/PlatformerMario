#include "Mario.h"

Mario::Mario(SDL_Renderer* renderer, string imagePath, Vector2D start_position)
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

Mario::~Mario()
{
	m_renderer = nullptr;
}

void Mario::Update(float deltaTime, SDL_Event e)
{
	//the only difference in mario's update is his control scheme.
	Character::Update(deltaTime, e);

	switch (e.type)
	{
	case SDL_KEYDOWN:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = true;

			break;
		case SDLK_RIGHT:
			m_moving_right = true;

			break;
		case SDLK_LSHIFT:
			//falling is only still true if tile checks force gravity to stay in effect.
			if (!m_isFalling) m_isJumping = true;

			break;
		}

		break;
	case SDL_KEYUP:
		switch (e.key.keysym.sym)
		{
		case SDLK_LEFT:
			m_moving_left = false;

			break;
		case SDLK_RIGHT:
			m_moving_right = false;

			break;
		case SDLK_LSHIFT:
			//stop jumping immediately if the jump key was released
			if (m_isJumping)
			{
				m_isJumping = false;
				m_isFalling = true;
			}

			break;
		}
		break;
	}
}