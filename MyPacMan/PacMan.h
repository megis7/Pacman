#pragma once

#include "Entity.h"
#include "Math.h"
#include <iostream>

class EntityManager;

class PacMan : public Entity
{
public:
	// ctors and dtors
	//--------------------------------------------------------
	PacMan();
	PacMan(Level& level, EntityManager& manager);
	~PacMan();
	//--------------------------------------------------------

	// Base interface override
	//--------------------------------------------------------
	virtual void Update(sf::Event event);
	virtual void OnTileEnter(int x, int y);

	virtual void Die();
	//--------------------------------------------------------
	
	// Necessary public members
	//--------------------------------------------------------
	sf::Vector2f nextDir;
	//--------------------------------------------------------

private:
	// private members
	//--------------------------------------------------------
	int score;
	void ReceiveInput(sf::Event event, sf::Vector2f& dir);
	const EntityManager* manager;
	float nextAngle;
	//--------------------------------------------------------
};

