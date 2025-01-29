#pragma once
#include "../Components/Components.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/EntityManager.hpp"
#include <SFML/Graphics.hpp>

class GameEngine {
public:
	void sMovement();
	void sUserInput();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void update();
private:
	void init();
private:
	sf::RenderWindow m_window;
	EntityManager m_entities;
	Entity m_player;
	bool m_paused;
	bool m_running;
};