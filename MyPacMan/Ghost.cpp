#include "Ghost.h"
#include <Windows.h>

Ghost::Ghost(Level& l, const PacMan* pacman) : Ghost(l, pacman, 80, 2, true)
{
}

Ghost::Ghost(Level& l, const PacMan* pacman, float delay, int frames, bool pingpong)
	: Entity(l, delay, frames, pingpong)
{
	speed = ENTITY_SPEED;

	prevBloxPosition = l.ghostStartLocation;

	pacMan = pacman;
	state = START;
}

Ghost::~Ghost()
{
}

void Ghost::Update(sf::Event event)
{
	animation.Update();

	sf::Vector2f prevDir = dir;

	Move();

	if (prevDir != dir)
		SetAnimationYCoord();

	// Someone dies here !!! PacMan or ghost

	if (Math::DistanceSquared(pacMan->GetRealPosition(), GetRealPosition()) < MAX_DEATH_DIST)		// MAX_DEATH_DIST = 225 = (ENTITY_WIDTH OR HEIGHT / 2) ^ 2
	{
		if (state == FLEE)
			Die();
		else if (state == CHASE)
		{
			dir = sf::Vector2f(0, 0);
			SnapToTile(pacMan->GetBloxPosition());
			const_cast<PacMan*>(pacMan)->Die();			// remove constantness for just one instruction
		}
	}
}

void Ghost::OnTileEnter(int x, int y)
{
	if (state == START)
	{
		BacknForth();
		return;
	}

	sf::Vector2i perp = sf::Vector2i(Math::GetPerpVector(dir));

	// if there is a junction we consider changing direction
	if (!level->IsWall(sf::Vector2i(x + perp.x, y + perp.y)) || !level->IsWall(sf::Vector2i(x - perp.x, y - perp.y)))
	{
		sf::Vector2f prevDir = dir;

		switch (state)
		{
		case CHASE:
			Chase();
			break;
		case FLEE:
			Flee();
			break;
		case PROTECT:
			ProtectPowerFruits();
			break;
		case DEAD:
			ReturnHome();
			break;
		}

		// if the ghost does not change its path then no need to snap
		if (Math::IsColinear(dir, prevDir) == false)
			SnapToTile(x, y);
	}
}

void Ghost::ClearPath()
{
	dir = sf::Vector2f(0, 0);
}

BFSInfo Ghost::PathFindTo(sf::Vector2i from, sf::Vector2i to)
{
	return PathFindTo(from, to, -dir, false);
}

BFSInfo Ghost::PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDir, bool calculateComplexity)
{
	std::function<bool(sf::Vector2i)> restr = std::bind(&Ghost::PathRestrictions, this, std::placeholders::_1);

	return PathFindTo(from, to, bannedDir, calculateComplexity, false, restr);
}

