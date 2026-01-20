//
// Created by Oier Álvarez on 20/01/2026.
//

#include "Utils/MathUtils.h"
#include <algorithm>

bool BoxCircumferenceCollision(const sf::Vector2f boxPosition, const float boxSize, const sf::Vector2f circlePosition, const float radius)
{
    const float closestX = std::max(boxPosition.x, std::min(circlePosition.x, boxPosition.x + boxSize));
    const float closestY = std::max(boxPosition.y, std::min(circlePosition.y, boxPosition.y + boxSize));

    const float distanceX = circlePosition.x - closestX;
    const float distanceY = circlePosition.y - closestY;

    const float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= radius * radius ;
}
