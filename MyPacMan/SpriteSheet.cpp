#include "SpriteSheet.h"

SpriteSheet::SpriteSheet() : SpriteSheet(0, NULL, 0, false)
{
}

SpriteSheet::SpriteSheet(float delay, sf::Sprite* sprite, int frames, bool pingpong)
{
	this->delay = delay;
	this->sprite = sprite;
	this->frames = frames;
	this->pingpong = pingpong;

	index = 0;

	if (pingpong == true)
		direction = 1;
	else
		direction = 0;

	yCoord = 0;

	texturePortion = sf::IntRect(index * ENTITY_WIDTH, yCoord, ENTITY_WIDTH, ENTITY_HEIGHT);
	this->sprite->setTextureRect(texturePortion);
	update = true;

	clock.restart();
}


SpriteSheet::~SpriteSheet()
{
}

void SpriteSheet::Update()
{
	if (sprite == NULL || update == false)
		return;

	if (clock.getElapsedTime().asMilliseconds() > delay)
	{
		index += direction;

		if (index == frames - 1 || index == 0)
		{
			if (pingpong == false)
				index = 0;
			else
				direction = -direction;
		}

		texturePortion = sf::IntRect(index * ENTITY_WIDTH, yCoord, ENTITY_WIDTH, ENTITY_HEIGHT);
		sprite->setTextureRect(texturePortion);
		clock.restart();
	}
}

void SpriteSheet::Start()
{
	update = true;
}

void SpriteSheet::Stop()
{
	update = false;
}