BFSInfo Ghost::PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDir, bool calculateComplexity, bool inverted, std::function<bool(sf::Vector2i)> restr)
{
	// if position and target are the same BFS stucks so return and of course there is no path to determine.
	sf::Vector2i current = from;

	if (current == to)
	{
		//ClearPath();			// D A N G E R : path is not cleared and ghost may run out of the map. Used for ghost death
		std::cout << "Zero path\n";
		return BFSInfo(0, to);
	}

	bool visited[BLOCKS_PER_ROW][BLOCKS_PER_COLUMN];
	sf::Vector2i parents[BLOCKS_PER_ROW][BLOCKS_PER_COLUMN];

	for (int i = 0; i < BLOCKS_PER_ROW; i++)
		for (int j = 0; j < BLOCKS_PER_COLUMN; j++)
			visited[i][j] = level->IsWall(sf::Vector2i(i, j));	// mark every wall tile as visited.

	// BFS for shortest path from position to (x, y).

	std::queue<sf::Vector2i> queue;

	queue.push(current);
	int counter = 0;

	while (queue.empty() == false)
	{
		sf::Vector2i pos = queue.front();
		queue.pop();

		if (pos == to)
			break;

		visited[pos.x][pos.y] = true;

		// restrict BFS according to direction. This applies only to the first pass of the BFS where
		// adjacent to the current tile tiles are searched.

		// Check for wall tiles is not made as they are marked as visited.

		if (!(counter == 0 && bannedDir.x == 1) && visited[pos.x + 1][pos.y] == false) // right
		{
			if (restr(sf::Vector2i(pos.x + 1, pos.y)))
			{
				queue.push(sf::Vector2i(pos.x + 1, pos.y));
				parents[pos.x + 1][pos.y] = pos;
			}
		}

		if (!(counter == 0 && bannedDir.x == -1) && visited[pos.x - 1][pos.y] == false) //left
		{
			if (restr(sf::Vector2i(pos.x - 1, pos.y)))
			{
				queue.push(sf::Vector2i(pos.x - 1, pos.y));
				parents[pos.x - 1][pos.y] = pos;
			}
		}

		if (!(counter == 0 && bannedDir.y == 1) && visited[pos.x][pos.y + 1] == false)	// down
		{
			if (restr(sf::Vector2i(pos.x, pos.y + 1)))
			{
				queue.push(sf::Vector2i(pos.x, pos.y + 1));
				parents[pos.x][pos.y + 1] = pos;
			}
		}

		if (!(counter == 0 && bannedDir.y == -1) && visited[pos.x][pos.y - 1] == false)	// up
		{
			if (restr(sf::Vector2i(pos.x, pos.y - 1)))
			{
				queue.push(sf::Vector2i(pos.x, pos.y - 1));
				parents[pos.x][pos.y - 1] = pos;
			}
		}

		counter++;
	}

	// Retrieve the shortest path to the target. This path is in reverse order.

	std::vector<sf::Vector2i> path;
	sf::Vector2i pos = to;
	path.push_back(pos);

	pos = parents[pos.x][pos.y];

	if (pos == sf::Vector2i(0, 0))			// if path is invalid (nonexistent) do infinite length and return target to continue execution of the program.
		return BFSInfo(INT_MAX, to, 0);

	while (pos != current)
	{
		path.push_back(pos);
		pos = parents[pos.x][pos.y];
	}

	sf::Vector2i target;

	if (inverted)
		target = path[0];
	else
		target = path[path.size() - 1];

	float complexity = -1;

	if (calculateComplexity)
		complexity = CalculatePathComplexity(path);

	return BFSInfo(path.size(), target, complexity);
}

sf::Vector2f Ghost::GetDirection(sf::Vector2i target)
{
	return GetDirection(GetBloxPosition(), target);
}

sf::Vector2f Ghost::GetDirection(sf::Vector2i position, sf::Vector2i target)
{
	sf::Vector2i next = target;
	sf::Vector2i pos = position;
	sf::Vector2f dir;

	if (pos.x < next.x)
		dir = sf::Vector2f(1, 0);
	else if (pos.x > next.x)
		dir = sf::Vector2f(-1, 0);
	else if (pos.y < next.y)
		dir = sf::Vector2f(0, 1);
	else if (pos.y > next.y)
		dir = sf::Vector2f(0, -1);
	else										// Handler for delicate situations where ghost is just over pacman but he is not close enough to trigger death.
	{											// and may run through the walls.
		if (pacMan->dir == sf::Vector2f(0, 0))	// if pacman is not moving keep the moving direction and get him. Death trigger will stop the ghost.
			dir = this->dir;
		else if (level->IsWall(position + sf::Vector2i(pacMan->dir)) == false)
			dir = pacMan->dir;					// if pacman is moving adopt his direction to follow him. Of course only if it is valid
		else
		{
			std::vector<sf::Vector2i> possibles;		//done when inky at 540 496 with dir downwards and pacman at 554 500 with dir left
			GetPossibleTiles(false, possibles);			// fixes terrbile bug where ghost goes on walls. See danger in BFS

			if (possibles.size() == 0)
				GetPossibleTiles(true, possibles);

			dir = GetDirection(possibles[0]);
			std::cout << "SUPER EXCEPTIONAL CASE~!!!!\N";
		}
	}

	return dir;
}

