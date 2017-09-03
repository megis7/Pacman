#include "Tile.h"


Tile::Tile()
{
}

Tile::Tile(sf::Vector2f pos, BlockType t, int tileIDx, int tileIDy, bool draw)
{
	type = t;

	animation = Animation(sf::IntRect(tileIDy * BLOCK_WIDTH, tileIDx * BLOCK_HEIGHT, BLOCK_WIDTH, BLOCK_HEIGHT), pos, draw);
}

Tile::~Tile()
{
}

void Tile::Draw(sf::RenderWindow& window)
{
	animation.Draw(window);
}

void Tile::DisableDraw()
{
	animation.shouldDraw = false;
}

void Tile::SetBlockType(BlockType newBlockType)
{
	if (newBlockType != Tile::EMPTY)
		animation.shouldDraw = true;
	else
		animation.shouldDraw = false;

	type = newBlockType;
}