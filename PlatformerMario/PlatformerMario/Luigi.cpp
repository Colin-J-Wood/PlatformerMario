#include "Luigi.h"

Luigi::Luigi()
{

}

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

	m_width = m_texture->GetWidth();
	m_height = m_texture->GetHeight();

	m_target_velocity = PLAYER_SPEED;

	m_jump_sound = new Sound("Sound/player_jump.mp3");
}

Luigi::~Luigi()
{
	m_renderer = nullptr;
}

void Luigi::Update(float deltaTime, SDL_Event e, LevelMap* map)
{
	//the only difference in mario's update is his control scheme.
	//don't do this if the player is dead.
	if (m_alive) Character::Update(deltaTime, e, map);
	//don't respawn if the player is game overed
	if (!m_alive && (m_lives > 0)) m_death_elapsed += deltaTime;
	if (m_death_elapsed > PLAYER_DEATH_TIME) m_death_elapsed = PLAYER_DEATH_TIME;

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
		case SDLK_SPACE:
			//falling is only still true if tile checks force gravity to stay in effect.
			m_ctrlJump = true;
			//reset alive if the death animation has finished.
			if (!m_alive && (m_death_elapsed >= PLAYER_DEATH_TIME))
			{
				m_alive = true;
				m_death_elapsed = 0.0f;
			}

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
		case SDLK_SPACE:
			//stop jumping immediately if the jump key was released
			//todo
			m_ctrlJump = false;

			break;
		}

		break;
	}

	//detect if we hit the ceiling recently.
	hit_ceiling = (m_blockResult[UP] != AIR);

	//play a death "animation"
	if (!m_alive && (m_death_elapsed <= PLAYER_DEATH_TIME / 2))
	{
		//be outside the top of the screen until half of the timer has passed.
		m_position.y = -GetHeight();
		m_position.x = 256;
	}
	else if (!m_alive && (m_death_elapsed > PLAYER_DEATH_TIME / 2))
	{
		//interpolate the animation based on how far along the timer is in the second half of its cycle.
		//the player should end up on the very top of the screen at the end of the animation.
		float start_time = PLAYER_DEATH_TIME / 2;
		float time_to_take = PLAYER_DEATH_TIME / 2;
		float normal = (m_death_elapsed - start_time) / time_to_take; //the normalized version of the timer, from 0.0 to 1.0.
		float origin = -GetHeight();
		float destination = -7.0f;
		float distance = destination - origin;
		float delta = origin + (distance * normal); //interpolate by adding to the origin.

		m_position.y = delta;
	}
}