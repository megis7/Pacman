#include "Behaviour.h"


Behaviour::Behaviour(Ghost* ghost, PacMan* pacman, Level* level)
{
	this->ghost = ghost;
	this->pacman = pacman;
	this->level = level;
}


Behaviour::~Behaviour()
{
}

BFSInfo Behaviour::PathFindTo(sf::Vector2i from, sf::Vector2i to)
{
	return PathFindTo(from, to, -ghost->dir, false);
}

BFSInfo Behaviour::PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDir, bool calculateComplexity)
{
	std::function<bool(sf::Vector2i)> restr = std::bind(&Ghost::PathRestrictions, this, std::placeholders::_1);

	return PathFindTo(from, to, bannedDir, calculateComplexity, false, restr);
}

BFSInfo Behaviour::PathFindTo(sf::Vector2i from, sf::Vector2i to, sf::Vector2f bannedDir, bool calculateComplexity, bool inverted, FBV2i restr)
{
	// if position and target are the same BFS stucks so return and of course there is no path to determine.
	sf::Vector2i current = from;

	if (current == to)
	{
		ghost->ClearPath();
		return BFSInfo(0, to);
	}

	bool visited[BLOCKS_PER_ROW][BLOCKS_PER_COLUMN];
	sf::Vector2i parents[BLOCKS_PER_ROW][BLOCKS_PER_COLUMN];

	for (int i = 0; i < BLOCKS_PER_ROW; i++)
	for (int j = 0; j < BLOCKS_PER_COLUMN; j++)
		visited[i][j] = level->grid[i][j].type == Tile::WALL ? true : false;	// mark every wall tile as visited.

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

		// restrict BFS according to direction. This applies only to the first pass of the BFS 
		// where adjacent to the current tile tiles are searched.

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

	//return path[path.size() - 1];

	// Reverse the path to have it correctly. Keep for debug purposes and visual drawing
	/*for (int i = 0; i < path.size() / 2; i++)
	{
	sf::Vector2i temp = path[i];
	path[i] = path[path.size() - i - 1];
	path[path.size() - i - 1] = temp;
	}*/

	sf::Vector2i target;

	if (inverted)
		target = path[0];
	else
		target = path[path.size() - 1];

	//gpath = path;

	float complexity = -1;

	if (calculateComplexity)
		complexity = CalculatePathComplexity(path);

	return BFSInfo(path.size(), target, complexity);
}

float Behaviour::CalculatePathComplexity(const std::vector<sf::Vector2i>& path)
{
	// Calculates the xomplexity (junctions) of a path.
	// a junction needs to be perp to the current dir.
	if (path.size() < 2)
		return 0;

	float complexity = 0;
	sf::Vector2i prevDir = sf::Vector2i(ghost->GetDirection(path[0], path[1]));
	sf::Vector2i pos;
	sf::Vector2i dir;

	for (int i = 0; i < path.size() - 1; i++)
	{
		//if (path[i + 1].x == path[i].x || path[i + 1].y == path[i].y) check is not needed as path is continuous.
		{
			pos = path[i];
			dir = sf::Vector2i(ghost->GetDirection(path[i], path[i + 1]));

			int junctions = ghost->GetJunctions(pos, dir, prevDir);

			complexity += junctions;

			prevDir = dir;
		}
	}

	return complexity / path.size() * 100;
}