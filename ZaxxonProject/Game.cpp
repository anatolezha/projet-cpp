#include "pch.h"
#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 150.f;
const sf::Time Game::TimePerFrame = sf::seconds(1.f / 60.f);

Game::Game() : mWindow(sf::VideoMode(840, 600), "Zaxxon Project", sf::Style::Close)
, mTexture()
, mPlayer()
, mFont()
, mIsMovingUp(false)
, mIsMovingDown(false)
, mIsMovingRight(false)
, mIsMovingLeft(false)
{
	mWindow.setFramerateLimit(160);
	mTexture.loadFromFile("Media/textures/playerSpaceship.png");
	_TextureWeapon.loadFromFile("Media/textures/playerWeapon.png");
	_TextureWeaponEnemy.loadFromFile("Media/textures/enemyWeapon.png");
	_TextureEnemy1.loadFromFile("Media/textures/enemy1.png");
	mFont.loadFromFile("Media/Font/ARCADECLASSIC.TTF");

	InitSprites();
}


Game::~Game()
{
}

void Game::InitSprites()
{
	_lives = 3;
	_score = 0;
	_IsGameOver = false;
	//
	// Player
	// Initialisation du sprite du joueur (texture, position, rotation) puis ajout à l'entity manager pour l'afficher

	mPlayer.setTexture(mTexture);
	mPlayer.setPosition(50.f, 275.f);
	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

	//
	// Lives
	//

	_LivesText.setFillColor(sf::Color::Green);
	_LivesText.setFont(mFont);
	_LivesText.setPosition(10.f, 50.f);
	_LivesText.setCharacterSize(20);
	_LivesText.setString(std::to_string(_lives));

	//
	// Text
	//

	_ScoreText.setFillColor(sf::Color::Green);
	_ScoreText.setFont(mFont);
	_ScoreText.setPosition(10.f, 100.f);
	_ScoreText.setCharacterSize(20);
	_ScoreText.setString(std::to_string(_score));

}

void Game::render()
{
	mWindow.clear();

	//Affichage des entity présent dans l'entity manager
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		mWindow.draw(entity->m_sprite);
	}

	//afichage de la fenêtre

	mWindow.draw(_LivesText);
	mWindow.draw(_ScoreText);
	mWindow.draw(mText);
	mWindow.display();
}

void Game::processEvents()
{
	//Gestion des actions de l'utilisateur

	sf::Event event;
	while (mWindow.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			handlePlayerInput(event.key.code, true);
			break;

		case sf::Event::KeyReleased:
			handlePlayerInput(event.key.code, false);
			break;
			
		case sf::Event::Closed:
			mWindow.close();
			break;
		}
	}
}

void Game::handlePlayerInput(sf::Keyboard::Key key, bool isPressed)
{
	if (_IsGameOver == true)
		return;
	if (key == sf::Keyboard::Up)
		mIsMovingUp = isPressed;
	else if (key == sf::Keyboard::Down)
		mIsMovingDown = isPressed;
	else if (key == sf::Keyboard::Left)
		mIsMovingLeft = isPressed;
	else if (key == sf::Keyboard::Right)
		mIsMovingRight = isPressed;


	if (key == sf::Keyboard::Space)
	{
		if (isPressed != false)
		{
			return;
		}

		if (_CountPlayerWeaponFired >= 3)
		{
			return;
		}

		std::shared_ptr<Entity> sw = std::make_shared<Entity>();
		sw->m_sprite.setTexture(_TextureWeapon);
		sw->m_sprite.setPosition(
			EntityManager::GetPlayer()->m_sprite.getPosition().x + EntityManager::GetPlayer()->m_sprite.getTexture()->getSize().y / 2,
			EntityManager::GetPlayer()->m_sprite.getPosition().y + EntityManager::GetPlayer()->m_sprite.getTexture()->getSize().y / 2);
		sw->m_type = EntityType::weapon;
		sw->m_size = _TextureWeapon.getSize();
		EntityManager::m_Entities.push_back(sw);
		_CountPlayerWeaponFired++;
	}

}


void Game::update(sf::Time elapsedTime)
{
	if (_IsGameOver == true)
		return;
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp && EntityManager::GetPlayer()->m_sprite.getPosition().y > 0)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown && EntityManager::GetPlayer()->m_sprite.getPosition().y < mWindow.getSize().y - EntityManager::GetPlayer()->m_sprite.getTexture()->getSize().y)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft && EntityManager::GetPlayer()->m_sprite.getPosition().x > 0)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight && EntityManager::GetPlayer()->m_sprite.getPosition().x  < mWindow.getSize().x - EntityManager::GetPlayer()->m_sprite.getTexture()->getSize().x)
		movement.x += PlayerSpeed;

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::player)
		{
			continue;
		}

		entity->m_sprite.move(movement * elapsedTime.asSeconds());
	}
	
	GenerateEnemies();
	HandleTexts();
	HandleGameOver();
	HandleCollisionWeaponEnemy();
	HandleCollisionWeaponPlayer();
	HandleCollisionEnemyPlayer();
	HanldeWeaponMoves();
	HandleEnemyWeaponFiring();
	HanldeEnemyWeaponMoves();
	HandleEnemyMoves();

}

