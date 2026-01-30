//
// Created by Oier Álvarez on 19/01/2026.
//

#include "Gameplay/Enemy.h"
#include <cmath>
#include <iostream>

#include "Core/World.h"
#include "Utils/MathUtils.h"

bool Enemy::init(const EnemyDescriptor& descriptor)
{
    m_velocity = descriptor.velocity;
    m_size = descriptor.size;
    m_damage = descriptor.damage;

    m_isAlive = true;

    return Entity::init(descriptor);
}

void Enemy::update(float dt)
{
    const float dtSeconds = dt / 1000.f;

    sf::Vector2f direction = m_targetPosition - m_position;

    const float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (length > 0.001f)
    {
        direction /= length;
        m_position += direction * m_velocity.x * dtSeconds;

        m_visual.setPosition(m_position);
    }

    Entity::update(dt);
}

void Enemy::setTarget(const sf::Vector2f& target, const float targetRadius)
{
    m_targetPosition = target;
    m_targetRadius = targetRadius;
}

float Enemy::getDamage()
{
    return m_damage;
}

void Enemy::receiveDamage(const float damage)
{
    Entity::receiveDamage(damage);
    if (!m_isAlive && m_onDeath) m_onDeath();
}

bool Enemy::isTargetReached() const
{
    return BoxCircumferenceCollision(m_position, m_size, m_targetPosition, m_targetRadius);
}

sf::FloatRect Enemy::getBounds() const
{
    return {
        m_position.x - m_size * 0.5f,
        m_position.y - m_size * 0.5f,
        m_size,
        m_size
        };
}

void Enemy::setOnDeathFunction(const std::function<void()>& func)
{
    m_onDeath = func;
}
