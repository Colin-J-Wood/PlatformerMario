#include "POWBlock.h"

using namespace std;

POWBlock::POWBlock()
{

}

//this constructor summons a default pow block and is used for testing/level 1.
POWBlock::POWBlock(SDL_Renderer* renderer, LevelMap* map)
{
	//first create the texture based on the passed file name.
	//file name is passed so multiple blocks can inherit from this class.
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile("Images/tile/PowBlock.png"))
	{
		cout << "Failed to load texture." << endl;
		return;
	}

	//setup the references and variables.
	m_level_map = map;
	m_single_sprite_w = m_texture->GetWidth() / 3; //there are three slices of this sprite.
	m_single_sprite_h = m_texture->GetHeight();
	m_num_hits_left = 3;
	m_position = Vector2D((SCREEN_WIDTH * 0.5f) - m_single_sprite_w * 0.5f, 260);

	//tell the tile map this block exists here now
	m_level_map->SetTileAt(m_position.x / DEFAULT_TILESIZE, m_position.y / DEFAULT_TILESIZE, OBJECT);

	m_hit_sound = new Sound("Sound/pow.mp3");
}

//this version initializes a pow block with a set number of hits, which requires the file to have that many slices.
POWBlock::POWBlock(SDL_Renderer* renderer, LevelMap* map, string filename, int num_hits)
{
	//first create the texture based on the passed file name.
	//file name is passed so multiple blocks can inherit from this class.
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(filename))
	{
		cout << "Failed to load texture." << endl;
		return;
	}

	//setup the references and variables.
	m_level_map = map;
	m_single_sprite_w = m_texture->GetWidth() / 3; //there are three slices of this sprite.
	m_single_sprite_h = m_texture->GetHeight();
	m_num_hits_left = num_hits;
	m_position = Vector2D((SCREEN_WIDTH * 0.5f) - m_single_sprite_w * 0.5f, 260);

	//tell the tile map this block exists here now
	m_level_map->SetTileAt(m_position.x / DEFAULT_TILESIZE, m_position.y / DEFAULT_TILESIZE, OBJECT);

	m_hit_sound = new Sound("Sound/pow.mp3");
}

//this constructor allows setting a position manually.
POWBlock::POWBlock(SDL_Renderer* renderer, LevelMap* map, Vector2D position, string filename, int num_hits)
{
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(filename))
	{
		cout << "Failed to load texture." << endl;
		return;
	}

	m_level_map = map;
	m_single_sprite_w = m_texture->GetWidth() / 3; //there are three slices of this sprite.
	m_single_sprite_h = m_texture->GetHeight();
	m_num_hits_left = num_hits;
	m_position = position;

	m_hit_sound = new Sound("Sound/pow.mp3");
}

POWBlock::~POWBlock()
{
	delete(m_texture);

	m_renderer = nullptr;
	m_texture = nullptr;
	m_level_map = nullptr;
}

void POWBlock::Render()
{
	if (m_num_hits_left > 0)
	{
		//get the portion of the sheet to draw from
		int left = m_single_sprite_w * (m_num_hits_left - 1);

		//get a portion of the sprite
		SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };

		//determine where to draw it.
		SDL_Rect dest_rect = {
			static_cast<int>(m_position.x), static_cast<int>(m_position.y),
			m_single_sprite_w, m_single_sprite_h
		};

		//draw the sprite
		m_texture->Render(portion_of_sprite, dest_rect, SDL_FLIP_NONE);
	}
}

void POWBlock::TakeHit()
{
	m_hit_sound->PlaySound(0);
	m_num_hits_left--;
	
	if (m_num_hits_left <= 0)
	{
		m_num_hits_left = 0;
		m_level_map->SetTileAt(m_position.x / DEFAULT_TILESIZE, m_position.y / DEFAULT_TILESIZE, AIR);
	}
}