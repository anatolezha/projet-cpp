#pragma once

class Game
{
public:
	Game();
	~Game();
	void update(sf::Time elapsedTime);
	void run();


private: 
	void InitSprites();
	void render();

	void processEvents();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);


private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::Texture	mTexture;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;


	sf::RenderWindow		mWindow;
};

