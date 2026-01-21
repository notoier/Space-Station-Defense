//
// Created by Oier Álvarez on 19/01/2026.
//

#ifndef SPACESTATIONDEFENSE_ENEMY_H
#define SPACESTATIONDEFENSE_ENEMY_H
#include "Entity.h"


class Enemy : public Entity
{
public:
    struct EnemyDescriptor : public EntityDescriptor
    {
        sf::Vector2f velocity {0.f, 0.f};
        float size = 0.f;
        float damage = 1.f;
    };

    ~Enemy() override = default;

    bool init (const EnemyDescriptor& descriptor);
    void update (float dt) override;

    void setTarget (const sf::Vector2f& target, float targetRadius);
    bool isTargetReached() const;

protected:
    float m_damage {1.f};
    sf::Vector2f m_velocity {0.f, 0.f};
    sf::Vector2f m_targetPosition {0.f, 0.f};
    float m_targetRadius {0.f};
};


#endif //SPACESTATIONDEFENSE_ENEMY_H