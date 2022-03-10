#pragma once
#include <SDL.h>

struct Vector2D
{
	float x;
	float y;

	Vector2D()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2D(float initial_x, float initial_y)
	{
		x = initial_x;
		y = initial_y;
	}
};

enum SCREENS
{
	SCREEN_INTRO,
	SCREEN_MENU,
	SCREEN_LEVEL1,
	SCREEN_LEVEL2,
	SCREEN_GAMEOVER,
	SCREEN_HIGHSCORES
};

enum FACING
{
	FACING_LEFT,
	FACING_RIGHT
};

//list of tile types for levelmap to read as integers.  expand or modify the values as necessary.
//use this as a reference table when creating the map.
enum TILETYPE
{
	AIR = 0,
	FLOOR = 1,
	PLATFORM = 2, //some checks, such as up, right and left, won't see a platform, as the player should be able to jump through it.
	OBJECT = 3,
	LAVA = 4
};

//list of blocking directions for characters that test for block.
enum BLOCKDIRECTION
{
	RIGHT = 0,
	UP = 1,
	LEFT = 2,
	DOWN = 3
};

struct Rect2D
{
	float x;
	float y;
	float width;
	float height;

	Rect2D(float x_pos, float y_pos, float width, float height)
	{
		x = x_pos;
		y = y_pos;
		this->width = width;
		this->height = height;
	}
};