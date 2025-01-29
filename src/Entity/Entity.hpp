#pragma once

#include <string>
#include <memory>
#include "../Components/Components.hpp"
#include "EntityManager.hpp"
class Entity {
public:
	size_t getID() const;
	const std::string& getTag() const;
	bool isAlive() const;
	void destory();
	
	std::shared_ptr<CTransform> cTransform;
	std::shared_ptr<CShape> cShape;
	std::shared_ptr<CCollision> cCollision;
	std::shared_ptr<CInput> cInput;
	std::shared_ptr<CScore> cScore;
	std::shared_ptr<CLifespan> cLifespan;

	friend class EntityManager;
private:
	Entity(size_t id, const std::string& tag = "Default", bool alive = true)
		:m_id{ id }, m_tag{ tag }, m_alive{ alive } {
	}
	size_t m_id;
	std::string m_tag;
	bool m_alive;
};