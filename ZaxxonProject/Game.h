#pragma once

class Game
{
public:
	Game();
	~Game();
	void run();

private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::RenderWindow		mWindow;
};

