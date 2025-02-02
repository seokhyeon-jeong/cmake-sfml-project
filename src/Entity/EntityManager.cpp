#include "EntityManager.hpp"

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
	auto e = std::shared_ptr<Entity>(new Entity(m_totalEntities, tag));
	++m_totalEntities;
	m_toAdd.push_back(e);
	return e;
}

EntityVec& EntityManager::getEntities(void)
{
	return m_entities;
}

EntityVec& EntityManager::getEntities(const std::string& tag)
{
	return m_entityMap[tag];
}

void EntityManager::update()
{
	for (auto& e : m_toAdd) {
		m_entities.push_back(e);
		m_entityMap[e->getTag()].push_back(e);
	}
	m_toAdd.clear();

	m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [](std::shared_ptr<Entity> e)->bool {
		return !e->isAlive();
		}), m_entities.end());

	for (auto& [tag, vec] : m_entityMap) {
		vec.erase(std::remove_if(vec.begin(), vec.end(), [](std::shared_ptr<Entity> e)->bool {
			return !e->isAlive();
			}), vec.end());
	}
}

void EntityManager::init()
{
}
