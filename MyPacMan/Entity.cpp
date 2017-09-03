#include "Entity.h"
#include "PacMan.h"
using std::cout;
using std::endl;

Entity::Entity(Level& l) : animation(FLT_MAX, &shape, 1, false)
{
	shape.setOrigin(ENTITY_WIDTH / 2, ENTITY_HEIGHT / 2);
	level = &l;
}

Entity::Entity(Level& level, float delay, int frames, bool pingpong) : Entity(level)
{
	animation = SpriteSheet(delay, &shape, frames, pingpong);
}

Entity::~Entity()
{
}

void Entity::Move()
{
	float nextx = (floor(GetLeft() / BLOCK_WIDTH) + dir.x) * BLOCK_WIDTH;
	float nexty = (floor(GetUp() / BLOCK_HEIGHT) + dir.y) * BLOCK_HEIGHT;

	sf::Vector2f target = sf::Vector2f(nextx, nexty);

	sf::Vector2f delta = target - GetTopLeftPosition();
	//delta = AdjustToMiddle(delta);

	if (delta != sf::Vector2f(0, 0))
		delta /= sqrt(delta.x * delta.x + delta.y * delta.y);

	// speed * dir is logical but does not work for some unknown reason
	shape.move(speed * dir);

	// this way entering a new tile does not take into account the speed.

	// consider enetring new tile with different conditions when moving positively.
	if (dir.x == 1 || dir.y == 1)
	{
		sf::Vector2i newBloxPosition = GetBloxPosition();
		if (newBloxPosition != prevBloxPosition)
		{
			prevBloxPosition = newBloxPosition;
			OnTileEnter(newBloxPosition.x, newBloxPosition.y);	// trigger new tile event
		}
	}
	// consider enetring a new tile with different conditions when moving negatively.
	else if (dir.x == -1 || dir.y == -1)
	{
		sf::Vector2i newBloxPosition = GetBloxPosition(GetLeft() + ENTITY_WIDTH, GetUp() + ENTITY_HEIGHT);
		if (newBloxPosition != prevBloxPosition)
		{
			prevBloxPosition = newBloxPosition;
			OnTileEnter(newBloxPosition.x, newBloxPosition.y);	// trigger new tile event
		}
	}
}

void Entity::Update(sf::Event event)
{
	animation.Update();
	Move();
}

void Entity::Draw(sf::RenderWindow& window)
{
	window.draw(shape);
}

sf::Vector2f Entity::AdjustToMiddle(sf::Vector2f position)
{
	return position + sf::Vector2f(BLOCK_WIDTH / 2, BLOCK_HEIGHT / 2);
}

sf::Vector2i Entity::GetBloxPosition(float x, float y) const
{
	return sf::Vector2i(x / BLOCK_WIDTH, y / BLOCK_HEIGHT);
}

sf::Vector2i Entity::GetBloxPosition(sf::Vector2f pos) const
{
	return sf::Vector2i(pos.x / BLOCK_WIDTH, pos.y / BLOCK_HEIGHT);
}

sf::Vector2i Entity::GetBloxPosition() const
{
	return GetBloxPosition(GetTopLeftPosition());
}

sf::Vector2f Entity::GetRealPosition(sf::Vector2i position) const
{
	return sf::Vector2f(position.x * BLOCK_WIDTH, position.y * BLOCK_HEIGHT);
}

sf::Vector2f Entity::GetRealPosition() const
{
	return GetTopLeftPosition();
}

void Entity::SnapToTile(int x, int y)
{
	shape.setPosition(AdjustToMiddle(sf::Vector2f(x * BLOCK_WIDTH, y * BLOCK_HEIGHT)));
}

void Entity::SnapToTile(sf::Vector2i tile)
{
	SnapToTile(tile.x, tile.y);
}

float Entity::GetLeft() const
{
	return shape.getPosition().x - (ENTITY_WIDTH / 2);
}

float Entity::GetRight() const
{
	return shape.getPosition().x + (ENTITY_WIDTH / 2);
}

float Entity::GetUp() const
{
	return shape.getPosition().y - (ENTITY_HEIGHT / 2);
}

float Entity::GetDown() const
{
	return shape.getPosition().y + (ENTITY_HEIGHT / 2);
}

sf::Vector2f Entity::GetTopLeftPosition() const
{
	return sf::Vector2f(GetLeft(), GetUp());
}

sf::Vector2f Entity::GetBottomRightPosition() const
{
	return sf::Vector2f(GetRight(), GetDown());
}