//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/Button.h"

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
