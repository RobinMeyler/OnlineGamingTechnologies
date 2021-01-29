#include "Game.h"
#include <string>

Game* Game::m_gamePtr{nullptr};

Game::Game() :
	m_window{ sf::VideoMode{ 1000, 800, 32 }, "Tag" },
	m_exitGame{ false },
	m_client(nullptr),
	m_server(nullptr)

{
	Game::m_gamePtr = this;
	m_client = new Client("127.0.0.1", 1111);// , this);
	if (!m_client->Connect())
	{
		// You're Server
		m_server = new Server(1111, false);
		std::thread PST(listenForNewConnections, std::ref(*m_server));
		PST.detach();
		m_gameThreads.push_back(&PST);
		isServer = true;
	}
	initialize();
}

Game::~Game()
{
}

void Game::run()
{
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;
	sf::Time timePerFrame = sf::seconds(1.f / 60.f); // 60 fps
	while (m_window.isOpen())
	{
		processEvents(); // as many as possible
		timeSinceLastUpdate += clock.restart();
		while (timeSinceLastUpdate > timePerFrame)
		{
			timeSinceLastUpdate -= timePerFrame;
			processEvents(); // at least 60 fps
			update(timePerFrame); //60 fps
		}
		render(); // as many as possible
	}
}

void Game::Clear(int id)
{
	for(int i = 0; i < m_otherPlayers.size(); i++)
	{
		if (id == m_otherPlayers.at(i)->id)
		{
			m_otherPlayers.erase(m_otherPlayers.begin() + i);
			return;
		}
	}
}

void Game::processEvents()
{
	while (m_window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type) // window message
		{
			m_window.close();
		}
		if (sf::Event::KeyPressed == event.type) //user key press
		{
			if (sf::Keyboard::Escape == event.key.code)
			{
				m_exitGame = true;
			}
		}
		if (event.type == sf::Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				
			}
		}
		if (event.type == sf::Event::MouseButtonReleased)
		{
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				
			}
		}
	}
}

void Game::initialize()
{
	if (isServer == true)
	{
		m_client->Connect();
	}

	m_client->sendPosition(Game::m_gamePtr->m_player.m_circle.getPosition().x, Game::m_gamePtr->m_player.m_circle.getPosition().y);
	if (m_player.m_circle.getFillColor() == sf::Color::Red)
		m_client->sendColor(0);
	else if (m_player.m_circle.getFillColor() == sf::Color::Blue)
		m_client->sendColor(1);
	else if (m_player.m_circle.getFillColor() == sf::Color::Green)
		m_client->sendColor(2);
	else if (m_player.m_circle.getFillColor() == sf::Color::White)
		m_client->sendColor(3);
	else if (m_player.m_circle.getFillColor() == sf::Color::Yellow)
		m_client->sendColor(4);
}

void Game::update(sf::Time t_deltaTime)
{
	if (m_exitGame)
	{
		m_window.close();		// Exiting the game
	}
	if (gameEnd == false)
	{
		if (playersInGame < m_otherPlayers.size())
		{
			m_client->sendPosition(Game::m_gamePtr->m_player.m_circle.getPosition().x, Game::m_gamePtr->m_player.m_circle.getPosition().y);

			if (m_player.m_circle.getFillColor() == sf::Color::Red)
				m_client->sendColor(0);
			else if (m_player.m_circle.getFillColor() == sf::Color::Blue)
				m_client->sendColor(1);
			else if (m_player.m_circle.getFillColor() == sf::Color::Green)
				m_client->sendColor(2);
			else if (m_player.m_circle.getFillColor() == sf::Color::White)
				m_client->sendColor(3);
			else if (m_player.m_circle.getFillColor() == sf::Color::Yellow)
				m_client->sendColor(4);
			playersInGame++;
			m_clock.restart();
		}
		if (playersInGame > 0)
		{
			m_timer = m_clock.getElapsedTime();
		}

		bool move = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_player.m_circle.setPosition(sf::Vector2f(m_player.m_circle.getPosition().x + 3, m_player.m_circle.getPosition().y));
			move = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			m_player.m_circle.setPosition(sf::Vector2f(m_player.m_circle.getPosition().x - 3, m_player.m_circle.getPosition().y));
			move = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			m_player.m_circle.setPosition(sf::Vector2f(m_player.m_circle.getPosition().x, m_player.m_circle.getPosition().y - 3));
			move = true;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			m_player.m_circle.setPosition(sf::Vector2f(m_player.m_circle.getPosition().x, m_player.m_circle.getPosition().y + 3));
			move = true;
		}

		if (move == true)
		{
			m_client->sendPosition(Game::m_gamePtr->m_player.m_circle.getPosition().x, Game::m_gamePtr->m_player.m_circle.getPosition().y);
		}

		m_player.update(t_deltaTime);
		for (auto otherPlayer : m_otherPlayers)
		{
			if (m_player.isColliding(otherPlayer))
			{
				gameEnd = true;
				m_client->sendEnd(1);
				std::cout << "Game lasted: " << std::to_string(m_timer.asSeconds()) << " seconds" << std::endl;
			}
		}
	}
}

void Game::render()
{
	m_window.clear(sf::Color::Black);

	for (auto otherPlayer : m_otherPlayers)
	{
		otherPlayer->render(m_window);
	}
	m_player.render(m_window);

	m_window.display();
}

void Game::listenForNewConnections(Server& t_server)
{
	while (true)
	{
		t_server.ListenForNewConnection();
	}
}

void Game::updatePosition(int t_id, float t_xPos, float t_yPos)
{
	for (auto player : m_otherPlayers)
	{
		if (t_id == player->id)
		{
			player->m_circle.setPosition(sf::Vector2f(t_xPos, t_yPos));
			return;
		}
	}
	// New player needed
	Player* otherPlayer = new Player();
	otherPlayer->id = t_id;
	otherPlayer->m_circle.setPosition(t_xPos, t_yPos);
	m_otherPlayers.push_back(otherPlayer);
}

void Game::updateColor(int id, int t_newCol)
{
	for (auto player : m_otherPlayers)
	{
		if (id == player->id)
		{
			switch (t_newCol)
			{
			case 0:
				player->m_circle.setFillColor(sf::Color::Red);
				break;
			case 1:
				player->m_circle.setFillColor(sf::Color::Blue);
				break;
			case 2:
				player->m_circle.setFillColor(sf::Color::Green);
				break;
			case 3:
				player->m_circle.setFillColor(sf::Color::White);
				break;
			case 4:
				player->m_circle.setFillColor(sf::Color::Yellow);
				break;
			default:
				break;
			}
		}
	}
}

void Game::endGame(int end)
{
	if (end == 1)
	{
		gameEnd = true;
	}
}
