#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include "GameEngine.hpp"


void GameEngine::init() {
	std::ifstream in("C:\\Users\\JSH\\Source\\Repos\\cmake-sfml-project\\src\\Configuration\\config.txt");
	if (!in.is_open()) {
		std::cerr << "Failed to open config\n";
		exit(-1);
	}

	std::istringstream iss;
	std::string lineInConfig;
	std::string specifier;
	
	sf::ContextSettings settings;
	settings.antiAliasingLevel = 8;

	while(in) {
		std::getline(in, lineInConfig);
		iss.clear();
		iss.str(lineInConfig);
		iss >> specifier;
		if (specifier == "Window") {
			iss >> windowConfig.width >> windowConfig.height >> windowConfig.frameLimit >> windowConfig.fullScreenMode;
			m_window.create(sf::VideoMode({ windowConfig.width, windowConfig.height }),
				"Assignemnt 2",
				sf::Style::Titlebar | sf::Style::Close,
				(windowConfig.fullScreenMode == 0) ? sf::State::Windowed : sf::State::Fullscreen,
				settings);
			m_window.setFramerateLimit(windowConfig.frameLimit);
		}
		else if (specifier == "Font") {
			iss >> fontConfig.fontFile >> fontConfig.fontSize >> fontConfig.r >> fontConfig.g >> fontConfig.b;
		}
		else if (specifier == "Player") {
			iss >> playerConfig.SR >> playerConfig.CR >> playerConfig.S
				>> playerConfig.FR >> playerConfig.FG >> playerConfig.FB
				>> playerConfig.OR >> playerConfig.OG >> playerConfig.OB
				>> playerConfig.OT >> playerConfig.V;
		}
		else if (specifier == "Enemy") {
			iss >> enemyConfig.SR >> enemyConfig.CR >> enemyConfig.SMIN >> enemyConfig.SMAX
				>> enemyConfig.OR >> enemyConfig.OG >> enemyConfig.OB >> enemyConfig.OT
				>> enemyConfig.VMIN >> enemyConfig.VMAX >> enemyConfig.L >> enemyConfig.SI;
		}
		else if (specifier == "Bullet") {
			iss >> bulletConfig.SR >> bulletConfig.CR >> bulletConfig.S
				>> bulletConfig.FR >> bulletConfig.FG >> bulletConfig.FB
				>> bulletConfig.OR >> bulletConfig.OG >> bulletConfig.OB
				>> bulletConfig.OT >> bulletConfig.V >> bulletConfig.L;
		}
		else
			;
	}
	in.close();
}

void GameEngine::setPaused(bool paused)
{
	m_paused = paused;
}

GameEngine::GameEngine()
{
	init();

	spawnPlayer();
}

void GameEngine::sMovement()
{
	m_player->cTransform->velocity = { 0,0 };
	if (m_player->cInput->up) {
		m_player->cTransform->velocity.y = -playerConfig.S;
	}
	if (m_player->cInput->left) {
		m_player->cTransform->velocity.x = -playerConfig.S;
	}
	if (m_player->cInput->down) {
		m_player->cTransform->velocity.y = playerConfig.S;
	}
	if (m_player->cInput->right) {
		m_player->cTransform->velocity.x = playerConfig.S;
	}
	m_player->cTransform->pos.x += m_player->cTransform->velocity.x;
	m_player->cTransform->pos.y += m_player->cTransform->velocity.y;

	auto& enemies = m_entities.getEntities("Enemy");
	for (auto& enemy : enemies) {
		if (enemy->isAlive()) {
			enemy->cTransform->pos.x += enemy->cTransform->velocity.x;
			enemy->cTransform->pos.y += enemy->cTransform->velocity.y;
		}
	}

	auto& bullets = m_entities.getEntities("Bullet");
	for (auto& bullet : bullets) {
		bullet->cTransform->pos.x += bullet->cTransform->velocity.x;
		bullet->cTransform->pos.y += bullet->cTransform->velocity.y;
	}

	auto& smallEnemies = m_entities.getEntities("SmallEnemy");
	for (auto& smallEnemy : smallEnemies) {
		smallEnemy->cTransform->pos += smallEnemy->cTransform->velocity;
	}
}

