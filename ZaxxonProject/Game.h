#pragma once

class Game
{
public:
	Game();
	~Game();
	void update(sf::Time elapsedTime);
	void HanldeWeaponMoves();
	void run();

private: 
	void InitSprites();
	void render();

	void processEvents();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void HandleEnemyMoves();
	void HandleEnemyWeaponFiring();
	void HanldeEnemyWeaponMoves();


private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::Texture	mTexture;
	sf::Texture	_TextureWeapon;
	sf::Texture _TextureEnemy1;
	sf::Texture _TextureWeaponEnemy;
	sf::Sprite	_Enemy1;
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mStatisticsText;
	sf::Time	mStatisticsUpdateTime;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;

	bool _IsPlayerWeaponFired = false;
	bool _IsEnemyWeaponFired = false;


	sf::RenderWindow		mWindow;
};

