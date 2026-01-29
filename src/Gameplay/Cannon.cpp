#include "Gameplay/Cannon.h"

#include <iostream>

#include "Gameplay/Enemy.h"
#include "Core/ObjectPool.h"
#include "Utils/MathUtils.h"

#include <limits>

#include "SFML/Graphics/RectangleShape.hpp"

Cannon::Cannon(const CannonDesc& desc)
{
    m_desc = desc;
    m_retargetTimerSec = 0.f;
    m_fireTimerSec = 0.f;
    m_target = nullptr;


    /* Shape */
    constexpr float size = 25.f;
    std::unique_ptr<sf::Shape> r = std::make_unique<sf::RectangleShape>(sf::Vector2f(size, size));
    r->setOrigin({size * 0.5, size * 0.5});
    r->setFillColor(sf::Color::Transparent);
    r->setOutlineThickness(2.f);
    r->setOutlineColor(sf::Color::White);
    m_shape.addPart(std::move(r), size, {0,-size * 0.5});
}

sf::Vector2f Cannon::computeOrbitPosition() const
{
    const sf::Vector2f offset = rotateDeg({m_desc.orbitRadius, 0.f}, m_orbitAngleDeg);
    return m_context.originWorld + offset;
}

void Cannon::update(float dtSeconds)
{
    if (!m_enabled) return;

    m_retargetTimerSec = std::max(0.f, m_retargetTimerSec - dtSeconds);
    m_fireTimerSec     = std::max(0.f, m_fireTimerSec - dtSeconds);

    if (m_target && m_target->isAlive())
    {
        const sf::Vector2f enemyPos = m_target->getCenter();

        const sf::Vector2f toEnemy = enemyPos - m_context.originWorld;
        if (lengthSq(toEnemy) > 0.000001f)
        {
            float desired = angleDegFromDirection(toEnemy);

            // Compute the offset for that angle
            const sf::Vector2f offset = rotateDeg({m_desc.orbitRadius, 0.f}, desired);

            // If offset points away from enemy direction, flip 180 degrees
            if (dot(offset, toEnemy) < 0.f)
                desired += 180.f;

            m_orbitAngleDeg = moveTowardsAngleDeg(
                m_orbitAngleDeg, desired,
                m_desc.orbitSpeedDegPerSec * dtSeconds
            );

            // Visual rotation offset
            m_shape.setRotation(m_orbitAngleDeg - 90.f);
        }
    }

    m_cannonPosWorld = computeOrbitPosition();
    m_shape.setPosition(m_cannonPosWorld);
}

void Cannon::applyEffectToEnemies(ObjectPool<Enemy>& enemyPool)
{
    if (!m_enabled) return;

    // Always drop dead target (laser kill)
    if (m_target && !m_target->isAlive())
        m_target = nullptr;

    if (m_retargetTimerSec <= 0.f || !m_target)
    {
        m_target = findClosestEnemyToStation(enemyPool);
        m_targetUID = (m_target ? m_target->getUID() : 0);
        m_retargetTimerSec = m_desc.retargetIntervalSec;
    }

    shoot();
}

Enemy* Cannon::findClosestEnemyToStation(ObjectPool<Enemy>& enemyPool) const
{
    Enemy* best = nullptr;

    const float maxDist = m_desc.targetMaxDistance;
    const float maxDistSq = maxDist * maxDist;

    float bestSq = std::numeric_limits<float>::max();

    enemyPool.forEachActive([&](Enemy& e)
    {
        if (!e.isAlive())
            return;

        const sf::Vector2f d = e.getPosition() - m_context.originWorld;
        const float distSq = lengthSq(d);

        if (distSq > maxDistSq)
            return;

        if (distSq < bestSq)
        {
            bestSq = distSq;
            best = &e;
        }
    });

    return best;
}

void Cannon::shoot()
{
    if (!m_enabled || !m_spawnProjectile || !m_target || m_fireTimerSec >  0.f)
        return;

    if (!m_target->isAlive())
    {
        m_target = nullptr;
        return;
    }

    m_aimWorld = m_target->getPosition();

    const sf::Vector2f dir = normalize(m_aimWorld - m_cannonPosWorld);
    if (lengthSq(dir) < 0.000001f)
        return;

    m_fireTimerSec = m_desc.fireCooldownSec - m_upgrades.cooldownReduction;

    Projectile::ProjectileDescriptor pd;
    pd.position = m_cannonPosWorld;
    pd.velocity = dir * m_desc.projectileSpeed;
    pd.radius = m_desc.projectileRadius;
    pd.damage = m_desc.damage;
    pd.maxLifeSeconds = m_desc.projectileLifeSec;

    m_spawnProjectile(pd);
}

float Cannon::getDamage()
{
    return m_desc.damage + m_upgrades.damageUpgrade;
}

void Cannon::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
}
