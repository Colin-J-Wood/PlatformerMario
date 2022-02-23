#include "Collisions.h"

Collisions* Collisions::m_instance = nullptr;

Collisions::Collisions()
{

}

Collisions::~Collisions()
{
	m_instance = nullptr;
}

Collisions* Collisions::Instance()
{
	if (!m_instance)
	{
		m_instance = new Collisions;
	}

	return m_instance;
}

bool Collisions::Box(Rect2D rect1, Rect2D rect2)
{
	//cout << "rect1 x" << rect1.x << endl;
	//cout << "rect1 y" << rect1.y << endl;
	//cout << "rect2 x" << rect2.x << endl;
	//cout << "rect2 y" << rect2.y << endl;
	//cout << "rect1 w" << rect1.width << endl;
	//cout << "rect1 h" << rect1.height << endl;
	//cout << "rect2 w" << rect2.width << endl;
	//cout << "rect2 h" << rect2.height << endl;

	/*Box collision corrected:
	 for any given line, two endpoints will always be outside of the lines crossover.
	 this means if they really are colliding, the far endpoint of the line
	 is guaranteed to be within the versus line.
	 when the lines overlap rect 1's far left will always be further to the left of rect 2's
	 far right.*/
	if (rect1.x < rect2.x + rect2.width && 
		rect2.x < rect1.x + rect1.width &&
		rect1.y < rect2.y + rect2.height &&
		rect2.y < rect1.y + rect1.height)
	{
		return true;
	}
	return false;
}

bool Collisions::Circle(Character* character1, Character* character2)
{
	Vector2D vec = Vector2D((character1->GetPosition().x - character2->GetPosition().x), (character1->GetPosition().y - character2->GetPosition().y));
	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double combined_distance = (character1->GetCollisionRadius() + character2->GetCollisionRadius());

	return distance < combined_distance;
}

bool Collisions::PerPixel(Character* character1, Character* character2)
{
	return false;
}