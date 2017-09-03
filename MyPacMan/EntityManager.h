#pragma once

#include "Ghost.h"
#include "Blinky.h"
#include "Inky.h"
#include "Clyde.h"
#include "PacMan.h"
#include <vector>
#include <algorithm>

class EntityManager
{
public:
	EntityManager(Level& level);
	~EntityManager();

	void Update(sf::Event event);
	void Draw(sf::RenderWindow& window);

	void Chase() const;
	void Flee() const;

public:
	Level& level;
	PacMan pacman;
	std::vector<Ghost*> ghosts;

	sf::Clock fleeClock;
	static const int fleeTime = 4;	// seconds
};
