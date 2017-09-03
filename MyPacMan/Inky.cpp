#include "Inky.h"

extern sf::Image inky;
#include <Windows.h>

Inky::Inky(Level& level, const PacMan* pacMan, const sf::Vector2f* blinkyPosition) : Ghost(level, pacMan)
{
	texture.loadFromImage(inky);
	shape.setTexture(texture);

	shape.setPosition(AdjustToMiddle(GetRealPosition(level.ghostStartLocation + sf::Vector2i(1, 0))));
	dir = sf::Vector2f(1, 0);

	goAfter = false;

	this->blinkyPosition = blinkyPosition;
}

Inky::~Inky()
{
}

void Inky::Chase()
{
	sf::Vector2i from = GetBloxPosition();
	sf::Vector2i to = pacMan->GetBloxPosition();

	// If pacman is not moving, go straight to him

	if (pacMan->dir == sf::Vector2f(0, 0) || Math::ManhattanDistance(from, to) > 10 ||
		(Math::ManhattanDistance(from, to) <= 2 && Math::ManhattanDistance(from, GetBloxPosition(*blinkyPosition)) > 3))
	{
		dir = GetDirection(PathFindTo(from, to).target);
		std::cout << "Inky going straight to pacman\n";
		return;
	}

	// If near pacman and away from blinky and 5 seconds have passed since chasing him from behind, chase him again from behind

	if (!goAfter && Math::ManhattanDistance(from, to) <= 4 && Math::ManhattanDistance(from, GetBloxPosition(*blinkyPosition)) > 3 && clock.getElapsedTime().asSeconds() > 5)
	{
		std::cout << "Start inky after.\n";
		goAfter = true;
		clock.restart();
	}

	// Chase from behind

	if (goAfter)
	{
		std::cout << "Inky after.\n";
		dir = GetDirection(PathFindTo(from, to).target);

		// If inky has been chasing for more than 1 second, set chasing from in front to avoid lagging behind pacman

		if (clock.getElapsedTime().asSeconds() > 1)
		{
			goAfter = false;
			clock.restart();
		}

		return;
	}

	// Go in front of pacman

	std::cout << "Inky infront\n";

	// Find the tile in front of Pacman

	sf::Vector2i dir = sf::Vector2i(pacMan->dir);

	if (level->IsWall(to + dir) == false)
		to += dir;
	else
	{
		dir = sf::Vector2i(pacMan->nextDir);

		if (level->IsWall(to + dir) == false)
			to += dir;
	}

	// From all possible tiles that inky can go to, go to the one with the shortest path to the front of pacman.
	// We dont BFS directly from the tile inky is at because due to the restriction function the algorithm may crush (path is not found).

	std::vector<sf::Vector2i> possibles;
	possibles.reserve(3);
	GetPossibleTiles(false, possibles);
	std::function<bool(sf::Vector2i)> restr = std::bind(&Inky::PathRestrictions, this, std::placeholders::_1);

	sf::Vector2i bestPossible;
	int min = INT_MAX;

	BFSInfo best;

	for (int i = 0; i < possibles.size(); i++)
	{
		from = possibles[i];

		BFSInfo temp = PathFindTo(from, to, -this->dir, false, false, restr);
		if (temp.pathSize <= min)
		{
			best = temp;
			bestPossible = possibles[i];
			min = temp.pathSize;
		}
	}

	this->dir = GetDirection(bestPossible);
}

bool Inky::PathRestrictions(sf::Vector2i pos)
{
	if (pacMan->GetBloxPosition() == pos)
		return false;
	return true;
}