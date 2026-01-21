//
// Created by Oier Álvarez on 20/01/2026.
//

#include "Utils/MathUtils.h"
#include <algorithm>
#include "SFML/Graphics/Rect.hpp"

bool BoxCircumferenceCollision(const sf::Vector2f boxPosition, const float boxSize, const sf::Vector2f circlePosition, const float radius)
{
    const float closestX = std::max(boxPosition.x, std::min(circlePosition.x, boxPosition.x + boxSize));
    const float closestY = std::max(boxPosition.y, std::min(circlePosition.y, boxPosition.y + boxSize));

    const float distanceX = circlePosition.x - closestX;
    const float distanceY = circlePosition.y - closestY;

    const float distanceSquared = distanceX * distanceX + distanceY * distanceY;

    return distanceSquared <= radius * radius ;
}

float dot(const sf::Vector2f& a, const sf::Vector2f& b)
{
    return a.x * b.x + a.y * b.y;
}

float lengthSq(const sf::Vector2f& v)
{
    return dot(v,v);
}

bool SegmentCircleCollision(const sf::Vector2f& start, const sf::Vector2f& end,
                            const sf::Vector2f& center, const float radius)
{
    const sf::Vector2f d = end - start;
    const sf::Vector2f f = start - center;

    const float a = dot(d, d);

    if (a < 0.00001f) return lengthSq(f) <= radius * radius;

    float t = -dot(f ,d) / a;

    t = std::clamp(t, 0.f, 1.f);

    const sf::Vector2f closestPoint = start + d * t;
    return lengthSq(closestPoint - center) <= radius * radius;
}

bool AABBCollision(const sf::FloatRect& a, const sf::FloatRect& b)
{
    const bool CollisionX = a.getPosition().x + a.width >= b.getPosition().x && b.getPosition().x + b.width >= a.getPosition().x;
    const bool CollisionY = a.getPosition().y + a.height >= b.getPosition().y && b.getPosition().y + b.height >= a.getPosition().y;

    return CollisionX && CollisionY;
}

bool SegmentAABBCollision(const sf::Vector2f& segStart,
                          const sf::Vector2f& segEnd,
                          const sf::FloatRect& box)
{
    const sf::Vector2f d = segEnd - segStart;

    float tMin = 0.f;
    float tMax = 1.f;

    auto slab = [&](float p, float dp, float minB, float maxB) -> bool
    {
        const float eps = 0.000001f;

        if (std::abs(dp) < eps)
            return (p >= minB && p <= maxB); // parallel to axis, must be inside slab

        float t1 = (minB - p) / dp;
        float t2 = (maxB - p) / dp;
        if (t1 > t2) std::swap(t1, t2);

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);
        return tMin <= tMax;
    };

    if (!slab(segStart.x, d.x, box.left, box.left + box.width))  return false;
    if (!slab(segStart.y, d.y, box.top,  box.top  + box.height)) return false;

    return true;
}

sf::FloatRect BuildLaserAABB(const sf::Vector2f& start,
                             const sf::Vector2f& end,
                             float thickness)
{
    const float half = thickness * 0.5f;

    const float minX = std::min(start.x, end.x) - half;
    const float minY = std::min(start.y, end.y) - half;
    const float maxX = std::max(start.x, end.x) + half;
    const float maxY = std::max(start.y, end.y) + half;

    return {minX, minY, maxX - minX, maxY - minY};
}

bool SegmentPointWithinRadius(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Vector2f& center, const float radius)
{
    const sf::Vector2f d = end - start;
    const float a = dot(d, d);

    if (a < 0.000001f)
        return lengthSq(center - start) <= radius * radius;

    float t = dot(center - start, d) / a;
    t = std::clamp(t, 0.f, 1.f);

    const sf::Vector2f closest = start + d * t;
    return lengthSq(center - closest) <= radius * radius;
}
