#pragma once

#include "Ghost.h"
#include "PacMan.h"
#include "Level.h"

#include <functional>

typedef std::function<bool(sf::Vector2i)> FBV2i;

class Behaviour
{
public:
	Behaviour(Ghost* ghost, PacMan* pacman, Level* level);
	~Behaviour();

protected:
	Ghost* ghost;
	PacMan* pacman;
	Level* level;

	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to);
	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDirection, bool calculateComplexity);
	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDirection, bool calculateComplexity, bool inverted, FBV2i restriction);

	float CalculatePathComplexity(const std::vector<sf::Vector2i>& path);
	int GetJunctions(sf::Vector2i pos, sf::Vector2i dir, sf::Vector2i prevDir);

	sf::Vector2f GetDirection(sf::Vector2i target);
	sf::Vector2f GetDirection(sf::Vector2i position, sf::Vector2i target);
};

