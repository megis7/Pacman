#include "Blinky.h"

extern sf::Image image;

Blinky::Blinky(Level& level, const PacMan* pacMan) : Ghost(level, pacMan)
{
	texture.loadFromImage(image);
	shape.setTexture(texture);

	shape.setPosition(AdjustToMiddle(GetRealPosition(level.ghostStartLocation)));
	dir = sf::Vector2f(1, 0);
}


Blinky::~Blinky()
{
}

void Blinky::Chase()
{
	// chase pacman by moving to the closest tile to him.
	sf::Vector2i target = GetBloxPosition(pacMan->shape.getPosition());
	sf::Vector2i temp = PathFindTo(GetBloxPosition(shape.getPosition()), target).target;
	dir = GetDirection(temp);
}