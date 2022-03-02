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
	/*Box collision corrected:
	 for any given line, two endpoints will always be outside of the lines crossover.
	 this means if they really are colliding, the far endpoint of the line
	 is guaranteed to be within the versus line.
	 when the lines overlap rect 1's far left will always be further to the left of rect 2's far right,
	 and rect 1's far right will always be more than rect 2's far left.*/
	if (rect1.x < rect2.x + rect2.width && 
		rect1.x + rect1.width > rect2.x &&
		rect1.y < rect2.y + rect2.height &&
		rect1.y + rect1.height > rect2.y)
	{
		return true;
	}
	return false;
}

bool Collisions::Circle(Vector2D position1, Vector2D position2, float radius1, float radius2)
{
	Vector2D vec = Vector2D((position1.x - position2.x), (position1.y - position2.y));
	double distance = sqrt((vec.x * vec.x) + (vec.y * vec.y));
	double combined_distance = double (radius1) + double (radius2);

	return distance < combined_distance;
}