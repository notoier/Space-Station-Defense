//
// Created by Oier Álvarez on 19/01/2026.
//

#ifndef SPACESTATIONDEFENSE_COMPOSITESHAPE_H
#define SPACESTATIONDEFENSE_COMPOSITESHAPE_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>
#include <vector>

namespace sf
{
    class Shape;
    class RenderTarget;
    class RenderStates;
}

class CompositeShape : public sf::Drawable, public sf::Transformable
{

public:

    struct Part
    {
        std::unique_ptr<sf::Shape> shape;
        float size = 0.f;

        sf::Vector2f localOffset {0, 0};

        float localRotationDeg {0.f};
        float spinSpeedDegPerSec {0.f};

        // Orbit around the composite origin (center)
        float orbitAngleDeg = 0.f;
        float orbitSpeedDegPerSec = 0.f;
        float orbitRadius = 0.f;
        float orbitPhaseDeg = 0.f;
    };

    Part& addPart(std::unique_ptr<sf::Shape> shape, float size, sf::Vector2f localOffset = {0.f, 0.f});

    void update(float dtSeconds);

    Part* getPart(std::size_t index);
    const Part* getPart(std::size_t index) const;

    CompositeShape() = default;
    ~CompositeShape() override = default;

    CompositeShape(const CompositeShape&) = delete;
    CompositeShape& operator=(const CompositeShape&) = delete;

    CompositeShape(CompositeShape&&) noexcept = default;
    CompositeShape& operator=(CompositeShape&&) noexcept = default;

private:
    void draw(sf::RenderTarget& target,
              sf::RenderStates states) const override;

    std::vector<Part> m_parts;
};

#endif //SPACESTATIONDEFENSE_COMPOSITESHAPE_H