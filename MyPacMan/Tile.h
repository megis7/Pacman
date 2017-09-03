#pragma once

#include <SFML\Graphics.hpp>
#include "BlockDefinitions.h"
#include "Animation.h"

class Tile
{
public:
	static enum BlockType{ EMPTY, FRUIT, POWER_FRUIT, WALL };	// static enum BlockType { ... WALL, WALL_DL, WALL_HOR, WALL_LD, WALL_VER, WALL_RU, WALL_UL };

	Tile();
	Tile(sf::Vector2f position, BlockType type, int tileIDx, int tileIDy, bool draw);
	~Tile();

	void DisableDraw();
	void Draw(sf::RenderWindow& window);

	void SetBlockType(BlockType newBlockType);

	BlockType type;

	Animation animation;
};

