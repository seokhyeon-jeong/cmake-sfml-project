#include "Entity.hpp"

size_t Entity::getID() const
{
	return m_id;
}

const std::string& Entity::getTag() const
{
	return m_tag;
}

bool Entity::isAlive() const
{
	return m_alive;
}

void Entity::destory()
{
	m_alive = false;
}
