#include "Animation.h"

sf::Texture Animation::atlas = sf::Texture();

Animation::Animation() : Animation(sf::IntRect(0, 0, 0, 0), sf::Vector2f(0, 0), false)
{
}

Animation::Animation(sf::IntRect texturePortion, sf::Vector2f pos, bool draw)
{
	shouldDraw = draw;

	if (draw == false)
		return;

	this->texturePortion = texturePortion;
	sprite = sf::Sprite(atlas);
	sprite.setTextureRect(this->texturePortion);

	sprite.setPosition(pos);
}

Animation::~Animation()
{
}

void Animation::Draw(sf::RenderWindow& window)
{
	if (shouldDraw)
		window.draw(sprite);
}