#pragma once
#include "Constants.h"
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
class POWBlock
{
public:
	POWBlock();
	POWBlock(SDL_Renderer* renderer, LevelMap* map);
	POWBlock(SDL_Renderer* renderer, LevelMap* map, string filename, int num_hits);
	POWBlock(SDL_Renderer* renderer, LevelMap* map, Vector2D position, string filename, int num_hits);
	~POWBlock();

	void Render();
	Rect2D GetCollisionBox() { return Rect2D(m_position.x, m_position.y, m_single_sprite_w, m_single_sprite_h);  };
	Vector2D GetPosition() { return m_position; };
	float GetHeight() { return m_texture->GetHeight(); };
	void TakeHit();
	bool isAvailable() { return m_num_hits_left > 0; };

private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;
	LevelMap* m_level_map;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_num_hits_left;
};

