//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_BUTTON_H
#define SPACESTATIONDEFENSE_BUTTON_H
#include <functional>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"

class Button
{
public:
    void init(sf::Vector2f position, const sf::RectangleShape& shape, const char* text, std::function<bool()> func);
    void setPosition(const sf::Vector2f& pos);
    void setFunction(const std::function<bool()>& func);
    void setShape(const sf::RectangleShape& shape);
    void setText(const char* text);
    bool onClick() const;

private:
    std::function<bool()> onClickFunction = nullptr;
    sf::Vector2f m_position {0.f, 0.f};
    sf::RectangleShape m_shape;
    sf::Color m_color;
    sf::Text m_text;

};


#endif //SPACESTATIONDEFENSE_BUTTON_H