#include <iostream>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "Commons.h"
#include "Character.h"
#include "LevelMap.h"

using namespace std;

class Collectible
{
private:
	COLLECTIBLETYPE collectibletype;
	Texture2D* m_texture;
	Vector2D m_position;
	LevelMap* m_map;

public:
	Collectible(SDL_Renderer* renderer, string imagePath, LevelMap* map, Vector2D start_position);
	~Collectible();

	virtual COLLECTIBLETYPE OnCollect() { return collectibletype; };

	virtual void Render(Rect2D camera_rect);
	virtual void Update(float deltaTime, SDL_Event e, LevelMap* map);

	Vector2D GetCenterPosition() { return Vector2D(m_position.x + (m_texture->GetWidth() / 2), m_position.y + (m_texture->GetHeight() / 2)); };
	float GetCollisionRadius() { return COLLECTIBLE_RADIUS; };
};
