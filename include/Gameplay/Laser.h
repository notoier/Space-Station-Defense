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
    struct Stats
    {
        float range = 400.f;
        float width = 2.f;
        float damage = 1.f;
    };

    explicit Laser(const Stats& stats);
    void update(float dtMilliseconds) override;
    void render(sf::RenderWindow& window) override;

    void setStats(const Stats& stats) { m_stats = stats; };
    void beamSetUp(sf::Vector2f start, sf::Vector2f end, sf::Vector2f dir);

    void shoot() override;
    float getDamage() override;
    void applyEffectToEnemies(ObjectPool<Enemy>& enemyPool);

    void setEnabled(bool enable);

    const Stats& getStats() const { return m_stats; };

private:
    Stats m_stats;
    sf::VertexArray m_beam;
    bool m_hasHit = false;
    bool m_fired = false;
};


#endif //SPACESTATIONDEFENSE_LASER_H