#include "Character.h"

using namespace std;

Character::Character()
{

}

Character::Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position)
{
	m_renderer = renderer;
	m_position = start_position;
	m_texture = new Texture2D(renderer);
	m_facing_direction = FACING_RIGHT;

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

void Character::Update(float deltaTime, SDL_Event e)
{
	if (m_moving_left)
	{
		MoveLeft(deltaTime);
	}
	else if (m_moving_right)
	{
		MoveRight(deltaTime);
	}

	if (m_isJumping)
	{
		Jump(deltaTime);
	}
}

void Character::MoveLeft(float deltaTime)
{
	m_facing_direction = FACING_LEFT;

	//cout << "Moving left" << endl;

	m_velocity = Vector2D(-m_target_velocity.x, m_velocity.y);

	m_position.x += m_velocity.x * deltaTime;
}

void Character::MoveRight(float deltaTime)
{
	m_facing_direction = FACING_RIGHT;

	//cout << "Moving right" << endl;

	m_velocity = Vector2D(m_target_velocity.x, m_velocity.y);

	m_position.x += m_velocity.x * deltaTime;
}

void Character::Jump(float deltaTime)
{
	//target velocity y provides target jump speed.
	m_velocity = Vector2D(m_velocity.x, m_target_velocity.y);
	m_jumpTimeElapsed += deltaTime;
	//stop jumping when jump timer has elapsed.
	if (m_jumpTimeElapsed > m_jumpTime)
	{
		cout << "Jumping!" << endl;
		m_jumpTimeElapsed = 0.0f;
		m_isJumping = false;
		m_isFalling = true;
	}

	m_position.y += m_velocity.y * deltaTime;
}

//add a fixed gravity speed to this character
//as fixed falling is more responsive for a platformer.
void Character::AddGravity(float strength, float deltaTime)
{
	if (m_isFalling)
	{
		cout << "Falling!" << endl;
		m_position.y += strength * deltaTime;
	}
}

//check for blocks above, below, to the right and left of this character
//and set the array accordingly so tile collision blocks properly.
//don't invoke this method if you want the object to clip through tiles.
void Character::CheckBlocks(LevelMap* map)
{
	//perform each of these checks, and correct the object's position accordingly.
	//first get the player's tile location in accordance to each corner
	int tile_positionX1 = m_position.x / map->tileSize;
	int tile_positionY1 = m_position.y / map->tileSize;
	int tile_positionX2 = (m_position.x + m_texture->GetWidth()) / map->tileSize;
	int tile_positionY2 = (m_position.y + m_texture->GetHeight()) / map->tileSize;

	//next, test the tiles around that tile position in all four directions.
	//this data can be used later by other methods.
	//priority is at the feet unless it is on the UP which is the head's center instead
	m_blockResult[RIGHT] = map->GetTileAt(tile_positionX2 + 1, tile_positionY2);
	m_blockResult[UP] = map->GetTileAt(tile_positionX1 + (tile_positionX2 - tile_positionX1), tile_positionY1 - 1);
	m_blockResult[LEFT] = map->GetTileAt(tile_positionX1 - 1, tile_positionY2);
	m_blockResult[DOWN] = map->GetTileAt(tile_positionX2, tile_positionY2 + 1);

	for (int i = 0; i <= DOWN; i++)
	{
		cout << m_blockResult[i] << endl;
	}
}