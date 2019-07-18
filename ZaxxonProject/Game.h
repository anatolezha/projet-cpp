#pragma once
#include "Entity.h"

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
	void HandleTexts();

	void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);
	void HandleEnemyMoves();
	void HandleEnemyWeaponFiring();
	void HanldeEnemyWeaponMoves();
	void HandleCollisionWeaponEnemy();
	void HandleCollisionWeaponPlayer();
	void HandleCollisionEnemyPlayer();

	void HandleGameOver();
	void DisplayGameOver();
	

	bool inBounds(std::shared_ptr<Entity> entity);
	
private:
	static const float		PlayerSpeed;
	static const sf::Time	TimePerFrame;

	sf::Texture	mTexture;
	sf::Texture	_TextureWeapon;
	sf::Texture _TextureEnemy1;
	sf::Texture _TextureWeaponEnemy;
	sf::Texture _TextureExplosion;
	sf::Sprite	_Enemy1;
	sf::Sprite	_Enemy[5][11];
	sf::Sprite	mPlayer;
	sf::Font	mFont;
	sf::Text	mText;
	sf::Text	_LivesText;
	int _lives = 3;
	sf::Text	_ScoreText;
	int _score = 0;

	std::size_t	mStatisticsNumFrames;
	bool mIsMovingUp;
	bool mIsMovingDown;
	bool mIsMovingRight;
	bool mIsMovingLeft;
	bool _IsGameOver = false;

	int _CountEnemyWeaponFired = 0;
	int _CountPlayerWeaponFired = 0;

	sf::RenderWindow		mWindow;
};

