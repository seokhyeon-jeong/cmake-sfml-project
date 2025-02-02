#pragma once
#include <SFML/Graphics.hpp>
class CTransform {
public:
	CTransform(const sf::Vector2f& p, const sf::Vector2f& v, float a=0.01f)
		:pos{ p }, velocity{ v }, angle{ sf::degrees(a) } {
	}
public:
	sf::Vector2f pos;
	sf::Vector2f velocity;
	sf::Angle angle;
};

class CShape {
public:
	CShape(float radius, size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
		:circle{ radius, points } {
		circle.setFillColor(fill);
		circle.setOutlineColor(outline);
		circle.setOutlineThickness(thickness);
		circle.setOrigin({ radius, radius });
	}
public:
	sf::CircleShape circle;
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
	CLifespan(int total)
		:remaining{ total }, total{ total } {
	}
public:
	int remaining;
	int total;
};

class CInput {
public:
	CInput()
		:up{ false }, down{ false }, left{ false }, right{ false }, shoot{ false } {
	}
public:
	bool up, down, left, right;
	bool shoot;
};