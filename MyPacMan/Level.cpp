#include "Level.h"

Level::Level()
{
}


Level::~Level()
{
}

void Level::LoadContent(std::string filename)
{
	std::ifstream filein(filename);

	if (filein.is_open() == false)
		exit(1);

	int x, y;
	x = y = 0;

	while (x < 20)
	{
		std::string line;
		std::getline(filein, line);

		while (y < line.length())
		{
			int id = line[y] - 'A';
			Tile::BlockType type = GetblockType(id);

			int idx = id / 7;
			int idy = id % 7;

			grid[x][y] = Tile(sf::Vector2f(x * BLOCK_WIDTH, y * BLOCK_HEIGHT), type, idx, idy, type != Tile::EMPTY);
			y++;
		}

		y = 0;
		x++;
	}

	filein >> ghostStartLocation.x >> ghostStartLocation.y >> ghostStartSize.x >> ghostStartSize.y >> ghostStartEntrance.x >> ghostStartEntrance.y;

	filein.close();
	
}

void Level::Draw(sf::RenderWindow& window)
{
	for (int i = 0; i < BLOCKS_PER_ROW; i++)
	{
		for (int j = 0; j < BLOCKS_PER_COLUMN; j++)
		{
			grid[i][j].Draw(window);
		}
	}
}

Tile::BlockType Level::GetblockType(int c)
{
	switch (c)
	{
	case EMPTY_ID:
		return Tile::EMPTY;
	case POWER_FRUIT_ID:
		return Tile::POWER_FRUIT;
	case FRUIT_ID:
		return Tile::FRUIT;
	default:
		return Tile::WALL;
	}
}

bool Level::IsWall(sf::Vector2i pos)
{
	return grid[pos.x][pos.y].type == Tile::WALL;
}