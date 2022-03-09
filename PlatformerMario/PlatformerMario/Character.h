#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Commons.h"
#include "Texture2D.h"
#include "Constants.h"
#include "LevelMap.h"

using namespace std;

class Character
{
protected:
	//methods
	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);
	virtual void Jump(float deltaTime);

	//variables
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Vector2D m_velocity;
	float m_collision_radius;
	Texture2D* m_texture;
	FACING m_facing_direction; //moved to here for inheritance.

	//states
	bool m_moving_left = false;
	bool m_moving_right = false;
	bool m_jumping;
	bool m_canJump;
	bool m_ctrlJump;
	TILETYPE m_blockResult[4];

public:
	Character();
	Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Character();

	//methods
	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e, LevelMap* map);
	//make gravity virtual just in case the object wants accelerating gravity instead
	virtual void AddGravity(float strength, float deltaTime);
	void CheckBlocks(LevelMap* map);

	//inlines
	void SetPosition(Vector2D new_position) { m_position = new_position; };
	Vector2D GetPosition() { return m_position; };
	float GetCollisionRadius() { return m_collision_radius; };
	Rect2D GetCollisionBox() { return Rect2D(m_position.x, m_position.y, m_texture->GetWidth(), m_texture->GetHeight()); }
	Texture2D* GetTexture() { return m_texture; };

	//variables
	Vector2D m_target_velocity = PLAYER_SPEED;
};

