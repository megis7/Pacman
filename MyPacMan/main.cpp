#include <SFML\Graphics.hpp>

#include "Level.h"
#include "EntityManager.h"
#include "Animation.h"

#include <vector>
#include <Windows.h>

#include <fstream>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

sf::Image image;
sf::Image inky;
sf::Image clyde;

void setupDebug()
{
	//track memory leaks

	FILE *pOut(nullptr);
	errno_t err = freopen_s(&pOut, "debug_out.txt", "w", stdout);

	if (err)
		std::cout << "error on freopen" << std::endl;
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF);

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
	_CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}

void FixImage(sf::Image& image, sf::Color c)
{
	for (int i = 0; i < image.getSize().x; i++)
		for (int j = 0; j < image.getSize().y; j++)
			if (image.getPixel(i, j).a < 100)	image.setPixel(i, j, sf::Color::Transparent);
			else if (image.getPixel(i, j) != sf::Color::White && image.getPixel(i, j) != sf::Color::Black)
				image.setPixel(i, j, c);
}

using namespace std;

int main()
{
	//setupDebug();
	unsigned int seed = time(0);
	std::cout << "Seed = " << seed << endl;
	srand(seed);
	image.loadFromFile("GhostAtlas.png");
	inky = image;
	clyde = image;

	FixImage(image, sf::Color::Red);
	FixImage(inky, sf::Color::Green);
	FixImage(clyde, sf::Color::Cyan);

	Animation::atlas.loadFromFile("atlas.png");

	sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "PacMan");
	sf::Event event;

	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(true);

	Level level;
	level.LoadContent("level1.txt");

	EntityManager manager(level);

	bool update = true;

	sf::Clock clock;
	sf::Time time;
	float fps = 0;
	int i = 0;

	float lastTime = 0;

	while (window.isOpen())
	{
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			else if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::P)
					update = !update;
				else if (event.key.code == sf::Keyboard::F)
					manager.Flee();
				else if (event.key.code == sf::Keyboard::C)
					manager.Chase();
				else if (event.key.code == sf::Keyboard::Space)
					manager.ghosts[0]->dir = sf::Vector2f();
				else if (event.key.code == sf::Keyboard::BackSpace)
				{
					manager.pacman.shape.setPosition(554, 500);
					manager.pacman.dir = sf::Vector2f(-1, 0);
					manager.ghosts[1]->shape.setPosition(540, 496);
					manager.ghosts[1]->dir = sf::Vector2f(0, 1);
				}
				else if (event.key.code == sf::Keyboard::L)
					manager.ghosts[1]->speed += 0.1f;
			}
		}

		if (!update)
			Sleep(150);

		manager.Update(event);

		window.clear(sf::Color::Black);

		level.Draw(window);
		manager.Draw(window);

		window.display();
	}

	return 0;
}