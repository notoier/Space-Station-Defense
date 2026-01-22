//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/Button.h"

#include "Utils/Constants.h"

void Button::init(const sf::Vector2f position, const sf::RectangleShape& shape, const char* text, std::function<bool()> func)
{
    setPosition(position);
    setFunction(func);
    setText(text);
    setShape(shape);
}

void Button::setPosition(const sf::Vector2f& pos)
{
    m_position = pos;
}

void Button::setFunction(const std::function<bool()>& func)
{
    onClickFunction = func;
}

void Button::setShape(const sf::RectangleShape& shape)
{
    m_shape = shape;
    m_shape.setPosition(m_position);
    m_shape.setFillColor(sf::Color(0, 0, 0, 160));
    m_shape.setOutlineThickness(2.f);
    m_shape.setOutlineColor(OUTLINE_COLOR);
}

void Button::setText(const char* text)
{
    //m_text = text;
}

bool Button::onClick() const
{
    if (onClickFunction)
    {
        return onClickFunction();
    }
    return false;
}

void Button::render(sf::RenderWindow& window) const
{
    window.draw(m_shape);
    window.draw(m_text);
}
