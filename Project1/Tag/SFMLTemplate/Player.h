#pragma once

#include <SFML/Graphics.hpp>
#include "MyVector3.h"
#include <ctime>

class Player
{
public:
	Player();
	int id{ -1 };
	void update(sf::Time t_deltatime);
	void render(sf::RenderWindow & t_renderwindow);

	bool isColliding(Player* m_otherPlayer);

	sf::CircleShape m_circle;
};

