#pragma once

#include <SFML\Graphics.hpp>
#include <string>
#include <fstream>
#include "BlockDefinitions.h"
#include "Tile.h"
#include <iostream>

#define FRUIT_ID 10
#define POWER_FRUIT_ID 11
#define EMPTY_ID 13

class Level
{
public:
	Level();
	~Level();

	void LoadContent(std::string filename);
	void Update(){}
	void UnloadContent(){}

	void Draw(sf::RenderWindow& window);

	bool IsWall(sf::Vector2i pos);
	
	sf::Vector2i ghostStartLocation;
	sf::Vector2i ghostStartSize;
	sf::Vector2i ghostStartEntrance;

	Tile grid[BLOCKS_PER_ROW][BLOCKS_PER_COLUMN];

private:
	Tile::BlockType GetblockType(int c);
};

