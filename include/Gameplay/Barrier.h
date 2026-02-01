//
// Created by Oier Álvarez on 31/01/2026.
//

#ifndef SPACESTATIONDEFENSE_BARRIER_H
#define SPACESTATIONDEFENSE_BARRIER_H
#include "Weapon.h"
#include "Gameplay/Enemy.h"


class Barrier: public Weapon
{
public:

    struct BarrierDesc
    {
        float regenCooldown = 0;
        float regenAmount = 0;
        float barrierHealth = 0;
    };

    struct BarrierUpgrades
    {
        float maxHealthUpgrade = 0;
        float regenAmountUpgrade = 0;
    };

    Barrier();
    Barrier(BarrierDesc desc);
    bool init(const BarrierDesc& barrierDescriptor);
    void update(float deltaSeconds) override;
    void render(sf::RenderWindow& window) override;
    void setUpgrades(const BarrierUpgrades& u);
    float receiveDamage(float damage);
    float getHealth() const;
    float getMaxHealth() const;

    void setOnBarrierHealthGainedFunction(const std::function<void(float health)>& func);

    /* Unused */
    void shoot() override;
    float getDamage() override;
    void applyEffectToEnemies(ObjectPool<Enemy>& enemyPool) override;

private:
    void regenBarrier();

    BarrierUpgrades m_upgrades;


    float m_regenCooldown{10};
    float m_regenTimer{0};
    float m_regenAmount{1};
    float m_baseRegenAmount{1};

    float m_health{10};
    float m_maxHealth{10};
    float m_baseMaxHealth{10};
    bool  m_damaged{false};

    std::function<void(float health)> m_onBarrierHealthGained;
};


#endif //SPACESTATIONDEFENSE_BARRIER_H