void Game::GenerateEnemies()
{
	if (_SecondElapsed == 50) {
		
		sf::Sprite newEnemy;
		newEnemy.setTexture(_TextureEnemy1);
		int r = rand() % (600 - _TextureEnemy1.getSize().y) + 1;
		newEnemy.setPosition(840, r);
		std::shared_ptr<Entity> se = std::make_shared<Entity>();
		se->m_sprite = newEnemy;
		se->m_type = EntityType::enemy;
		se->m_size = _TextureEnemy1.getSize();
		se->m_position = newEnemy.getPosition();
		EntityManager::m_Entities.push_back(se);


		_SecondElapsed = 0;
	} else {
		_SecondElapsed += 1;
		//std::cout << _SecondElapsed;
	}
}

void Game::HanldeWeaponMoves()
{
	//
	// Handle Weapon moves
	//

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::weapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x+=6;

		if (x >= mWindow.getSize().x)
		{
			entity->m_enabled = false;
			_CountPlayerWeaponFired--;
		}

		entity->m_sprite.setPosition(x, y);
	}
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

			processEvents();
			update(TimePerFrame);
		}
		render();
	}


}

void Game::HandleTexts()
{
	std::string lives = "Lives  " + std::to_string(_lives);
	_LivesText.setString(lives);
	std::string score = "Score  " + std::to_string(_score);
	_ScoreText.setString(score);
	return;
}

void Game::HandleEnemyMoves()
{

	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemy)
		{
			continue;

		}
		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x -= 2.0f;



		if (x <= 0)
		{
			entity->m_enabled = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}




	}
}

void Game::HandleEnemyWeaponFiring()
{
	if (_CountEnemyWeaponFired >= 3)
		return;
	
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (!inBounds(entity)) {
			continue;
		}

		if (entity->m_type == EntityType::enemy)
		{
			int r = rand() % 20;
			if (r != 10)
				continue;

			float x, y;
			x = entity->m_sprite.getPosition().x;
			y = entity->m_sprite.getPosition().y;
			

			std::shared_ptr<Entity> sw = std::make_shared<Entity>();
			sw->m_sprite.setTexture(_TextureWeaponEnemy);
			sw->m_sprite.setPosition(
				x ,
				y + _TextureEnemy1.getSize().x / 2 );

			sw->m_type = EntityType::enemyWeapon;
			sw->m_size = _TextureWeaponEnemy.getSize();
			EntityManager::m_Entities.push_back(sw);

			_CountEnemyWeaponFired++;
			break;
		}
	}
}

void Game::HanldeEnemyWeaponMoves()
{
	for (std::shared_ptr<Entity> entity : EntityManager::m_Entities)
	{
		if (entity->m_enabled == false)
		{
			continue;
		}

		if (entity->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		float x, y;
		x = entity->m_sprite.getPosition().x;
		y = entity->m_sprite.getPosition().y;
		x -= 6.0f;

		if (x <= 0)
		{
			entity->m_enabled = false;
			_CountEnemyWeaponFired--;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}
	}
}

void Game::HandleCollisionWeaponEnemy()
{
	// Handle collision weapon enemies

	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::weapon)
		{
			continue;
		}

		for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
		{
			if (enemy->m_type != EntityType::enemy)
			{
				continue;
			}

			if (enemy->m_enabled == false)
			{
				continue;
			}

			sf::FloatRect boundWeapon;
			boundWeapon = weapon->m_sprite.getGlobalBounds();

			sf::FloatRect boundEnemy;
			boundEnemy = enemy->m_sprite.getGlobalBounds();

			if (boundWeapon.intersects(boundEnemy) == true)
			{
				enemy->m_enabled = false;
				weapon->m_enabled = false;
				_CountPlayerWeaponFired--;
				_score += 10;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionEnemyPlayer()
{
	for (std::shared_ptr<Entity> enemy : EntityManager::m_Entities)
	{
		if (enemy->m_type != EntityType::enemy)
		{
			continue;
		}

		if (enemy->m_enabled == false)
		{
			continue;
		}

		sf::FloatRect boundEnemy;
		boundEnemy = enemy->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundEnemy.intersects(boundPlayer) == true)
		{
			enemy->m_enabled = false;
			_lives--;
			goto end;
		}
	}

end:
	//nop
	return;
}

void Game::HandleCollisionWeaponPlayer()
{
	for (std::shared_ptr<Entity> weapon : EntityManager::m_Entities)
	{
		if (weapon->m_enabled == false)
		{
			continue;
		}

		if (weapon->m_type != EntityType::enemyWeapon)
		{
			continue;
		}

		sf::FloatRect boundWeapon;
		boundWeapon = weapon->m_sprite.getGlobalBounds();

		sf::FloatRect boundPlayer;
		boundPlayer = EntityManager::GetPlayer()->m_sprite.getGlobalBounds();

		if (boundWeapon.intersects(boundPlayer) == true)
		{
			weapon->m_enabled = false;
			_CountEnemyWeaponFired--;
			_lives--;
			goto end;
		}
	}

end:
	//nop
	return;
}


void Game::HandleGameOver()
{
	
	if (_lives == 0)
	{
		DisplayGameOver();
	}
}

void Game::DisplayGameOver()
{
	
		mText.setFillColor(sf::Color::Green);
		mText.setFont(mFont);
		mText.setPosition(200.f, 200.f);
		mText.setCharacterSize(80);
		mText.setString("GAME OVER");

		_ScoreText.setPosition(260.f, 270.f);
		_ScoreText.setCharacterSize(60);

		_IsGameOver = true;
	
}


bool Game::inBounds(std::shared_ptr<Entity> entity) {
	if (entity->m_sprite.getPosition().x > mWindow.getSize().x) {
		return false;
	}
	return true;
}

