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

	m_hit_sound = new Sound("Sound/pow.mp3");
}

//this version adds sound and block type support.
POWBlock::POWBlock(SDL_Renderer* renderer, LevelMap* map, string image_filename, string sound_filename, int num_hits, BLOCKTYPE blocktype, Vector2D position)
{
	//first create the texture based on the passed file name.
	//file name is passed so multiple blocks can inherit from this class.
	m_texture = new Texture2D(renderer);
	if (!m_texture->LoadFromFile(image_filename))
	{
		cout << "Failed to load texture." << endl;
		return;
	}

	//setup the references and variables.
	m_level_map = map;
	m_num_hits_left = num_hits;
	m_num_hits_max = num_hits;
	m_single_sprite_w = m_texture->GetWidth() / num_hits; //there are num_hits slices of this sprite.
	m_single_sprite_h = m_texture->GetHeight();
	m_position = position;
	m_blocktype = blocktype;

	m_hit_sound = new Sound(sound_filename);
}

POWBlock::~POWBlock()
{
	delete(m_texture);

	m_renderer = nullptr;
	m_texture = nullptr;
	m_level_map = nullptr;
}

void POWBlock::Update(float deltaTime)
{
	//certain types of blocks should update in different fashions, such as respawning.
	switch (m_blocktype)
	{
	case DESTRUCTIBLE:
		if (m_num_hits_left <= 0) m_time_since_gone += deltaTime;
		if (m_time_since_gone > DESTRUCTIBLE_RESPAWN_TIME)
		{
			m_time_since_gone = 0.0f;
			m_num_hits_left = m_num_hits_max;

			m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE), (GetPosition().y / DEFAULT_TILESIZE) + 1, FLOOR);
			m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE) + 1, (GetPosition().y / DEFAULT_TILESIZE) + 1, FLOOR);
			m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE), (GetPosition().y / DEFAULT_TILESIZE), FLOOR);
			m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE) + 1, (GetPosition().y / DEFAULT_TILESIZE), FLOOR);
		}

		break;
	}
}

void POWBlock::Render()
{
	if (m_num_hits_left > 0)
	{
		//get the portion of the sheet to draw from
		int left = m_single_sprite_w * (m_num_hits_left - 1);

		//get a portion of the sprite
		//the maximum number of hits tells how many pixels wide the image is as well, since each slice is one state.
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

void POWBlock::Render(Rect2D camera_rect)
{
	if (m_num_hits_left > 0)
	{
		//get the portion of the sheet to draw from
		int left = m_single_sprite_w * (m_num_hits_left - 1);

		//get a portion of the sprite
		SDL_Rect portion_of_sprite = { left, 0, m_single_sprite_w, m_single_sprite_h };

		//determine where to draw it.
		SDL_Rect dest_rect = {
			static_cast<int>(m_position.x + camera_rect.x), static_cast<int>(m_position.y + camera_rect.y),
			m_single_sprite_w, m_single_sprite_h
		};

		//draw the sprite
		m_texture->Render(portion_of_sprite, dest_rect, SDL_FLIP_NONE);
	}
}

BLOCKTYPE POWBlock::TakeHit()
{
	m_hit_sound->PlaySound(0);
	m_num_hits_left--;
	
	if (m_num_hits_left <= 0)
	{
		m_num_hits_left = 0;
		m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE), (GetPosition().y / DEFAULT_TILESIZE) + 1, AIR);
		m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE) + 1, (GetPosition().y / DEFAULT_TILESIZE) + 1, AIR);
		m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE), (GetPosition().y / DEFAULT_TILESIZE), AIR);
		m_level_map->SetTileAt((GetPosition().x / DEFAULT_TILESIZE) + 1, (GetPosition().y / DEFAULT_TILESIZE), AIR);
	}

	return m_blocktype;
}