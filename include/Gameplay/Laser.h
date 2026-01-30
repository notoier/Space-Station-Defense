//
// Created by Oier Álvarez on 20/01/2026.
//

#ifndef SPACESTATIONDEFENSE_LASER_H
#define SPACESTATIONDEFENSE_LASER_H
#include "Weapon.h"
#include "SFML/Graphics/VertexArray.hpp"


class Laser final : public Weapon
{
public:
    struct LaserBaseStats
    {
        float range = 400.f;
        float width = 2.f;
        float damage = 1.f;
    };

    struct LaserUpgrades
    {
        float damageUpgrade = 0.f;
        float rangeUpgrade = 0.f;
    };

    explicit Laser(const LaserBaseStats& stats);
    void update(float dtMilliseconds) override;
    void render(sf::RenderWindow& window) override;

    void setStats(const LaserBaseStats& stats) { m_stats = stats; };
    void beamSetUp(sf::Vector2f start, sf::Vector2f end, sf::Vector2f dir);

    void shoot() override;
    float getDamage() override;
    void applyEffectToEnemies(ObjectPool<Enemy>& enemyPool) override;

    void setUpgrades(const LaserUpgrades& u);

    void setEnabled(bool enable);

    const LaserBaseStats& getStats() const { return m_stats; };
    const LaserUpgrades& getUpgrades() const { return m_upgrades; };

private:
    LaserBaseStats m_stats;
    LaserUpgrades m_upgrades;

    sf::VertexArray m_beam;
    bool m_hasHit = false;
    bool m_fired = false;
    bool m_OnCoolDown = false;
    float m_coolDown = 2.f;
    float m_coolTime = 0.f;
};


#endif //SPACESTATIONDEFENSE_LASER_H