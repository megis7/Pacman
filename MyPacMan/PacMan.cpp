#include "PacMan.h"
#include "EntityManager.h"

using std::cout;
using std::endl;

PacMan::PacMan(Level& l, EntityManager& m) : Entity(l, 80, 3, true)
{
	texture.loadFromFile("PacMan.png");

	shape.setTexture(texture);

	speed = ENTITY_SPEED;

	shape.setPosition(AdjustToMiddle(sf::Vector2f(10 * BLOCK_WIDTH, 11 * BLOCK_HEIGHT)));
	prevBloxPosition = sf::Vector2i(shape.getPosition() / (float)BLOCK_WIDTH);
	dir = sf::Vector2f(0, 0);
	nextDir = dir;

	score = 0;

	manager = &m;
}

PacMan::~PacMan()
{
}

void PacMan::Update(sf::Event event)
{
	animation.Update();

	if (event.type == sf::Event::KeyPressed)
	{
		if (dir == sf::Vector2f(0, 0))
		{
			animation.Start();

			ReceiveInput(event, dir);

			// when not moving check if moving with dir will enter a wall block as OnTileEnter cannot account for that.
			sf::Vector2i next = GetBloxPosition() + sf::Vector2i(dir);

			if (level->IsWall(next) == true)
			{
				dir = sf::Vector2f(0, 0);
				animation.Stop();
			}
		}
		else
			ReceiveInput(event, nextDir);
	}

	Move();
}

void PacMan::ReceiveInput(sf::Event event, sf::Vector2f& dir)
{
	if (event.key.code == sf::Keyboard::Left)
	{
		dir = sf::Vector2f(-1, 0);
		nextAngle = 180;
	}
	else if (event.key.code == sf::Keyboard::Right)
	{
		dir = sf::Vector2f(1, 0);
		nextAngle = 0;
	}
	else if (event.key.code == sf::Keyboard::Up)
	{
		dir = sf::Vector2f(0, -1);
		nextAngle = -90;
	}
	else if (event.key.code == sf::Keyboard::Down)
	{
		dir = sf::Vector2f(0, 1);
		nextAngle = 90;
	}
	else if (event.key.code == sf::Keyboard::BackSpace)
		dir = sf::Vector2f(0, 0);

	// if input is in a colinear direction to the current direction change immediately
	// as no snapping is required and so we evade glitching, waiting to enter a new tile.
	if (dir != sf::Vector2f() && Math::IsColinear(this->dir, dir) == true)
	{
		this->dir = dir;
		this->nextDir = dir;
		shape.setRotation(nextAngle);
	}
}

void PacMan::OnTileEnter(int x, int y)
{
	if (level->grid[x][y].type == Tile::BlockType::POWER_FRUIT)
		manager->Flee();

	// if pacman is ontop of a fruit or power fruit make it empty tile and increase his score
	if (level->grid[x][y].type == Tile::BlockType::FRUIT || level->grid[x][y].type == Tile::BlockType::POWER_FRUIT)
	{
		level->grid[x][y].SetBlockType(Tile::BlockType::EMPTY);
		score++;
	}

	// if there is a request to change direction consider if it is valid and snap.
	if (dir != nextDir)
	{
		// if there is a request to change dir and it is valid for some grid[x][y] change dir on that time.
		sf::Vector2i next(x + nextDir.x, y + nextDir.y);

		// this check is not needed as it is guaranteed that a surrounding wall will prevent movement to edges
		//if (!(nextx < 0 || nextx >= BLOCKS_PER_ROW || nexty < 0 || nexty >= BLOCKS_PER_COLUMN))
		{
			if (level->IsWall(next) == false)
			{
				if (Math::IsColinear(dir, nextDir) == false)	// snap only when new direction is not colinear to old direction for gaps and glitches purposes.
					SnapToTile(x, y);

				shape.setRotation(nextAngle);
				dir = nextDir;
				return;
			}
		}
	}

	// if moving with dir sends pacman to invalid territory, stop moving.
	sf::Vector2i next = sf::Vector2i(x + dir.x, y + dir.y);

	if (level->IsWall(next) == true)
	{
		animation.Stop();

		SnapToTile(x, y);
		dir = sf::Vector2f(0, 0);
		nextDir = sf::Vector2f(0, 0);
	}
}

void PacMan::Die()
{
	shape.setPosition(AdjustToMiddle(sf::Vector2f(10 * BLOCK_WIDTH, 11 * BLOCK_HEIGHT)));
	prevBloxPosition = sf::Vector2i(shape.getPosition() / (float)BLOCK_WIDTH);
	dir = sf::Vector2f(0, 0);
	nextDir = dir;
	shape.setRotation(0);
	nextAngle = 0;
}