void GameEngine::sUserInput()
{
	while (const std::optional event = m_window.pollEvent()) {
		if (event->is<sf::Event::Closed>()) {
			m_running = false;
		}
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->scancode == sf::Keyboard::Scancode::Escape) {
				m_running = false;
				break;
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::W) {
				m_player->cInput->up = true;
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::A) {
				m_player->cInput->left = true;
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::S) {
				m_player->cInput->down = true;
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::D) {
				m_player->cInput->right = true;
			}
			else if (keyPressed->scancode == sf::Keyboard::Scancode::P) {
				setPaused(!m_paused);
			}
		}
		if (const auto* keyReleased = event->getIf<sf::Event::KeyReleased>()) {
			switch (keyReleased->scancode) {
			case sf::Keyboard::Scancode::W:
				m_player->cInput->up = false;
				break;
			case sf::Keyboard::Scancode::A:
				m_player->cInput->left = false;
				break;
			case sf::Keyboard::Scancode::S:
				m_player->cInput->down = false;
				break;
			case sf::Keyboard::Scancode::D:
				m_player->cInput->right = false;
				break;
			}
		}
		if (const auto* mousedButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
			if (mousedButtonPressed->button == sf::Mouse::Button::Left) {
				auto [x, y] = mousedButtonPressed->position;
				spawnBullet(m_player, sf::Vector2f{ (float)x, (float)y });
			}
		}
	}
}

void GameEngine::sLifespan()
{
	auto& bullets = m_entities.getEntities("Bullet");
	for (auto& bullet : bullets) {
		--bullet->cLifespan->remaining;
		if (bullet->cLifespan->remaining < 0) {
			bullet->destory();
		}
	}
	auto& smallEnemies = m_entities.getEntities("SmallEnemy");
	for (auto& smallEnemy : smallEnemies) {
		--smallEnemy->cLifespan->remaining;
		if (smallEnemy->cLifespan->remaining < 0) {
			smallEnemy->destory();
		}
	}
}

void GameEngine::sRender()
{
	m_window.clear();
	const auto& entities = m_entities.getEntities();
	for (const auto& entity : entities) {
		if (entity->isAlive()) {
			auto& transform = entity->cTransform;
			auto& shape = entity->cShape;
			shape->circle.setPosition(transform->pos);
			transform->angle += sf::radians(PI / 64);
			shape->circle.setRotation(transform->angle);
			if (entity->cLifespan) {
				auto color = entity->cShape->circle.getFillColor();
				float ratio = ((float)entity->cLifespan->remaining / entity->cLifespan->total) * 255.f;
				color.a = ratio;
				entity->cShape->circle.setFillColor(color);
				entity->cShape->circle.setOutlineColor(color);
			}
			m_window.draw(entity->cShape->circle);
		}
	}

	m_window.display();
}

void GameEngine::sEnemySpawner()
{
	if (m_currentFrame - m_lastEnemySpawnTime >= enemyConfig.SI) {
		spawnEnemy();
	}
}

void GameEngine::sCollision()
{
	auto& playerPos = m_player->cTransform->pos;
	auto& playerCR = m_player->cCollision->radius;
	auto [wWidth, wHeight] = m_window.getSize();
	if (playerPos.x <= playerCR) {
		playerPos.x = playerCR;
	}
	if (playerPos.y <= playerCR) {
		playerPos.y = playerCR;
	}
	if (playerPos.x >= wWidth - playerCR) {
		playerPos.x = wWidth - playerCR;
	}
	if (playerPos.y >= wHeight - playerCR) {
		playerPos.y = wHeight - playerCR;
	}

	auto& enemies = m_entities.getEntities("Enemy");
	for (auto& enemy : enemies) {
		auto& enemyPos = enemy->cTransform->pos;
		auto& enemyVelocity = enemy->cTransform->velocity;
		auto& enemyCR = enemy->cCollision->radius;

		if (enemyPos.x <= enemyCR) {
			enemyPos.x = enemyCR;
			enemyVelocity.x = -enemyVelocity.x;
		}
		if (enemyPos.y <= enemyCR) {
			enemyPos.y = enemyCR;
			enemyVelocity.y = -enemyVelocity.y;
		}
		if (enemyPos.x >= wWidth-enemyCR) {
			enemyPos.x = wWidth-enemyCR;
			enemyVelocity.x = -enemyVelocity.x;
		}
		if (enemyPos.y >= wHeight-enemyCR) {
			enemyPos.y = wHeight-enemyCR;
			enemyVelocity.y = -enemyVelocity.y;
		}
	}

	auto& bullets = m_entities.getEntities("Bullet");
	for (auto& enemy : enemies) {
		for (auto& bullet : bullets) {
			const auto& enemyPos = enemy->cTransform->pos;
			const auto& bulletPos = bullet->cTransform->pos;
			const auto& enemyRadius = enemy->cCollision->radius;
			const auto& bulletRadius = bullet->cCollision->radius;
			sf::Vector2f diffVec{ enemyPos - bulletPos };
			float actualDistance = diffVec.x * diffVec.x + diffVec.y * diffVec.y;
			float collisionDistance = (enemyRadius + bulletRadius) * (enemyRadius + bulletRadius);
			if (actualDistance <= collisionDistance) {
				bullet->destory();
				enemy->destory();
				spawnSmallEnemies(enemy);
			}
		}
	}

}

