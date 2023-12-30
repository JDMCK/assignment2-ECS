#pragma once

#include "Entity.h"
#include <vector>
#include <map>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<Tag, EntityVec>             EntityMap;

class EntityManager
{
    EntityVec m_entities;
    EntityMap m_entityMap;
    EntityVec m_toAdd;
    size_t    m_totalEntities = 0;

public:
    void update();
    std::shared_ptr<Entity> addEntity(const Tag tag);
    void removeEntity(const size_t id);
    void removeEntitiesByTag(const Tag tag);
    EntityVec& getEntities();
    EntityVec& getEntitiesByTag(const Tag tag);    
};