//
// Created by Oier Álvarez on 21/01/2026.
//

#ifndef SPACESTATIONDEFENSE_BUTTON_H
#define SPACESTATIONDEFENSE_BUTTON_H
#include <functional>

#include "SFML/Graphics/Color.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Shape.hpp"
#include "SFML/Graphics/Text.hpp"
#include "SFML/System/Vector2.hpp"

class Button
{
public:
    void init(sf::Vector2f position, const sf::RectangleShape& shape, const char* text, const sf::Font& font, const std::function<
                  void()>& func);

    void setPosition(const sf::Vector2f& pos);
    void setFunction(const std::function<void()>& func);
    void setShape(const sf::RectangleShape& shape);
    sf::RectangleShape getShape() const;

    void setText(const char* text);
    void onClick() const;
    void render(sf::RenderWindow& window) const;

private:
    std::function<void()> onClickFunction = nullptr;
    sf::Vector2f m_position {0.f, 0.f};
    sf::RectangleShape m_shape;
    sf::Color m_color;

    const sf::Font * m_font = nullptr;
    sf::Text m_text;

};


#endif //SPACESTATIONDEFENSE_BUTTON_H