float Ghost::CalculatePathComplexity(const std::vector<sf::Vector2i>& path)
{
	// Calculates the xomplexity (junctions) of a path.
	// a junction needs to be perp to the current dir.
	if (path.size() < 2)
		return 0;

	float complexity = 0;
	sf::Vector2i prevDir = sf::Vector2i(GetDirection(path[0], path[1]));
	sf::Vector2i pos;
	sf::Vector2i dir;

	for (int i = 0; i < path.size() - 1; i++)
	{
		//if (path[i + 1].x == path[i].x || path[i + 1].y == path[i].y) check is not needed as path is continuous.
		{
			pos = path[i];
			dir = sf::Vector2i(GetDirection(path[i], path[i + 1]));

			int junctions = GetJunctions(pos, dir, prevDir);

			complexity += junctions;

			prevDir = dir;
		}
	}

	return complexity;
}

int Ghost::GetJunctions(sf::Vector2i pos, sf::Vector2i dir, sf::Vector2i prevDir)
{
	int junctions = 0;

	if (dir == prevDir)	// the travelling direction has not changed so check for perp valid tiles.
	{
		sf::Vector2i perp = Math::GetPerpVector(dir);

		if (!level->IsWall(pos + perp))
			junctions++;
		if (!level->IsWall(pos - perp))
			junctions++;
	}
	else				// the travelling direction has changed so a junction may be at -dir or +prevDir
	{
		if (!level->IsWall(pos - dir))
			junctions++;
		if (!level->IsWall(pos + prevDir))
			junctions++;
	}

	return junctions;
}

sf::Vector2i Ghost::GetFleeTargetPath(const std::vector<sf::Vector2i>& possibles, bool allowBackwardMovement)
{
	//Calculate the target based on which is set dir, taking into account paths starting from all possible tiles.

	sf::Vector2i bestPossible;
	sf::Vector2i pacManBlox = pacMan->GetBloxPosition();

	int size = possibles.size();

	if (possibles.size() == 0)
		bestPossible = pacMan->GetBloxPosition();					//if there is no possible way to go, stop moving (Error, paths are cyclic).

	else if (size == 1)												// one way(junction). No BFS needed.
		bestPossible = possibles[0];

	else															// in a multiple way situation calculate the path stats with a BFS and choose the best.
	{
		sf::Vector2f bannedDir = -GetDirection(possibles[0]);
		BFSInfo temp = PathFindTo(possibles[0], pacManBlox, bannedDir, true);
		BFSInfo bestPath = temp;
		bestPossible = possibles[0];					// bug |bestPossible = temp.target| target is not next to ghost. Condition (size > 1 && first path is best)

		for (int i = 1; i < size; i++)
		{
			bannedDir = -GetDirection(possibles[i]);
			temp = PathFindTo(possibles[i], pacManBlox, bannedDir, true);

			if (0.5f * temp.pathComplexity + 0.5f * temp.pathSize >= 0.5f * bestPath.pathComplexity + 0.5f * bestPath.pathSize)
			{
				bestPath = temp;
				bestPossible = possibles[i];
			}
		}
	}

	return bestPossible;
}

void Ghost::GetPossibleTiles(bool allowBackwarMovement, std::vector<sf::Vector2i>& possibles)
{
	//Checks if any the four possible ways a ghost can go from its position is valid and returns the array created.

	sf::Vector2i perp = sf::Vector2i(Math::GetPerpVector(dir));
	sf::Vector2i pos = GetBloxPosition();
	sf::Vector2i dir = sf::Vector2i(this->dir);

	if (allowBackwarMovement && !level->IsWall(pos - dir))
		possibles.push_back(pos - dir);

	if (!level->IsWall(pos + dir))
		possibles.push_back(pos + dir);

	if (!level->IsWall(pos + perp))
		possibles.push_back(pos + perp);

	if (!level->IsWall(pos - perp))
		possibles.push_back(pos - perp);
}

void Ghost::Chase()
{
}

