//
// Created by Oier Álvarez on 20/01/2026.
//

#ifndef SPACESTATIONDEFENSE_MATHUTILS_H
#define SPACESTATIONDEFENSE_MATHUTILS_H
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

float dot (const sf::Vector2f& a, const sf::Vector2f& b);
float lengthSq(const sf::Vector2f& v);
sf::FloatRect BuildLaserAABB(const sf::Vector2f& start, const sf::Vector2f& end, float thickness);

// ===== Collisions ===== \\

bool BoxCircumferenceCollision(sf::Vector2f boxPosition, float boxSize, sf::Vector2f circlePosition, float radius);
bool SegmentCircleCollision(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Vector2f& center, float radius);
bool AABBCollision(const sf::FloatRect& a, const sf::FloatRect& b);
bool SegmentAABBCollision(const sf::Vector2f& segStart, const sf::Vector2f& segEnd, const sf::FloatRect& box);
bool SegmentPointWithinRadius(const sf::Vector2f& start, const sf::Vector2f& end, const sf::Vector2f& center, float radius);

#endif //SPACESTATIONDEFENSE_MATHUTILS_H