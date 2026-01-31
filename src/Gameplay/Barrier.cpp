//
// Created by Oier Álvarez on 31/01/2026.
//

#include "../../include/Gameplay/Barrier.h"

Barrier::Barrier()
{
    BarrierDesc barrierDesc;
    barrierDesc.regenAmount = 1;
    barrierDesc.regenCooldown = 10;
    barrierDesc.barrierHealth = 10;

    init(barrierDesc);
}

Barrier::Barrier(const BarrierDesc desc)
{
    init(desc);
}

bool Barrier::init(const BarrierDesc& barrierDescriptor)
{
    m_regenCooldown = barrierDescriptor.regenCooldown;
    m_regenAmount = barrierDescriptor.regenAmount;
    m_health = barrierDescriptor.barrierHealth;
    m_maxHealth = barrierDescriptor.barrierHealth;

    std::unique_ptr<sf::Shape> barrierShape = std::make_unique<sf::CircleShape>(BARRIER_SIZE);
    barrierShape->setOrigin(BARRIER_SIZE, BARRIER_SIZE);
    barrierShape->setPosition(STATION_POSITION);
    barrierShape->setOutlineColor(BARRIER_COLOR);
    barrierShape->setFillColor(sf::Color::Transparent);
    barrierShape->setOutlineThickness(3);
    m_shape.addPart(std::move(barrierShape), BARRIER_SIZE, {0,0});
    return true;
}


void Barrier::update(const float deltaSeconds)
{
    if (!m_damaged) return;

    m_regenTimer += deltaSeconds;
    if (m_regenTimer >= m_regenCooldown) regenBarrier();
}

void Barrier::render(sf::RenderWindow& window)
{
    window.draw(m_shape);
}

void Barrier::regenBarrier()
{
    m_health = std::min(m_health + m_regenAmount, m_maxHealth);
    m_regenTimer = 0.f;
    m_onBarrierHealthGained(m_health / m_maxHealth);
}

float Barrier::receiveDamage(const float damage)
{
    if (damage <= 0.f)
        return 0.f;

    m_damaged = true;

    const float prev = m_health;

    if (damage >= prev)
    {
        m_health = 0.f;
        return damage - prev; /* Return remaining damage */
    }

    m_health -= damage;
    return 0.f;
}

float Barrier::getHealth() const
{
    return m_health;
}

void Barrier::shoot(){}
float Barrier::getDamage(){return 0;}
void Barrier::applyEffectToEnemies(ObjectPool<Enemy>& enemyPool){}

float Barrier::getMaxHealth() const
{
    return m_maxHealth;
}

void Barrier::setOnBarrierHealthGainedFunction(const std::function<void(float health)>& func)
{
    m_onBarrierHealthGained = func;
}
