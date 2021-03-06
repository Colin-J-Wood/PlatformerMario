#pragma once
#include "Constants.h"
#include "Commons.h"
#include "LevelMap.h"
#include "Texture2D.h"
#include "Sound.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
class POWBlock
{
public:
	POWBlock();
	POWBlock(SDL_Renderer* renderer, LevelMap* map);
	POWBlock(SDL_Renderer* renderer, LevelMap* map, string filename, int num_hits);
	POWBlock(SDL_Renderer* renderer, LevelMap* map, string image_filename, string sound_filename, int num_hits, BLOCKTYPE blocktype, Vector2D position);
	~POWBlock();

	void Update(float deltaTime);
	void Render();
	void Render(Rect2D camera_rect);
	Rect2D GetCollisionBox() { return Rect2D(m_position.x, m_position.y, m_single_sprite_w, m_single_sprite_h);  };
	Vector2D GetPosition() { return m_position; };
	Vector2D GetCenterPosition() { return Vector2D((m_position.x + (GetWidth() / 2)), (m_position.y + (GetHeight() / 2))); };
	Vector2D GetTilePosition() { return Vector2D(GetCenterPosition().x / DEFAULT_TILESIZE, GetCenterPosition().y / DEFAULT_TILESIZE); };
	float GetHeight() { return m_texture->GetHeight(); };
	float GetWidth() { return m_texture->GetWidth(); };
	virtual BLOCKTYPE TakeHit(); //override this for different take hit functionality.
	bool isAvailable() { return m_num_hits_left > 0; };

private:
	SDL_Renderer* m_renderer;
	Vector2D m_position;
	Texture2D* m_texture;
	LevelMap* m_level_map;
	Sound* m_hit_sound;

	BLOCKTYPE m_blocktype = POW;

	float m_single_sprite_w;
	float m_single_sprite_h;
	int m_num_hits_left;
	int m_num_hits_max;
	float m_time_since_gone;
};

