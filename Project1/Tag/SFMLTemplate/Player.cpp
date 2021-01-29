#include "Player.h"
#include "Global.h"

Player::Player()
{
	srand(time(nullptr));
	m_circle.setFillColor(sf::Color::Red);
	m_circle.setRadius(40);
	m_circle.setPosition(sf::Vector2f(rand() % 1000, rand() % 800));
	int col = rand() % 5;
	Color color = (Color)col;
	switch (color)
	{
	case red:
		m_circle.setFillColor(sf::Color::Red);
		break;
	case blue:
		m_circle.setFillColor(sf::Color::Blue);
		break;
	case green:
		m_circle.setFillColor(sf::Color::Green);
		break;
	case white:
		m_circle.setFillColor(sf::Color::White);
		break;
	case yellow:
		m_circle.setFillColor(sf::Color::Yellow);
		break;
	default:
		break;
	}
	m_circle.setOrigin(40, 40);
}

void Player::update(sf::Time t_deltatime)
{
	if (m_circle.getPosition().x < 0)
	{
		m_circle.setPosition(sf::Vector2f(1000, m_circle.getPosition().y));
	}
	else if (m_circle.getPosition().x > 1000)
	{
		m_circle.setPosition(sf::Vector2f(0, m_circle.getPosition().y));
	}
	if (m_circle.getPosition().y < 0)
	{
		m_circle.setPosition(sf::Vector2f(m_circle.getPosition().x, 800));
	}
	if (m_circle.getPosition().y > 800)
	{
		m_circle.setPosition(sf::Vector2f(m_circle.getPosition().x, 0));
	}

}

void Player::render(sf::RenderWindow& t_renderwindow)
{
	t_renderwindow.draw(m_circle);
}

bool Player::isColliding(Player* m_otherPlayer)
{
	MyVector3 playerPos = m_circle.getPosition();
	MyVector3 otherPlayerPos = m_otherPlayer->m_circle.getPosition();

	if((otherPlayerPos - playerPos).length() < (m_circle.getRadius() * 2)) // Colliding
	{
		return true;
	}
	return false;
}
