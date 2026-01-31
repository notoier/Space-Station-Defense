//
// Created by Oier Álvarez on 20/01/2026.
//

#ifndef SPACESTATIONDEFENSE_WEAPON_H
#define SPACESTATIONDEFENSE_WEAPON_H
#include "Projectile.h"
#include "Render/CompositeShape.h"
#include "Utils/Constants.h"
#include "Core/ObjectPool.h"
#include <functional>
#include "Gameplay/Enemy.h"

namespace sf
{
    class RenderWindow;
}

class World;

class Weapon
{

public:

    enum WeaponType
    {
        Laser,
        Cannon,
        Barrier,
    };

    struct WeaponContext
    {
        sf::Vector2f originWorld = STATION_POSITION;   // Station position
        sf::Vector2f aimWorld {0.f, 0.f};      // Mouse in world coords
    };

    virtual ~Weapon() = default;
    virtual void update(float dtMilliseconds) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void shoot() = 0;

    virtual float getDamage() = 0;
    virtual void applyEffectToEnemies(ObjectPool<Enemy>& enemyPool) = 0;

    bool isEnabled() const { return m_enabled; }
    void setWeaponContext(const WeaponContext ctx) {m_context = ctx;}
    void setSpawnProjectileFn(std::function<void(const Projectile::ProjectileDescriptor&)> fn) { m_spawnProjectile = std::move(fn); }

protected:

    bool m_enabled = true;
    std::function<void(const Projectile::ProjectileDescriptor&)> m_spawnProjectile;
    CompositeShape m_shape;
    WeaponContext m_context;
};


#endif //SPACESTATIONDEFENSE_WEAPON_H