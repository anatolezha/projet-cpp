#include "pch.h"
#include "Game.h"


Game::Game() : mWindow(sf::VideoMode(840, 600), "Space Invaders 1978", sf::Style::Close)
{
	mWindow.setFramerateLimit(160);
}


Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	while (mWindow.isOpen())
	{
		sf::Time elapsedTime = clock.restart();
		timeSinceLastUpdate += elapsedTime;
		while (timeSinceLastUpdate > TimePerFrame)
		{
			timeSinceLastUpdate -= TimePerFrame;
		}
	}
}

