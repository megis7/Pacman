#include "Math.h"


Math::Math()
{
}


Math::~Math()
{
}

float Math::DistanceSquared(sf::Vector2f p1, sf::Vector2f p2)
{
	return pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
}

int Math::ManhattanDistance(sf::Vector2i p1, sf::Vector2i p2)
{
	return abs(p1.x - p2.x) + abs(p1.y - p2.y);
}

sf::Vector2f Math::GetPerpVector(sf::Vector2f v1)
{
	// cross product for 2 dimensions produces a vector, that are perpendicular to v1
	return sf::Vector2f(v1.y, -v1.x);
}

sf::Vector2i Math::GetPerpVector(sf::Vector2i v1)
{
	return sf::Vector2i(v1.y, -v1.x);
}

bool Math::IsColinear(sf::Vector2f v1, sf::Vector2f v2)
{
	return (v1.x * v2.y == v1.y * v2.x);
}

bool Math::IsColinear(sf::Vector2i v1, sf::Vector2i v2)
{
	return (v1.x * v2.y == v1.y * v2.x);
}

sf::Vector2i Math::Absolute(sf::Vector2i v1)
{
	return sf::Vector2i(abs(v1.x), abs(v1.y));
}