#pragma once

#include <SFML/Graphics.hpp>
#include "Vec2.h"

class CTransform
{
public:

    Vec2  position = Vec2::ZERO;
    Vec2  velocity = Vec2::ZERO;
    float rotation = 0.0f;

    CTransform() {};
    CTransform(Vec2 position, Vec2 velocity, float rotation) :
        position(position),
        velocity(velocity),
        rotation(rotation)
    {}
    CTransform(Vec2 position, Vec2 velocity) :
        position(position),
        velocity(velocity)
    {}
    CTransform(Vec2 position) :
        position(position)
    {}
};

class CShape
{
public:

    sf::CircleShape shape;
    int             sideCount;

    CShape(float radius, int sideCount, const sf::Color& fill,
           const sf::Color& outline, float outlineThickness) :
        shape(radius, sideCount),
        sideCount(sideCount)
    {
        shape.setFillColor(fill);
        shape.setOutlineColor(outline);
        shape.setOutlineThickness(outlineThickness);
        shape.setOrigin(radius, radius);
    }
};

class CInput
{
public:

    bool isLocked = false;
    bool up       = false;
    bool down     = false;
    bool left     = false;
    bool right    = false;
    bool shoot    = false;
    bool dash     = false;
};

class CScore
{
public:

    int score = 0;

    CScore(int score) :
        score(score)
    {}
};

class CLifespan
{
public:

    float total = 0.0f;
    float remaining = 0.0f;

    CLifespan(float total) :
        total(total),
        remaining(total)
    {}
};

class CCircleCollider
{
public:

    float radius = 0.0f;

    CCircleCollider(float radius) :
        radius(radius)
    {}
};

class CInvulnerable
{
public:
    
    bool isInvulnerable = false;

    CInvulnerable() {}
};

class CDashAbility
{
public:

    int dashDuration = 0;
    int dashCoolDown = 0;
    int remainingCoolDown = 0;
    int remainingDash = 0;

    CDashAbility(int dashDuration, int dashCoolDown) :
        dashDuration(dashDuration),
        dashCoolDown(dashCoolDown),
        remainingCoolDown(dashCoolDown)
    {}
};

class CParentChild
{
public:

    std::shared_ptr<CTransform> parent;

    CParentChild(std::shared_ptr<CTransform> parent) :
        parent(parent)
    {}
};

class CLives
{
public:

    int lives;

    CLives (int lives) :
        lives(lives)
    {}
};