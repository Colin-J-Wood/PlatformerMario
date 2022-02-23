#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include "Commons.h"
#include "Texture2D.h"

using namespace std;

class Character
{
protected:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Vector2D m_velocity;
	Texture2D* m_texture;
	FACING m_facing_direction; //moved to here for inheritance.

	bool m_moving_left = false;
	bool m_moving_right = false;

	virtual void MoveLeft(float deltaTime);
	virtual void MoveRight(float deltaTime);

public:
	Character();
	Character(SDL_Renderer* renderer, string imagePath, Vector2D start_position);
	~Character();

	virtual void Render();
	virtual void Update(float deltaTime, SDL_Event e);
	void SetPosition(Vector2D new_position);
	Vector2D GetPosition();

	Vector2D m_target_velocity = Vector2D(100.0f, 0.0f);
};

