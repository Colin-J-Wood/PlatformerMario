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

	bool Circle(Character* character1, Character* character2);
	bool Box(Rect2D rect1, Rect2D rect2);
	bool PerPixel(Character* character1, Character* character2);
};

