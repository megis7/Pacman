#pragma once

#include <SFML\Graphics.hpp>
#include <cmath>

class Math
{
public:
	Math();
	~Math();

	static float DistanceSquared(sf::Vector2f p1, sf::Vector2f p2);
	static int ManhattanDistance(sf::Vector2i p1, sf::Vector2i p2);

	static sf::Vector2f GetPerpVector(sf::Vector2f v1);
	static sf::Vector2i GetPerpVector(sf::Vector2i v2);

	static bool IsColinear(sf::Vector2f v1, sf::Vector2f v2);
	static bool IsColinear(sf::Vector2i v1, sf::Vector2i v2);

	static sf::Vector2i Absolute(sf::Vector2i v1);
};

