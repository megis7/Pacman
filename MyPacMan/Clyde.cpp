#include "Clyde.h"

extern sf::Image clyde;

Clyde::Clyde(Level& level, const PacMan* pacMan) : Ghost(level, pacMan)
{
	texture.loadFromImage(clyde);
	shape.setTexture(texture);

	shape.setPosition(AdjustToMiddle(GetRealPosition(level.ghostStartLocation + sf::Vector2i(2, 0))));
	dir = sf::Vector2f(-1, 0);
	attacking = false;
}


Clyde::~Clyde()
{
}

void Clyde::Chase()
{
	sf::Vector2i myPos = GetBloxPosition();
	sf::Vector2i pacPos = pacMan->GetBloxPosition();

	if (Math::ManhattanDistance(myPos, pacPos) <= 3 && !attacking)	// if clyde is close to pacman then BFS to him to show attacking, not random 
																	// behaviour
	{
		dir = GetDirection(PathFindTo(myPos, pacPos).target);
		std::cout << "Clyde attacks pacMan" << std::endl;
		attacking = true;
	}
	else
	{
		std::vector<sf::Vector2i> possibles;
		possibles.reserve(3);

		GetPossibleTiles(false, possibles);

		dir = GetDirection(possibles[rand() % possibles.size()]);
		attacking = false;
	}
}
