//
// Created by Oier Álvarez on 27/01/2026.
//

#include "Gameplay/Projectile.h"

bool Projectile::init(const ProjectileDescriptor& d)
{
    m_alive = true;
    m_damage = d.damage;
    m_velocity = d.velocity;
    m_life = d.maxLifeSeconds;

    m_shape.setRadius(d.radius);
    m_shape.setOrigin(d.radius, d.radius);
    m_shape.setPosition(d.position);

    return true;
}

void Projectile::update(float dtSeconds)
{
    if (!m_alive)
        return;

    m_life -= dtSeconds;
    if (m_life <= 0.f)
    {
        m_alive = false;
        return;
    }

    m_shape.move(m_velocity * dtSeconds);
}

void Projectile::render(sf::RenderWindow& window)
{
    if (!m_alive)
        return;

    window.draw(m_shape);
}

bool Projectile::isAlive() const
{
    return m_alive;
}

void Projectile::kill()
{
    m_alive = false;
}

float Projectile::getDamage() const
{
    return m_damage;
}

sf::FloatRect Projectile::getBounds() const
{
    return m_shape.getGlobalBounds();
}

sf::Vector2f Projectile::getPosition() const
{
    return m_shape.getPosition();
}
