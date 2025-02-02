#pragma once
#include "../Components/Components.hpp"
#include "../Entity/Entity.hpp"
#include "../Entity/EntityManager.hpp"
#include <SFML/Graphics.hpp>
struct WINDOW_CONFIG {
	unsigned int width, height, frameLimit, fullScreenMode;
};

struct FONT_CONFIG {
	std::string fontFile;
	int fontSize, r, g, b;
};

struct PLAYER_CONFIG {
	int SR, CR, S, FR, FG, FB, OR, OG, OB, OT, V;
};

struct ENEMY_CONFIG {
	int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI;
	float SMAX, SMIN;
};

struct BULLET_CONFIG {
	int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
	float S;
};

class GameEngine {
public:
	GameEngine();
	void run();
private:
	void init();
	void setPaused(bool paused);

	void sMovement();
	void sUserInput();
	void sLifespan();
	void sRender();
	void sEnemySpawner();
	void sCollision();

	void spawnPlayer();
	void spawnEnemy();
	void spawnSmallEnemies(std::shared_ptr<Entity> entity);
	void spawnBullet(std::shared_ptr<Entity> entity, const sf::Vector2f& mousePos);
	void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

	void update();
private:
	sf::RenderWindow		m_window;
	sf::Font				m_font;
	EntityManager			m_entities;
	std::shared_ptr<Entity> m_player;

	int						m_score					= 0;
	int						m_currentFrame			= 0;
	int						m_lastEnemySpawnTime	= 0;
	bool					m_paused				= false;
	bool					m_running				= true;
	float					PI						= atan2f(1,1)*4;

	WINDOW_CONFIG	windowConfig;
	FONT_CONFIG		fontConfig;
	PLAYER_CONFIG	playerConfig;
	ENEMY_CONFIG	enemyConfig;
	BULLET_CONFIG	bulletConfig;
};