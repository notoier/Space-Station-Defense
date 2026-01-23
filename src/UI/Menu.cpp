//
// Created by Oier Álvarez on 21/01/2026.
//

#include "UI/Menu.h"

#include <iostream>
#include <ostream>

#include "UI/Button.h"

Menu::Menu() = default;
Menu::~Menu() = default;

bool Menu::init()
{
    return true;
}

void Menu::setMenuTitle(const sf::Text& text)
{
    m_text = text;
}

void Menu::setTitlePosition(const sf::Vector2f& position)
{
    m_text.setPosition(position);
}

void Menu::createButton(const sf::Vector2f position, const sf::RectangleShape& shape, const char* text,
    const std::function<void()>& func)
{
    Button button;
    button.init(position, shape, text, m_font, func);
    m_buttons.push_back(std::make_unique<Button>(button));
}

void Menu::onLeftClick(const sf::Vector2f mousePos) const
{
    for (const auto& button : m_buttons)
    {
        if (sf::RectangleShape bShape = button->getShape(); bShape.getGlobalBounds().contains(mousePos))
        {
            button->onClick();
        }

    }
}

void Menu::enable(const bool enable)
{
    m_enabled = enable;
}

bool Menu::isEnabled() const
{
    return m_enabled;
}

