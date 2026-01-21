//
// Created by Oier Álvarez on 18/01/2026.
//

#include "Gameplay/Entity.h"

#include <iostream>
#include <ostream>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

bool Entity::init(const EntityDescriptor& descriptor)
{
    // Initialize base GameObject data
    m_position = descriptor.position;
    m_health   = descriptor.health;

    // Build visual representation based on descriptor
    buildVisual(descriptor.visualType);
    m_visual.setPosition(m_position);

    return true;
}

void Entity::update(const float deltaMilliseconds)
{
    const float dtSeconds = deltaMilliseconds / 1000.f;

    // Update visual (rotations, animations, etc.)
    m_visual.update(dtSeconds);
}

void Entity::render(sf::RenderWindow& window)
{
    window.draw(m_visual);
}

void Entity::receiveDamage(const float damage)
{
    m_health -= damage;
    std::cout << "Health: " << m_health << std::endl;
    if (m_health <= 0)
    {
        m_isAlive = false;
    }
}

float Entity::getSize() const
{
    return m_size;
}

sf::Vector2f Entity::getCenter() const
{
    return { m_position.x + m_size * 0.5f, m_position.y + m_size * 0.5f };
}

bool Entity::isAlive() const
{
    return m_isAlive;
}

void Entity::buildVisual(VisualType type)
{
    m_visual = CompositeShape(); // Reset previous visual

    switch (type)
    {
        case VisualType::StationCircle:
        {
            constexpr float radius = 50.f;
            auto circle = std::make_unique<sf::CircleShape>(radius);
            circle->setOrigin(32.f, 32.f);
            circle->setFillColor(sf::Color::Transparent);
            circle->setOutlineThickness(2.f);
            circle->setOutlineColor(sf::Color::White);

            m_visual.addPart(std::move(circle), radius);
            break;
        }

        case VisualType::BasicEnemySquares:
        {
            auto makeSquare = [](const float size)
            {
                auto r = std::make_unique<sf::RectangleShape>(sf::Vector2f(size, size));
                r->setOrigin(size * 0.5f, size * 0.5f);

                r->setFillColor(sf::Color::Transparent);
                r->setOutlineThickness(2.f);
                r->setOutlineColor(sf::Color::White);

                return r;
            };

            // Square 1
            {
                constexpr float size = 22.f;
                m_size = size;
                auto& p = m_visual.addPart(makeSquare(size), size);
                p.orbitRadius = 0.f;               // stays in center
                p.orbitSpeedDegPerSec = 0.f;
                p.spinSpeedDegPerSec = 220.f;      // spins fast
            }

            // Square 2 (orbits)
            {
                constexpr float size = 16.f;
                auto& p = m_visual.addPart(makeSquare(size), size);
                p.orbitRadius = 14.f;
                p.orbitPhaseDeg = 0.f;
                p.orbitSpeedDegPerSec = 140.f;     // orbit speed
                p.spinSpeedDegPerSec = -260.f;     // spin opposite
            }

            // Square 3 (orbits differently)
            {
                constexpr float size = 12.f;
                auto& p = m_visual.addPart(makeSquare(size), size);
                p.orbitRadius = 20.f;
                p.orbitPhaseDeg = 120.f;           // different axis/phase
                p.orbitSpeedDegPerSec = -90.f;     // opposite orbit direction
                p.spinSpeedDegPerSec = 320.f;
            }

            break;
        }
    }
}
