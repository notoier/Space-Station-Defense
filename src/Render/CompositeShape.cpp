//
// Created by Oier Álvarez on 19/01/2026.
//

#include <cmath>
#include <Render/CompositeShape.h>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

CompositeShape::Part& CompositeShape::addPart(std::unique_ptr<sf::Shape> shape, const float size, const sf::Vector2f localOffset)
{
    m_parts.push_back(
        Part{
            std::move(shape),
            size,
            localOffset,
        0.f,
        0.f
        }
    );

    return m_parts.back();
}

void CompositeShape::update(float dtSeconds)
{
    for (auto& p : m_parts)
    {
        p.localRotationDeg += p.spinSpeedDegPerSec * dtSeconds;
        p.orbitAngleDeg    += p.orbitSpeedDegPerSec * dtSeconds;
    }
}

CompositeShape::Part CompositeShape::getPart(const int index)
{
    return ((index < m_parts.size()) ? std::move(m_parts[index]) : Part{});
}

void CompositeShape::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    for (const auto& p : m_parts)
    {
        sf::RenderStates localStates = states;

        // Compute orbit offset from center
        const float angleDeg = p.orbitAngleDeg + p.orbitPhaseDeg;
        const float angleRad = angleDeg * 3.14159265f / 180.f;

        const sf::Vector2f orbitOffset(
            std::cos(angleRad) * p.orbitRadius,
            std::sin(angleRad) * p.orbitRadius
        );

        localStates.transform.translate(orbitOffset);
        localStates.transform.rotate(p.localRotationDeg);

        target.draw(*p.shape, localStates);
    }
}