void Ghost::OnFlee()
{
	// if state is start or dead then do not change it. Bug occurs with EntityManager timer that forces ghosts after their death to start chasing before the right time.
	// set flee flag and calculate moving direction taking into account the extra available path of -dir. This means a ghost can go backward in this situation.
	if (state == FLEE || state == START || state == DEAD)
		return;

	speed = ENTITY_SPEED - 1;
	//shape.setColor(sf::Color(255, 255, 255));
	state = FLEE;

	std::vector<sf::Vector2i> possibles;
	possibles.reserve(4);

	// Get the possible moving tiles (max 4 as -dir is included)
	GetPossibleTiles(true, possibles);

	// Get the direction based on the best of the possible paths (best: 60% pathComplexity, 40% pathLength). best is changed
	sf::Vector2f tempDir = GetDirection(GetFleeTargetPath(possibles, true));

	// if the new direction is colinear to the current direction
	if (Math::IsColinear(dir, tempDir))
		dir = tempDir;					// change it immediately so there are no movement glitches
	else								// if they are not colinear snap to tile and redo the pathfinding so as not to pass through walls. (wall bug)
	{
		SnapToTile(GetBloxPosition());
		GetPossibleTiles(true, possibles);
		dir = GetDirection(GetFleeTargetPath(possibles, true));
	}

	SetAnimationYCoord();
}

void Ghost::OnChase()
{
	// if state is start or dead then do not change it. Bug occurs with EntityManager timer that forces ghosts after their death to start chasing before the right time.
	if (state == CHASE || state == START || state == DEAD)
		return;

	state = CHASE;
	//shape.setColor(sf::Color::White);
	speed = ENTITY_SPEED;
}

void Ghost::Flee()
{
	std::vector<sf::Vector2i> possibles;

	possibles.reserve(3);

	// Calculate possible moving tiles (max 3)
	GetPossibleTiles(false, possibles);

	// Get the direction of the best path.
	dir = GetDirection(GetFleeTargetPath(possibles, false));
}

void Ghost::BacknForth()
{
	sf::Vector2i pos = GetBloxPosition();

	if (clk.getElapsedTime() >= sf::seconds(3))
	{
		state = CHASE;
		SnapToTile(GetBloxPosition());
		dir = GetDirection(PathFindTo(pos, pacMan->GetBloxPosition(), sf::Vector2f(0, 0), false).target);
		return;
	}

	if (dir == sf::Vector2f(1, 0) && pos == level->ghostStartLocation + sf::Vector2i(level->ghostStartSize.x, 0))
		dir = -dir;
	else if (dir == sf::Vector2f(-1, 0) && pos == level->ghostStartLocation)
		dir = -dir;
}

void Ghost::ReturnHome()
{
	if (level->ghostStartLocation + sf::Vector2i(1, 0) == GetBloxPosition())
	{
		Resurrect();
		return;
	}

	BFSInfo info = PathFindTo(GetBloxPosition(), level->ghostStartLocation, sf::Vector2f(0, 0), false);
	dir = GetDirection(info.target);
}

void Ghost::ProtectPowerFruits()
{
}

void Ghost::Die()
{
	shape.setColor(sf::Color(255, 255, 255, 100));		// set sprite to semi transparent to show death.
	state = DEAD;
	std::cout << "DEAD from update\n";
	speed = ENTITY_SPEED + 1;

	BFSInfo info = PathFindTo(GetBloxPosition(), level->ghostStartLocation, sf::Vector2f(0, 0), false);
	sf::Vector2f tempDir = GetDirection(info.target);

	if (Math::IsColinear(dir, tempDir) == false)		// if tempDir is not colinear to dir then snap to tile to fix a small glitch
		SnapToTile(pacMan->GetBloxPosition());

	dir = tempDir;
	SetAnimationYCoord();
}

void Ghost::Resurrect()
{
	shape.setColor(sf::Color::White);
	state = START;
	dir = sf::Vector2f(1, 0);
	speed = ENTITY_SPEED;
	clk.restart();					// restart the back n forth clock to last 3 seconds as it has been running from the beginning.
}

void Ghost::SetAnimationYCoord()
{
	if (dir.x == 1)
		animation.SetyCoord(3 * ENTITY_HEIGHT);
	else if (dir.x == -1)
		animation.SetyCoord(2 * ENTITY_HEIGHT);
	else if (dir.y == 1)
		animation.SetyCoord(1 * ENTITY_HEIGHT);
	else if (dir.y == -1)
		animation.SetyCoord(0);
}