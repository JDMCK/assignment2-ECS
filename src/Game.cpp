#include <fstream>
#include <cstdlib>
#include <ctime>
#include "Game.h"

Game::Game(const std::string& config)
{
    init(config);
}

void Game::run()
{
    while (m_window.isOpen())
    {
        m_entities.update();
        m_window.clear();
        if (!m_paused)
        {
            sEnemySpawner();
            sLifespan();
            sPlayerInput();
            sMovement();
            sParentChild();
            sCollision();
        }
        sAnimate();
        sUserInput();
        sRender();
        m_window.display();
        m_enemyCoolDown++;
        m_bulletCoolDown++;
    }
}

void Game::init(const std::string& path)
{
    //std::ifstream fin(path);

    m_window.create(sf::VideoMode(1280, 720), "Geometry Wars");
    m_window.setFramerateLimit(60);

    m_windowWidth = 1280;
    m_windowHeight = 720;

    m_font.loadFromFile("fonts/minecraft.ttf");
    m_scoreText.setFont(m_font);
    m_livesText.setFont(m_font);
    m_scoreText.setCharacterSize(20);
    m_livesText.setCharacterSize(20);
    m_scoreText.setFillColor(sf::Color::White);
    m_livesText.setFillColor(sf::Color::Red);
    m_scoreText.setPosition(10, 12);
    m_livesText.setPosition(m_windowWidth - 108, 12);

    spawnPlayer();
}

void Game::setPaused(bool paused)
{
    m_paused = paused;
}

void Game::sMovement()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->cTransform)
        {
            e->cTransform->position += e->cTransform->velocity;
            e->cTransform->rotation += 2;
        }
    }
}

void Game::sAnimate()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->cTransform && e->cShape)
        {
            e->cShape->shape.setPosition(
                e->cTransform->position.x,
                e->cTransform->position.y
            );
            e->cShape->shape.setRotation(e->cTransform->rotation);

            if (e->cLifespan)
            {
                e->cShape->shape.setFillColor(sf::Color(
                    e->cShape->shape.getFillColor().r,
                    e->cShape->shape.getFillColor().g,
                    e->cShape->shape.getFillColor().b,
                    255 * (e->cLifespan->remaining / e->cLifespan->total)
                ));
                e->cShape->shape.setOutlineColor(sf::Color(
                    e->cShape->shape.getOutlineColor().r,
                    e->cShape->shape.getOutlineColor().g,
                    e->cShape->shape.getOutlineColor().b,
                    255 * (e->cLifespan->remaining / e->cLifespan->total)
                ));
            }
        }
    }
}

void Game::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                m_window.close();
                break;
            
            case sf::Event::KeyPressed:
                for (auto& e : m_entities.getEntities())
                {
                    if (e->cInput)
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::W:
                                e->cInput->up = true;
                                break;
                            case sf::Keyboard::A:
                                e->cInput->left = true;
                                break;
                            case sf::Keyboard::S:
                                e->cInput->down = true;
                                break;
                            case sf::Keyboard::D:
                                e->cInput->right = true;
                                break;
                            case sf::Keyboard::Space:
                                e->cInput->dash = true;
                                break;
                            case sf::Keyboard::Escape:
                                setPaused(!m_paused);
                            default:
                                break;
                        }
                    }
                }
                break;

            case sf::Event::KeyReleased:
                for (auto& e : m_entities.getEntities())
                {
                    if (e->cInput)
                    {
                        switch (event.key.code)
                        {
                            case sf::Keyboard::W:
                                e->cInput->up = false;
                                break;
                            case sf::Keyboard::A:
                                e->cInput->left = false;
                                break;
                            case sf::Keyboard::S:
                                e->cInput->down = false;
                                break;
                            case sf::Keyboard::D:
                                e->cInput->right = false;
                                break;
                            case sf::Keyboard::Space:
                                e->cInput->dash = false;
                                break;
                            default:
                                break;
                        }
                    }
                }
                break;

            default:
                break;
        }
    }
    // Why isn't this working in the event loop??
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        for (auto& e : m_entities.getEntities())
        {
            if (e->cInput)
            {
                e->cInput->shoot = true;
            }
        }
    } else
    {
        for (auto& e : m_entities.getEntities())
        {
            if (e->cInput)
            {
                e->cInput->shoot = false;
            }
        }
    }
}

void Game::sPlayerInput()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->cInput && e->cTransform)
        {
            Vec2 vel = Vec2::ZERO;
            if (e->cInput->up) vel += Vec2::UP;
            if (e->cInput->down) vel += Vec2::DOWN;
            if (e->cInput->left) vel += Vec2::LEFT;
            if (e->cInput->right) vel += Vec2::RIGHT;
            vel.normalize().scale(7);

            e->cTransform->velocity = vel;

            Vec2 mousePos = Vec2(sf::Mouse::getPosition(m_window).x,
                                 sf::Mouse::getPosition(m_window).y);
            if (e->cInput->shoot && m_bulletCoolDown >= 15) 
            {
                spawnBullet(e, mousePos);
                m_bulletCoolDown = 0;
            }
            spawnSpecialWeapon(e);
        }
    }
}

