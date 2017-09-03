#pragma once

#include "Ghost.h"

class Clyde : public Ghost
{
public:
	Clyde(Level& level, const PacMan* pacMan);
	~Clyde();

	virtual void Chase();

private:
	bool attacking;
};

