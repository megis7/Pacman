#include "EntityManager.h"

EntityManager::EntityManager(Level& level) : pacman(level, *this), level(level)
{
	ghosts.push_back(new Blinky(level, &pacman));
	ghosts.push_back(new Inky(level, &pacman, &ghosts[0]->shape.getPosition()));
	ghosts.push_back(new Clyde(level, &pacman));
}

EntityManager::~EntityManager()
{
	for (auto iter = ghosts.begin(); iter != ghosts.end(); iter++)
		delete *iter;
	ghosts.clear();
}

void EntityManager::Update(sf::Event event)
{
	pacman.Update(event);
	std::for_each(ghosts.begin(), ghosts.end(), [&event](Ghost* g){ g->Update(event); });

	if (fleeClock.getElapsedTime().asSeconds() >= fleeTime)
		Chase();
}

void EntityManager::Draw(sf::RenderWindow& window)
{
	pacman.Draw(window);
	std::for_each(ghosts.begin(), ghosts.end(), [&window](Ghost* g) { g->Draw(window); });
}

void EntityManager::Flee() const
{
	std::for_each(ghosts.begin(), ghosts.end(), [](Ghost* g){ g->OnFlee(); });
	const_cast<sf::Clock&>(fleeClock).restart();
}

void EntityManager::Chase() const
{
	std::for_each(ghosts.begin(), ghosts.end(), [](Ghost* g){ g->OnChase(); });
}