void Game::sLifespan()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->cLifespan)
        {
            e->cLifespan->remaining--;
            if (e->cLifespan->remaining <= 0) e->destroy();
        }
    }
}

void Game::sRender()
{
    for (auto& e : m_entities.getEntities())
    {
        m_window.draw(e->cShape->shape);
    }
    m_scoreText.setString("Score: " + std::to_string(m_score));
    m_livesText.setString("Lives: " + std::to_string(m_player->cLives->lives));
    m_window.draw(m_scoreText);
    m_window.draw(m_livesText);
}

void Game::sEnemySpawner()
{
    if (m_enemyCoolDown >= 120)
    {
        spawnEnemy();
        m_enemyCoolDown = 0;
    }
}

void Game::sCollision()
{
    sBulletCollision();
    sPlayerCollision();
    sEnemyCollision();
}

void Game::sBulletCollision()
{
    for (auto& b : m_entities.getEntitiesByTag(BULLET))
    {
        for (auto& e : m_entities.getEntitiesByTag(ENEMY))
        {
            if (b->cCircleCollider && b->cTransform && e->cCircleCollider && e->cTransform)
            {
                if (b->cTransform->position.dist(e->cTransform->position) <
                    b->cCircleCollider->radius + e->cCircleCollider->radius)
                    {
                        b->destroy();
                        spawnSmallEnemies(e);
                        m_score += e->cShape->sideCount;
                        e->destroy();
                    }
            }
        }
        for (auto& e : m_entities.getEntitiesByTag(SMALLENEMY))
        {
            if (b->cCircleCollider && b->cTransform && e->cCircleCollider && e->cTransform)
            {
                if (b->cTransform->position.dist(e->cTransform->position) <
                    b->cCircleCollider->radius + e->cCircleCollider->radius)
                    {
                        b->destroy();
                        m_score += e->cShape->sideCount / 2;
                        e->destroy();
                    }
            }
        }
    }
}

void Game::sPlayerCollision()
{
    // Collisions between Player and Enemies
    float width = m_window.getSize().x;
    float height = m_window.getSize().y;
    for (auto& p : m_entities.getEntitiesByTag(PLAYER))
    {
        if (p->cCircleCollider && p->cTransform && p->cInvulnerable)
        {
            for (auto& e : m_entities.getEntitiesByTag(ENEMY))
            {
                if (e->cCircleCollider && e->cTransform)
                {
                    if (p->cTransform->position.dist(e->cTransform->position) <
                        p->cCircleCollider->radius + e->cCircleCollider->radius)
                        {
                            if (!p->cInvulnerable->isInvulnerable)
                            {
                                respawnPlayer();
                            }
                            spawnSmallEnemies(e);
                            m_score += e->cShape->sideCount;
                            e->destroy();
                        }
                }
            }
            for (auto& e : m_entities.getEntitiesByTag(SMALLENEMY))
            {
                if (e->cCircleCollider && e->cTransform)
                {
                    if (p->cTransform->position.dist(e->cTransform->position) <
                        p->cCircleCollider->radius + e->cCircleCollider->radius)
                        {
                            if (!p->cInvulnerable->isInvulnerable)
                            {
                                respawnPlayer();
                            }
                            m_score += e->cShape->sideCount / 2;
                            e->destroy();
                        }
                }
            }

            // Wall collisions
            if (p->cTransform->position.x + p->cCircleCollider->radius > width)
                p->cTransform->position.x = width - p->cCircleCollider->radius;
            if (p->cTransform->position.y + p->cCircleCollider->radius > height)
                p->cTransform->position.y = height - p->cCircleCollider->radius;
            if (p->cTransform->position.x - p->cCircleCollider->radius < 0)
                p->cTransform->position.x = p->cCircleCollider->radius;
            if (p->cTransform->position.y - p->cCircleCollider->radius < 0)
                p->cTransform->position.y = p->cCircleCollider->radius;
        }
    }
}

void Game::respawnPlayer()
{
    m_player->cTransform->position = Vec2(m_windowWidth / 2, m_windowHeight / 2);
    m_player->cLives->lives--;
    if (m_player->cLives->lives <= 0) {
        m_paused = true;
    }
}

void Game::sEnemyCollision()
{
    for (auto& e : m_entities.getEntitiesByTag(ENEMY))
    {
        if (e->cTransform && e->cCircleCollider)
        {
            if (e->cTransform->position.x + e->cCircleCollider->radius > m_windowWidth
                || e->cTransform->position.x - e->cCircleCollider->radius < 0)
                e->cTransform->velocity.flipX();
            if (e->cTransform->position.y + e->cCircleCollider->radius > m_windowHeight
                || e->cTransform->position.y - e->cCircleCollider->radius < 0)
                e->cTransform->velocity.flipY();
        }
    }
}

