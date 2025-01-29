#pragma once
#include <SFML/Graphics.hpp>
class CTransform {
public:
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v, const sf::Vector2f& s = { 1.f,1.f }, double a=0.f)
		:pos{ p }, velocity{ v } {
	}
public:
	sf::Vector2f pos;
	sf::Vector2f velocity;
	sf::Vector2f scale;
	double angle;
};

class CShape {
public:
public:
	sf::CircleShape shape;
};

class CCollision {
public:
	CCollision(float radius) : radius{ radius } {}
public:
	float radius;
};

class CScore {
public:
	CScore(int s) : score{ s } {}
public:
	int score;
};

class CLifespan {
public:
	CLifespan(float lifespan)
		:lifespan{ lifespan } {
	}
public:
	float lifespan;
};

class CInput {
public:
public:
	int left;
	int right;
	int up;
	int down;
};