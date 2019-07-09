#include "pch.h"
#include "Game.h"
#include "Entity.h"
#include "EntityManager.h"

const float Game::PlayerSpeed = 100.f;
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
	InitSprites();
}


Game::~Game()
{
}

void Game::InitSprites()
{

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


	int x = 0;
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if (i % 2 != 0 && j % 2 == 0) {
				_Enemy[i][j].setTexture(_TextureEnemy1);
				_Enemy[i][j].setPosition(840 + 100.f + 50.f * (i + 1) + x, 10.f + 50.f * (j + 1));
				std::shared_ptr<Entity> se = std::make_shared<Entity>();
				se->m_sprite = _Enemy[i][j];
				se->m_type = EntityType::enemy;
				se->m_size = _TextureEnemy1.getSize();
				se->m_position = _Enemy[i][j].getPosition();
				EntityManager::m_Entities.push_back(se);
			}
			else if (i % 2 == 0 && j % 2 != 0) {
				_Enemy[i][j].setTexture(_TextureEnemy1);
				_Enemy[i][j].setPosition(840 + 100.f + 50.f * (i + 1) + x, 10.f + 50.f * (j + 1));
				std::shared_ptr<Entity> se = std::make_shared<Entity>();
				se->m_sprite = _Enemy[i][j];
				se->m_type = EntityType::enemy;
				se->m_size = _TextureEnemy1.getSize();
				se->m_position = _Enemy[i][j].getPosition();
				EntityManager::m_Entities.push_back(se);
			}
		}
		x += 200;
	}

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
		if (isPressed == false)
		{
			return;
		}

		if (_IsPlayerWeaponFired == true)
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
		_IsPlayerWeaponFired = true;
	}

}


void Game::update(sf::Time elapsedTime)
{
	sf::Vector2f movement(0.f, 0.f);
	if (mIsMovingUp)
		movement.y -= PlayerSpeed;
	if (mIsMovingDown)
		movement.y += PlayerSpeed;
	if (mIsMovingLeft)
		movement.x -= PlayerSpeed;
	if (mIsMovingRight)
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

	HandleCollisionWeaponEnemy();
	HanldeWeaponMoves();
	HandleEnemyWeaponFiring();
	HanldeEnemyWeaponMoves();
	HandleEnemyMoves();

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
		x+=4;

		if (x >= mWindow.getSize().x)
		{
			entity->m_enabled = false;
			_IsPlayerWeaponFired = false;
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
		x -= 1.0f;



		if (x <= 0)
		{
			entity->m_enabled = false;
			_IsEnemyWeaponFired = false;
		}
		else
		{
			entity->m_sprite.setPosition(x, y);
		}




	}
}

void Game::HandleEnemyWeaponFiring()
{
	
	if (_IsEnemyWeaponFired == true)
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

			_IsEnemyWeaponFired = true;
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
		x -= 4.0f;

		if (x <= 0)
		{
			entity->m_enabled = false;
			_IsEnemyWeaponFired = false;
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
				_IsPlayerWeaponFired = false;
				goto end;
			}
		}
	}

end:
	//nop
	return;
}

bool Game::inBounds(std::shared_ptr<Entity> entity) {
	if (entity->m_sprite.getPosition().x > mWindow.getSize().x) {
		return false;
	}
	return true;
}

