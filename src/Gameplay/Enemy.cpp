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

bool Enemy::isTargetReached() const
{
    return BoxCircumferenceCollision(m_position, m_size, m_targetPosition, m_targetRadius);
}
