//
// Created by Oier Álvarez on 19/01/2026.
//

#include "Gameplay/Station.h"

#include "Gameplay/Laser.h"

bool Station::init(const StationDescriptor& descriptor)
{
    m_size = descriptor.radius;
    return Entity::init(descriptor);
}

void Station::update(float dt)
{
    Entity::update(dt);
}

void Station::render(sf::RenderWindow& window)
{
    Entity::render(window);

    for (const auto& w : m_weapons)
        w->render(window);
}

void Station::setAimWorld(const sf::Vector2f& aim)
{
    m_aimWorld = aim;
}


const sf::Vector2f& Station::getAimWorld() const
{
    return m_aimWorld;
}

void Station::onLeftClick()
{
    shootLaser();
}


std::vector<std::unique_ptr<Weapon>>& Station::getWeapons()
{
    return m_weapons;
}

const std::vector<std::unique_ptr<Weapon>>& Station::getWeapons() const
{
    return m_weapons;
}

void Station::addWeapon(std::unique_ptr<Weapon> weapon)
{
    m_weapons.push_back(std::move(weapon));
}

void Station::shootLaser()
{
    for (auto& w : m_weapons)
    {
        if (auto* laser = dynamic_cast<Laser*>(w.get()))
        {
            laser->setEnabled(true);
            laser->shoot();
            return;
        }
    }
}
