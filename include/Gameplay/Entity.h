//
// Created by Oier Álvarez on 18/01/2026.
//

#ifndef SPACESTATIONDEFENSE_ENTITY_H
#define SPACESTATIONDEFENSE_ENTITY_H

#include "Gameplay/GameObject.h"
#include "Render/CompositeShape.h"
#include "SFML/Graphics/Shape.hpp"

class Entity : public GameObject
{
public:
    enum class VisualType
    {
        StationCircle,
        BasicEnemySquares
    };

    struct EntityDescriptor
    {
        sf::Vector2f position{0.f, 0.f};
        float health{0};
        VisualType visualType{VisualType::StationCircle};
    };

public:
    bool init(const EntityDescriptor& descriptor);
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    void receiveDamage(float damage);


    sf::Vector2f getCenter() const;

    bool isAlive() const;

protected:
    // Allows derived classes (Enemy, Station, etc.) to customize visuals if needed.
    void buildVisual(VisualType type);

protected:
    float m_health{1};
    bool m_isAlive{true};
    float m_size{0.f};

    CompositeShape m_visual;
};


#endif //SPACESTATIONDEFENSE_ENTITY_H