void Game::sParentChild()
{
    for (auto& e : m_entities.getEntities())
    {
        if (e->cParentChild && e->cTransform && e->cParentChild->parent)
        {
            e->cTransform->position = e->cParentChild->parent->position;
        }
    }
}

void Game::spawnPlayer()
{
    m_player = m_entities.addEntity(PLAYER);

    float mx = m_windowWidth / 2;
    float my = m_windowHeight / 2;

    m_player->cTransform = std::make_shared<CTransform>(Vec2(mx, my));
    m_player->cShape = std::make_shared<CShape>(
        30, 8, sf::Color(0x000000FF), sf::Color(0xFF0000FF), 5.0
    );
    m_player->cShape->shape.setPosition(
        mx, my
    );
    m_player->cShape->shape.setRotation(
        m_player->cTransform->rotation
    );
    m_player->cInput = std::make_shared<CInput>();
    m_player->cInvulnerable = std::make_shared<CInvulnerable>();
    m_player->cDashAbility = std::make_shared<CDashAbility>(10, 120);
    m_player->cCircleCollider = std::make_shared<CCircleCollider>(50);
    m_player->cLives = std::make_shared<CLives>(3);
}

void Game::spawnEnemy()
{
    srand(time(NULL));

    int radius = 30;

    auto enemy = m_entities.addEntity(ENEMY);
    enemy->cCircleCollider = std::make_shared<CCircleCollider>(radius);
    enemy->cShape = std::make_shared<CShape>(
        radius, (rand() % 10) + 3, sf::Color(rand() % 255, rand() % 255, rand() % 255, 255u),
        sf::Color::White, 5);

    float x = rand() % ((int) m_windowWidth - radius * 2);
    float y = rand() % ((int) m_windowHeight - radius * 2);
    enemy->cTransform = std::make_shared<CTransform>(
        Vec2(x, y),
        Vec2((rand() % 5) - 2.5, (rand() % 5) - 2.5));
}

void Game::spawnSmallEnemies(std::shared_ptr<Entity> entity)
{
    float radius = 30;
    int angle = 0;
    int sideCount = entity->cShape->sideCount;
    float arcLength = 360 / sideCount;
    entity->cShape->shape.setScale(0.5, 0.5);
    for (int i = 0; i < sideCount; i++)
    {
        Vec2 vel = Vec2::RIGHT;
        auto enemy = m_entities.addEntity(SMALLENEMY);
        enemy->cCircleCollider = std::make_shared<CCircleCollider>(radius/2);
        enemy->cShape = std::make_shared<CShape>(*(entity->cShape));
        enemy->cLifespan = std::make_shared<CLifespan>(60);
        enemy->cTransform = std::make_shared<CTransform>(
            entity->cTransform->position,
            vel.rotate(angle)
        );
        angle += arcLength;
    }
}

void Game::spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos)
{
    auto bullet = m_entities.addEntity(BULLET);

    bullet->cTransform = std::make_shared<CTransform>(
        entity->cTransform->position,
        (mousePos - entity->cTransform->position).normalize().scale(10));
    bullet->cCircleCollider = std::make_shared<CCircleCollider>(5);
    bullet->cLifespan = std::make_shared<CLifespan>(360);
    bullet->cShape = std::make_shared<CShape>(5, 32,
        sf::Color::White, sf::Color::Yellow, 2);
}

void Game::spawnSpecialWeapon(std::shared_ptr<Entity> entity)
{
    if (entity->cTransform && entity->cInvulnerable && entity->cDashAbility && entity->cInput)
    {
        if (entity->cDashAbility->remainingCoolDown <= 0 && entity->cInput->dash)
        {
            entity->cInput->isLocked = true;
            entity->cDashAbility->remainingCoolDown = entity->cDashAbility->dashCoolDown;
            entity->cDashAbility->remainingDash = entity->cDashAbility->dashDuration;
        }
        else if (entity->cDashAbility->remainingCoolDown == 0)
        {
            auto sparkle = m_entities.addEntity(DEFAULT);
            sparkle->cTransform = std::make_shared<CTransform>(*(entity->cTransform));
            sparkle->cLifespan = std::make_shared<CLifespan>(45);
            sparkle->cShape = std::make_shared<CShape>(
                10, 3, sf::Color::White, sf::Color::White, 2
            );
            sparkle->cParentChild = std::make_shared<CParentChild>(m_player->cTransform);
        }
        else if (entity->cDashAbility->remainingDash > 0)
        {
            entity->cInvulnerable->isInvulnerable = true;
            entity->cTransform->velocity.scale(5);
        }
        else if (entity->cDashAbility->remainingDash <= 0)
        {
            entity->cInput->isLocked = false;
            entity->cInvulnerable->isInvulnerable = false;
        }
        entity->cDashAbility->remainingCoolDown--;
        entity->cDashAbility->remainingDash--;
    }
}