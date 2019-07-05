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
	mPlayer.setRotation(90.f);
	std::shared_ptr<Entity> player = std::make_shared<Entity>();
	player->m_sprite = mPlayer;
	player->m_type = EntityType::player;
	player->m_size = mTexture.getSize();
	player->m_position = mPlayer.getPosition();
	EntityManager::m_Entities.push_back(player);

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

