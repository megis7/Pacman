#pragma once

#include <SFML\Graphics.hpp>
#include "BlockDefinitions.h"

class SpriteSheet
{
public:
	SpriteSheet();
	SpriteSheet(float delay, sf::Sprite* sprite, int frames, bool pingpong);
	~SpriteSheet();

	void Update();

	void Start();
	void Stop();

	inline void SetyCoord(int value){ yCoord = value; }
	inline int GetyCoord(){ return yCoord; }

private:
	sf::IntRect texturePortion;
	float delay;
	sf::Sprite* sprite;
	sf::Clock clock;
	int index;
	int frames;
	bool pingpong;
	int direction;
	bool update;
	int yCoord;
};

