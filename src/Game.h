#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include "EntityManager.h"
#include "Components.h"

struct PlayerConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V; float S; };
struct EnemyConfig  { int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI; float SMIN; };
struct BulletConfig { int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L; float S; };

class Game
{
    sf::RenderWindow m_window; 
    EntityManager    m_entities;
    sf::Font         m_font;
    sf::Text         m_scoreText;
    sf::Text         m_livesText;
    PlayerConfig     m_playerConfig;
    EnemyConfig      m_enemyConfig;
    BulletConfig     m_bulletConfig;
    int              m_score = 0;
    int              m_bulletCoolDown = 0;
    int              m_enemyCoolDown = 0;
    int              m_windowWidth;
    int              m_windowHeight;
    bool             m_paused = false;
    bool             m_running = true;

    std::shared_ptr<Entity> m_player;

    void init(const std::string& config);
    void setPaused(bool paused);

    void sMovement();
    void sAnimate();
    void sUserInput();
    void sPlayerInput();
    void sLifespan();
    void sRender();
    void sEnemySpawner();
    void sCollision();
    void sBulletCollision();
    void sPlayerCollision();
    void sEnemyCollision();
    void sParentChild();

    void respawnPlayer();
    void spawnPlayer();
    void spawnEnemy();
    void spawnSmallEnemies(std::shared_ptr<Entity> entity);
    void spawnBullet(std::shared_ptr<Entity> entity, const Vec2& mousePos);
    void spawnSpecialWeapon(std::shared_ptr<Entity> entity);

public:

    Game(const std::string& config);

    void run();
};
