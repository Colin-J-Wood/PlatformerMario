#include "Koopa.h"

using namespace std;

Koopa::Koopa(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position, FACING start_facing, Vector2D target_speed) : Character(renderer, imagePath, start_position, map)
{
	m_facing_direction = start_facing;
	m_target_velocity = target_speed;
	m_position = start_position;
	m_injured = false;

	if (!m_texture->LoadFromFile(imagePath))
	{
		cout << "Failed to load character texture!" << endl;
	}

	m_single_sprite_w = m_texture->GetWidth() / 2;
	m_single_sprite_h = m_texture->GetHeight();
}

Koopa::~Koopa()
{
	m_renderer = nullptr;
}

void Koopa::FlipRightwayUp()
{
	//swap directions
	if (m_facing_direction = FACING_RIGHT)
	{
		m_facing_direction = FACING_LEFT;
	}
	else
	{
		m_facing_direction = FACING_RIGHT;
	}
	m_injured = false;

	KoopaJump();
}

void Koopa::TakeDamage()
{
	m_injured = true;
	m_injured_time = KOOPA_INJURED_TIME;

	KoopaJump();
}

void Koopa::KoopaJump()
{
	if (!m_jumping)
	{
		m_velocity.y = -m_target_velocity.y;
		m_jumping = true;
		m_canJump = false;
	}
}

void Koopa::Render()
{
	//left position of the sprite we want.
	int left = 0.0f;

	//if injured, move to the second image of the sprite sheet
	if (m_injured) left = m_single_sprite_w;

	//get the portion of the sprite sheet we want to draw.
	SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };

	//determine where it should be drawn
	SDL_Rect destRect = { (int)(m_position.x), (int)(m_position.y), m_single_sprite_w, m_single_sprite_h };

	//then draw it facing the correct direction
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(portion_of_sprite, destRect, SDL_FLIP_HORIZONTAL);
	}
}

void Koopa::Update(float deltaTime, SDL_Event e, LevelMap* map)
{
	Character::Update(deltaTime, e, map);

	if (!m_injured)
	{
		//enemy is not injured, so move
		if (m_facing_direction == FACING_LEFT)
		{
			m_moving_left = true;
			m_moving_right = false;
		}
		else if (m_facing_direction == FACING_RIGHT)
		{
			m_moving_right = true;
			m_moving_left = false;
		}
	}
	else
	{
		//don't move whilst injured.
		m_moving_left = false;
		m_moving_right = false;

		//count down the injured time.
		m_injured_time -= deltaTime;

		if (m_injured_time <= 0.0) FlipRightwayUp();
	}
}