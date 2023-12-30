#pragma once

#include "Components.h"
#include <memory>

enum Tag
{
    PLAYER,
    ENEMY,
    BULLET,
    SMALLENEMY,
    GAMEMANAGER,
    DEFAULT
};

class Entity
{
    const size_t m_id;
    const Tag    m_tag   = DEFAULT;
    bool         m_alive = true;

    friend class EntityManager;

    Entity(const Tag tag, size_t id) :
        m_tag(tag),
        m_id(id)
    {}

public:

    std::shared_ptr<CTransform>      cTransform;
    std::shared_ptr<CShape>          cShape;
    std::shared_ptr<CInput>          cInput;
    std::shared_ptr<CScore>          cScore;
    std::shared_ptr<CLifespan>       cLifespan;
    std::shared_ptr<CCircleCollider> cCircleCollider;
    std::shared_ptr<CInvulnerable>   cInvulnerable;
    std::shared_ptr<CDashAbility>    cDashAbility;
    std::shared_ptr<CParentChild>    cParentChild;
    std::shared_ptr<CLives>          cLives;

    const size_t id() const;
    const Tag tag() const;
    void destroy();
    bool isAlive() const;
};