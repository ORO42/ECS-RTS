#include <vector>

#include "include/EntityManager.h"

// EntityManager::EntityManager()
// {
// }

void EntityManager::update()
{
    // add entities from m_entitiesToAdd to the proper location(s)
    // add to vector of all entities
    // add to vector inside map, with the tag as a key
    // entities are dead when isActive == false

    for (auto e : m_entitiesToAdd)
    {
        m_entities.push_back(e);
        m_entityMap[e->m_tag].push_back(e);
    };

    m_entitiesToAdd.clear();

    // remove dead entities from the vector of all entities
    removeDeadEntities(m_entities);

    // remove dead entities from each vector in the entity map
    for (auto &[tag, EntityVec] : m_entityMap)
    {
        removeDeadEntities(EntityVec);
    }
}

void EntityManager::removeDeadEntities(EntityVec &vec)
{
    // Use std::remove_if to remove elements based on the condition
    vec.erase(std::remove_if(vec.begin(), vec.end(), [](const std::shared_ptr<Entity> &e)
                             {
                                 return !e->isActive(); // Return true for entities that are not active
                             }),
              vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag)
{
    auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    return entity;
}

const EntityVec &EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec &EntityManager::getEntities(const std::string &tag)
{
    return m_entityMap[tag];
}

const std::map<std::string, EntityVec> &EntityManager::getEntityMap()
{
    return m_entityMap;
}