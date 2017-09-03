#pragma once

#include <sfml\Graphics.hpp>
#include "Level.h"
#include "Entity.h"
#include "PacMan.h"
#include "Math.h"

#include <queue>
#include <functional>

#define MAX_DEATH_DIST 225

struct BFSInfo;

class Ghost : public Entity
{
public:
	static enum State{ START, CHASE, FLEE, PROTECT, DEAD };

	Ghost(Level& level, const PacMan* pacman);
	Ghost(Level& level, const PacMan* pacman, float delay, int frames, bool pingpong);
	~Ghost();

	virtual void Update(sf::Event event);

public:
	// Events
	virtual void OnTileEnter(int x, int y);
	void OnFlee();
	void OnChase();

	// State Functions
	void BacknForth();
	virtual void Chase();
	virtual void Flee();
	virtual void ProtectPowerFruits();
	void ReturnHome();

public:
	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to);
	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDirection, bool calculateComplexity);
	BFSInfo PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDirection, bool calculateComplexity, bool inverted, std::function<bool(sf::Vector2i)> restriction);

	const PacMan* pacMan;
	sf::Vector2f GetDirection(sf::Vector2i target);
	sf::Vector2f GetDirection(sf::Vector2i position, sf::Vector2i target);
	void ClearPath();

	bool PathRestrictions(sf::Vector2i pos){ return true; }

	State state;

protected:
	int GetJunctions(sf::Vector2i pos, sf::Vector2i dir, sf::Vector2i prevDir);

	sf::Clock clk;

protected:
	float CalculatePathComplexity(const std::vector<sf::Vector2i>& path);
	sf::Vector2i GetFleeTargetPath(const std::vector<sf::Vector2i>& possibles, bool allowBackwardMovement);
	void GetPossibleTiles(bool allowBackwarMovement, std::vector<sf::Vector2i>& possibles);

private:
	virtual void Die();
	void Resurrect();

	void SetAnimationYCoord();
};

struct BFSInfo
{
	BFSInfo(int pathSize, sf::Vector2i target) : BFSInfo(pathSize, target, 0)
	{
	}

	BFSInfo(int pathSize, sf::Vector2i target, float pathComplexity)
	{
		this->pathSize = pathSize;
		this->pathComplexity = pathComplexity;
		this->target = target;
	}

	BFSInfo() : BFSInfo(0, sf::Vector2i(0, 0), 0)
	{
	}

	int pathSize;
	float pathComplexity;
	sf::Vector2i target;
};