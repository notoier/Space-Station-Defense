#ifndef SPACESTATIONDEFENSE_CANNON_H
#define SPACESTATIONDEFENSE_CANNON_H

#include "Weapon.h"
#include <SFML/System/Vector2.hpp>

class Cannon : public Weapon
{
public:
    struct CannonDesc
    {
        float damage = 0.5f;

        float orbitRadius = 75.f;
        float orbitSpeedDegPerSec = 90.f;

        float projectileSpeed = 200.f;
        float projectileRadius = 3.f;
        float projectileLifeSec = 3.f;

        float targetMaxDistance = 10000.f;    // max distance from station to target
        float retargetIntervalSec = 3.f;    // every 3 seconds pick closest
        float fireCooldownSec = 1.f;
    };

    struct CannonUpgrades
    {
        float damageUpgrade = 0.f;
        float cooldownReduction = 0.f;
    };

    Cannon() = default;
    explicit Cannon(const CannonDesc& desc);

    void update(float dtSeconds) override;
    void render(sf::RenderWindow& window) override;
    void shoot() override;

    float getDamage() override;
    void applyEffectToEnemies(ObjectPool<Enemy>& enemyPool) override;

    const CannonUpgrades& getUpgrades() const { return m_upgrades; };
    void setUpgrades(const CannonUpgrades& u);
private:
    sf::Vector2f computeOrbitPosition() const;

    Enemy* findClosestEnemyToStation(ObjectPool<Enemy>& enemyPool) const;

private:
    CannonDesc m_desc{};
    CannonUpgrades m_upgrades{};

    float m_orbitAngleDeg = 0.f;
    float m_retargetTimerSec = 0.f;
    float m_fireTimerSec = 0.f;

    Enemy* m_target = nullptr;
    uint32_t m_targetUID = 0;

    sf::Vector2f m_cannonPosWorld {0.f, 0.f};
    sf::Vector2f m_aimWorld {0.f, 0.f};
};

#endif
