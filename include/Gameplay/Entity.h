//
// Created by Oier Álvarez on 18/01/2026.
//

#ifndef SPACESTATIONDEFENSE_ENTITY_H
#define SPACESTATIONDEFENSE_ENTITY_H

#include <string>

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

    static VisualType parseVisualType(const std::string& s, const VisualType def)
    {
        if (s == "BasicEnemySquares") return VisualType::BasicEnemySquares;
        if (s == "StationCircle") return VisualType::StationCircle;

        return def;
    }

    struct EntityDescriptor
    {
        sf::Vector2f position{0.f, 0.f};
        float health{0};
        VisualType visualType{VisualType::StationCircle};
    };

    bool init(const EntityDescriptor& descriptor);
    void update(float deltaMilliseconds) override;
    void render(sf::RenderWindow& window) override;
    virtual void receiveDamage(float damage);

    float getSize() const;
    float getHealth() const;
    float getMaxHealth() const;
    sf::Vector2f getCenter() const;

    uint32_t getUID() const { return m_uid; }
    void setUID(const uint32_t uid) { m_uid = uid; }

    bool isAlive() const;

protected:
    // Allows derived classes (Enemy, Station, etc.) to customize visuals if needed.
    void buildVisual(VisualType type);

    float m_health{1};
    float m_maxHealth{1};
    bool m_isAlive{true};
    float m_size{0.f};
    uint32_t m_uid = 0;

    CompositeShape m_visual;
};


#endif //SPACESTATIONDEFENSE_ENTITY_H