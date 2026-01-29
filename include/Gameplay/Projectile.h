//
// Created by Oier Álvarez on 27/01/2026.
//

#ifndef SPACESTATIONDEFENSE_PROJECTILE_H
#define SPACESTATIONDEFENSE_PROJECTILE_H

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>

class Projectile
{
public:
    struct ProjectileDescriptor
    {
        sf::Vector2f position {0.f, 0.f};
        sf::Vector2f velocity {0.f, 0.f};
        float radius = 3.f;
        float damage = 1.f;
        float maxLifeSeconds = 3.f;
    };

    bool init(const ProjectileDescriptor& d);

    void update(float dtSeconds);
    void render(sf::RenderWindow& window);

    bool isAlive() const;
    void kill();

    float getDamage() const;

    sf::FloatRect getBounds() const;
    sf::Vector2f getPosition() const;

private:
    sf::CircleShape m_shape;
    sf::Vector2f m_velocity {0.f, 0.f};
    float m_damage = 1.f;
    float m_life = 0.f;
    bool m_alive = false;
};

#endif // SPACESTATIONDEFENSE_PROJECTILE_H
