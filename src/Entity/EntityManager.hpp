#pragma once
#include <map>
#include <vector>
#include <string>
#include <memory>
#include "Entity.hpp"
class Entity;
using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
public:
	EntityManager() : m_totalEntities{ 0ULL } {}

	std::shared_ptr<Entity> addEntity(const std::string& tag);
	EntityVec& getEntities(void);
	EntityVec& getEntities(const std::string& tag);

	void update();
private:
	void init();
	size_t m_totalEntities;
	EntityVec m_entities;
	EntityMap m_entityMap;
	EntityVec m_toAdd;
};