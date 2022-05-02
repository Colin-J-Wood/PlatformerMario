
#include "Collectible.h"

Collectible::Collectible(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position)
{
	m_position = start_position;
	collectibletype = COIN;
	m_texture = new Texture2D(renderer);
	m_map = map;
	if (!m_texture->LoadFromFile(imagePath))
	{
		cout << "Failed to load collectible texture!" << endl;
	}
}

Collectible::~Collectible()
{
	delete(m_texture);
}

void Collectible::Render(Rect2D camera_rect)
{
	m_texture->Render(Vector2D(m_position.x + camera_rect.x, m_position.y + camera_rect.y), SDL_FLIP_NONE);
}

void Collectible::Update(float deltaTime, SDL_Event e, LevelMap* map)
{

}