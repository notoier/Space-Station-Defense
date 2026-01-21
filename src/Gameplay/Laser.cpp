//
// Created by Oier Álvarez on 20/01/2026.
//

#include "Gameplay/Laser.h"

#include <cmath>
#include <iostream>

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "Utils/MathUtils.h"

class World;

static float length(const sf::Vector2f& vec)
{
    return std::sqrt((vec.x * vec.x) + (vec.y * vec.y));
}

static sf::Vector2f normalize(const sf::Vector2f& vec)
{
    const float len = length(vec);
    if (len < 0.0001f) return {0.f, 0.f};
    return vec / len;
}

Laser::Laser(const Stats& stats)
{
    m_stats = stats;
    m_enabled = false;
    m_beam = sf::VertexArray(sf::Quads, 4);
}

void Laser::update(float dtMilliseconds)
{
    if (!m_enabled) return;
    // Direction from station to mouse

    // Gameplay: apply DPS to first enemy hit along the beam (simple version)
    // Implement in World: raycast-like check (line vs enemy bounds) and return Enemy*
    // Enemy* hit = world.findFirstEnemyHit(ctx.originWorld, end);
    // if (hit) hit->applyDamage(m_stats.damagePerSecond * dtSeconds);

    // remove once you use it

}

void Laser::render(sf::RenderWindow& window)
{
    if (m_enabled)
    {
        window.draw(m_beam);
    }

    setEnabled(false);
}

void Laser::beamSetUp(const sf::Vector2f start, const sf::Vector2f end, const sf::Vector2f dir)
{
    const sf::Vector2f perp(-dir.y, dir.x);
    const float halfWidth = m_stats.width * 0.5f;
    const sf::Vector2f offset = perp * halfWidth;

    m_beam[0].position = start + offset;
    m_beam[3].position = start - offset;
    m_beam[1].position = end + offset;
    m_beam[2].position = end - offset;

    for (int i = 0; i < 4; ++i)
    {
        m_beam[i].color = sf::Color::Red;
    }

}

void Laser::shoot()
{
    setEnabled(true);
    const sf::Vector2f dir = normalize(m_context.aimWorld - m_context.originWorld);
    const sf::Vector2f end = m_context.originWorld + dir * m_stats.range;
    m_fired = true;

    // Visual
    beamSetUp(m_context.originWorld, end, dir);
}

float Laser::getDamage()
{
    return m_stats.damage;
}

void Laser::applyEffectToEnemies(ObjectPool<Enemy>& enemyPool)
{
    if (!m_enabled || !m_fired)
        return;

    m_fired = false;

    const sf::Vector2f start = m_context.originWorld;
    const sf::Vector2f delta = m_context.aimWorld - start;

    if (dot(delta, delta) < 0.000001f)
        return;

    const sf::Vector2f dir = normalize(delta);
    const sf::Vector2f end = start + dir * m_stats.range;

    const float halfWidth = m_stats.width * 0.5f;

    enemyPool.forEachActive([&](Enemy& e)
    {
        if (!e.isAlive())
            return;

        const float size = e.getSize();
        const sf::Vector2f center = e.getCenter();

        const auto radius = static_cast<float>(sqrt(2) * size/2);
        const float hitRadius = radius + halfWidth;

        if (SegmentCircleCollision(start, end, center, hitRadius))
        {
            e.receiveDamage(getDamage());
        }
    });

}


void Laser::setEnabled(const bool enable)
{
    m_enabled = enable;
}