void GameEngine::spawnPlayer()
{
	m_player = m_entities.addEntity("Player");
	m_player->cTransform = std::make_shared<CTransform>(sf::Vector2f{ (float)windowConfig.width / 2, (float)windowConfig.height / 2 },
														sf::Vector2f{ (float)playerConfig.V, (float)playerConfig.V });
	m_player->cCollision = std::make_shared<CCollision>((float)playerConfig.CR);
	m_player->cInput = std::make_shared<CInput>();
	m_player->cShape = std::make_shared<CShape>((float)playerConfig.SR, playerConfig.V, 
												sf::Color(playerConfig.FR, playerConfig.FG, playerConfig.FB),
												sf::Color(playerConfig.OR, playerConfig.OG, playerConfig.OB),
												playerConfig.OT);
}

void GameEngine::spawnEnemy()
{
	auto e = m_entities.addEntity("Enemy");
	int vertices = enemyConfig.VMIN + rand() % (enemyConfig.VMAX - enemyConfig.VMIN + 1);
	e->cShape = std::make_shared<CShape>((float)enemyConfig.SR, (size_t)vertices, sf::Color(rand() % 255, rand() % 255, rand() % 255),
		sf::Color(enemyConfig.OR, enemyConfig.OG, enemyConfig.OB), enemyConfig.OT);

	auto [wWidth, wHeight] = m_window.getSize();
	int x = enemyConfig.SR + rand() % (wWidth - 2 * enemyConfig.SR);
	int y = enemyConfig.SR + rand() % (wHeight - 2 * enemyConfig.SR);
	int vx = enemyConfig.VMIN + rand() % (enemyConfig.VMAX - enemyConfig.VMIN + 1);
	int vy = enemyConfig.VMIN + rand() % (enemyConfig.VMAX - enemyConfig.VMIN + 1);
	vx = (rand() & 1) ? -vx : vx;
	vy = (rand() & 1) ? -vy : vy;
	e->cTransform = std::make_shared<CTransform>(sf::Vector2f{ (float)x, (float)y }, sf::Vector2f{ (float)vx, (float)vy });

	e->cCollision = std::make_shared<CCollision>((float)enemyConfig.CR);
	m_lastEnemySpawnTime = m_currentFrame;
}

void GameEngine::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
	const auto& vertices = entity->cShape->circle.getPointCount();
	float theta = 2 * PI / vertices;
	const auto& entityRadius = entity->cCollision->radius;
	const auto& entityPos = entity->cTransform->pos;
	for (int i= 1; i<= vertices; ++i) {
		sf::Vector2f directionVec{ std::cosf(i * theta), std::sinf(i * theta) };
		auto smallEnemy = m_entities.addEntity("SmallEnemy");
		smallEnemy->cShape = std::make_shared<CShape>(*entity->cShape);
		smallEnemy->cTransform = std::make_shared<CTransform>(entity->cTransform->pos, directionVec);
		smallEnemy->cShape->circle.setScale(sf::Vector2f{ 0.5f, 0.5f });
		smallEnemy->cLifespan = std::make_shared<CLifespan>(enemyConfig.L);
	}
}

void GameEngine::spawnBullet(std::shared_ptr<Entity> entity, const sf::Vector2f& mousePos)
{
	sf::Vector2f& entityPos = entity->cTransform->pos;
	float entityRadius = entity->cCollision->radius;
	sf::Vector2f diffVec{ mousePos.x - entityPos.x, mousePos.y - entityPos.y };
	float length = sqrtf(diffVec.x * diffVec.x + diffVec.y * diffVec.y);
	sf::Vector2f normalized{ diffVec.x / length, diffVec.y / length };
	sf::Vector2f departure{ entityPos.x + entityRadius * normalized.x, entityPos.y + entityRadius * normalized.y };

	auto e = m_entities.addEntity("Bullet");
	e->cShape = std::make_shared<CShape>(bulletConfig.SR, bulletConfig.V, sf::Color(bulletConfig.FR, bulletConfig.FB, bulletConfig.FB),
		sf::Color(bulletConfig.OR, bulletConfig.OG, bulletConfig.OB), bulletConfig.OT);
	e->cTransform = std::make_shared<CTransform>(departure, sf::Vector2f{bulletConfig.S * normalized.x, bulletConfig.S*normalized.y});
	e->cLifespan = std::make_shared<CLifespan>(bulletConfig.L);
	e->cCollision = std::make_shared<CCollision>(bulletConfig.CR);
}

void GameEngine::update()
{
	m_entities.update();
}

void GameEngine::run()
{
	while (m_running) {
		if (!m_paused) {
			update();
			sEnemySpawner();
			sMovement();
			sLifespan(); 
		}
		sUserInput();
		sCollision();
		sRender();
		++m_currentFrame;
	}
}
