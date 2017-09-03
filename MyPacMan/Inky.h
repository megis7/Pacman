#pragma once


#include "Ghost.h"
#include <functional>


class Inky : public Ghost
{
public:
	Inky(Level& level, const PacMan* pacMan, const sf::Vector2f* blinkyPosition);
	~Inky();

	virtual void Chase();

	bool PathRestrictions(sf::Vector2i pos);

	bool goAfter;
	sf::Clock clock;

	const sf::Vector2f* blinkyPosition;
};

