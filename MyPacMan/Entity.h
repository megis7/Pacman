#pragma once

#include <SFML\Graphics.hpp>
#include "Level.h"

#include <iostream>

#include "SpriteSheet.h"

class Entity
{
public:
	// ctors and dtors
	//--------------------------------------------------------
	Entity(Level& level);
	Entity(Level& level, float delay, int frames, bool pingpong);
	~Entity();
	//--------------------------------------------------------

	// class public interface
	//--------------------------------------------------------
	void LoadContent();
	void UnloadContent(){}
	virtual void Update(sf::Event event);
	virtual void Draw(sf::RenderWindow& window);
	//--------------------------------------------------------

public:

	//necessary public memebrs
	//--------------------------------------------------------
	sf::Sprite shape;
	sf::Vector2f dir;

	sf::Vector2i GetBloxPosition() const;
	sf::Vector2i GetBloxPosition(float x, float y) const;
	sf::Vector2i GetBloxPosition(sf::Vector2f position) const;

	sf::Vector2f GetRealPosition(sf::Vector2i position) const;
	sf::Vector2f GetRealPosition() const;

	float GetLeft() const;
	float GetRight() const;
	float GetUp() const;
	float GetDown() const;

	sf::Vector2f GetTopLeftPosition() const;
	sf::Vector2f GetBottomRightPosition() const;

	virtual void Die() = 0;
	//--------------------------------------------------------

public:

	// class protected interface
	//--------------------------------------------------------
	virtual void OnTileEnter(int x, int y) = 0;
	void Move();
	//--------------------------------------------------------

	// protected members
	//--------------------------------------------------------
	sf::Texture texture;
	Level* level;
	sf::Vector2i prevBloxPosition;
	float speed;
	SpriteSheet animation;

	static const int ENTITY_SPEED = 2;

	void SnapToTile(int x, int y);								// snaps shape to the given tile coordinates
	void SnapToTile(sf::Vector2i tile);

	sf::Vector2f AdjustToMiddle(sf::Vector2f position);
	//--------------------------------------------------------
};
