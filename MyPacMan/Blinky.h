#pragma once

#include "Ghost.h"
#include "Level.h"

// Pacman's raw follower.
class Blinky : public Ghost
{
public:
	Blinky(Level& level, const PacMan* pacMan);
	~Blinky();

	virtual void Chase();
};

