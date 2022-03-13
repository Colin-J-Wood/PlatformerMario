#include "Character.h"

using namespace std;

Character::Character()
{

}

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position, LevelMap* map)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(renderer);
	m_facing_direction = FACING_RIGHT;
	m_level_map = map;
	m_alive = true;

	if (!m_texture->LoadFromFile(imagePath))
	{
		cout << "Failed to load character texture!" << endl;
	}
}

Character::~Character()
{
	m_renderer = nullptr;
}

void Character::Render()
{
	if (m_facing_direction == FACING_RIGHT)
	{
		m_texture->Render(m_position, SDL_FLIP_NONE);
	}
	else
	{
		m_texture->Render(m_position, SDL_FLIP_HORIZONTAL);
	}
}

void Character::Update(float deltaTime, SDL_Event e, LevelMap* map)
{
	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}

	if (m_moving_right)
	{
		MoveRight(deltaTime);
	}

	//if neither are moving, set velocity to 0.
	if (!m_moving_left && !m_moving_right)
	{
		m_velocity.x = 0.0f;
	}

	//give a new map if you want to switch map references
	m_level_map = map;
	CheckBlocks(map);

	//positionally correct if there's a tile to left or right, regardless of velocity.
	//then set horizontal velocity to 0 if trying to move into that block
	if ((m_blockResult[LEFT] != AIR))
	{
		m_position.x += DEFAULT_TILESIZE - ((int)m_position.x % DEFAULT_TILESIZE);

		if (m_velocity.x < 0.0f) m_velocity.x = 0.0f;
	}
	if ((m_blockResult[RIGHT] != AIR))
	{
		m_position.x -= (int)(m_position.x + m_texture->GetWidth()) % DEFAULT_TILESIZE;

		if (m_velocity.x > 0.0f) m_velocity.x = 0.0f;
	}

	//make the player go immediately downward if they hit a tile above them, unlesss it's a platform
	if ((m_blockResult[UP] == FLOOR) && (m_velocity.y < 0.0f)) m_velocity.y = 0.0f;
	//then react to the floor in here.
	//don't allow another jump until the jump has stopped moving upward.
	if (m_blockResult[DOWN] == AIR)
	{
		AddGravity(GRAVITY, deltaTime);
	}
	else
	{
		//jump only resets if the player was falling.
		if (m_velocity.y >= 0.0f)
		{
			m_canJump = true;
			m_jumping = false;
			m_velocity.y = 0.0f;
		}

		//position correction.
		m_position.y -= ((int)(m_position.y + m_texture->GetHeight()) % DEFAULT_TILESIZE);
	}

	if (m_ctrlJump && !m_jumping && m_canJump)
	{
		Jump(deltaTime);
	}

	//apply physics velocity results
	m_position.x += m_velocity.x;
	m_position.y += m_velocity.y;
}

void Character::MoveLeft(float deltaTime)
{
	m_facing_direction = FACING_LEFT;

	//cout << "Moving left" << endl;

	m_velocity.x = -m_target_velocity.x * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	m_facing_direction = FACING_RIGHT;

	//cout << "Moving right" << endl;

	m_velocity.x = m_target_velocity.x * deltaTime;
}

void Character::Jump(float deltaTime)
{
	//target velocity y is the jump speed
	m_velocity.y = -m_target_velocity.y * deltaTime;
	m_jumping = true;
	m_canJump = false;
}

//add a fixed gravity speed to this character
//as fixed falling is more responsive for a platformer.
void Character::AddGravity(float strength, float deltaTime)
{
	m_velocity.y += (int)strength * deltaTime;
}

//check for blocks above, below, to the right and left of this character
//and set the array accordingly so tile collision blocks properly.
//don't invoke this method if you want the object to clip through tiles.
void Character::CheckBlocks(LevelMap* map)
{
	int centralX_position = (int)(m_position.x + (m_texture->GetWidth() * 0.5)) / DEFAULT_TILESIZE;
	int centralY_position = (int)(m_position.y + (m_texture->GetHeight() * 0.5)) / DEFAULT_TILESIZE;
	int foot_position = (int)(m_position.y + m_texture->GetHeight()) / DEFAULT_TILESIZE;
	int head_position = (int)(m_position.y) / DEFAULT_TILESIZE;
	int right_position = (int)(m_position.x + m_texture->GetWidth()) / DEFAULT_TILESIZE;
	int left_position = (int)(m_position.x) / DEFAULT_TILESIZE;

	//next, test the tiles around that tile position in all four directions.
	//this data can be used later by other methods.
	m_blockResult[DOWN] = map->GetTileAt(centralX_position, foot_position);
	m_blockResult[UP] = map->GetTileAt(centralX_position, head_position);
	m_blockResult[LEFT] = map->GetTileAt(left_position, centralY_position);
	m_blockResult[RIGHT] = map->GetTileAt(right_position, centralY_position);
}