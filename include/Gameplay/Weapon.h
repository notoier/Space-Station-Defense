//
// Created by Oier Álvarez on 20/01/2026.
//

#ifndef SPACESTATIONDEFENSE_WEAPON_H
#define SPACESTATIONDEFENSE_WEAPON_H
#include "Core/World.h"
#include "Render/CompositeShape.h"
#include "Utils/Constants.h"


namespace sf
{
    class RenderWindow;
}

class World;

class Weapon
{

public:

    struct WeaponContext
    {
        sf::Vector2f originWorld = STATION_POSITION;   // Station position
        sf::Vector2f aimWorld {0.f, 0.f};      // Mouse in world coords
    };

    virtual ~Weapon() = default;
    virtual void update(float dtMilliseconds) = 0;
    virtual void render(sf::RenderWindow& window) = 0;
    virtual void shoot() = 0;
    void setWeaponContext(const WeaponContext ctx) {m_context = ctx;}

protected:
    CompositeShape m_shape;
    WeaponContext m_context;
    bool m_enabled = true;
};


#endif //SPACESTATIONDEFENSE_WEAPON_H