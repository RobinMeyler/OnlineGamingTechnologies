#pragma 
#include <SFML/Graphics.hpp>

#include <thread>

#include "Server.h"
#include "Client.h"

#include "Player.h"

#include "Global.h"

class Game
{
public:
	Game();
	~Game();
	/// <summary>
	/// main method for game
	/// </summary>
	void run();
	void Clear(int id);
	void updatePosition(int t_id, float t_xPos, float t_yPos);
	void updateColor(int id, int t_newCol);
	void endGame(int end);
	static Game* m_gamePtr;
private:

	void processEvents();				// Loop functions
	void initialize();
	void update(sf::Time t_deltaTime);
	void render();
	static void listenForNewConnections(Server & t_server);
	static void updateServerPosition(Client & t_client);

	sf::Event event;
	sf::RenderWindow m_window; // main SFML window
	sf::Font m_ArialBlackfont; // font used by message
	bool m_exitGame; // control exiting game
	
	Player m_player;
	std::vector<Player*> m_otherPlayers;

	Server* m_server;
	Client* m_client;

	std::vector<std::thread*> m_gameThreads;
	bool isServer;
	int playersInGame = 0;

	sf::Clock m_clock;
	sf::Time m_timer;
	bool gameEnd{ false };
};

//static Game* gamePtr;
