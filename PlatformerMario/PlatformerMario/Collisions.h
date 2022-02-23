#pragma once
#include "Commons.h"
#include "Constants.h"
#include "Character.h"
class Collisions
{
private:
	Collisions();

	static Collisions* m_instance;
public:
	~Collisions();

	static Collisions* Instance();

	bool Circle(Vector2D position1, Vector2D position2, float radius1, float radius2);
	bool Box(Rect2D rect1, Rect2D rect2);
	bool PerPixel(Character* character1, Character* character2);
};

