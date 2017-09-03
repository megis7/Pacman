#pragma once

#include <SFML\Graphics.hpp>
#include "BlockDefinitions.h"

class Animation
{
public:
	Animation();
	Animation(sf::IntRect texturePortion, sf::Vector2f pos, bool draw);
	~Animation();

	//void Move();
	void Draw(sf::RenderWindow& window);

	static sf::Texture atlas;

	sf::IntRect texturePortion;
	sf::Sprite sprite;

	bool shouldDraw;
};

